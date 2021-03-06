#include "PHashMap.h"
#include "Utils.h"
#include "Errors.h"
#include "PVector.h"
#include <stdlib.h>

#define BUCKET_COUNT_INCREASE_FACTOR 1.5

ISizedContainer PHashMap_ISizedContainer =
        {
            PHashMap_Size
        };

IMapContainer PHashMap_IMapContainer =
        {
            PHashMap_ContainsKey,
            PHashMap_Insert,
            PHashMap_GetValue,
            PHashMap_Erase
        };

typedef struct
{
    void *key;
    void *value;
} KeyValuePair;

typedef struct
{
    ulint size;
    ulint maxLoad;
    /// Vector of vector handles, each inner vector is key-value pair.
    PVectorHandle buckets;

    ulint (*HashFunction) (void *key);
    lint (*KeyComparator) (void *first, void *second);
} PHashMap;

static void PHashMap_BucketDestroyCallbackKeepPairs (void **bucket)
{
    PVector_Destruct (*bucket, ContainerCallback_NoAction);
}

static void PHashMap_BucketDestroyCallback (void **bucket)
{
    PVector_Destruct (*bucket, ContainerCallback_Free);
}

static void PHashMap_InitBuckets (PHashMap *hashMap, ulint bucketsCount)
{
    PVectorHandle buckets = PVector_Create (bucketsCount);
    while (bucketsCount--)
    {
        PVectorHandle bucket = PVector_Create (hashMap->maxLoad);
        PVector_Insert (buckets, PVector_End (buckets), bucket);
    }

    hashMap->buckets = buckets;
}

static PVectorHandle PHashMap_GetBucket (PHashMap *hashMap, void *key)
{
    ulint bucketIndex = hashMap->HashFunction (key) % PVector_Size (hashMap->buckets);
    return *PVectorIterator_ValueAt (PVector_At (hashMap->buckets, bucketIndex));
}

static PVectorIterator PHashMap_GetPairIteratorInBucket (PHashMap *hashMap, PVectorHandle bucket, void *key)
{
    PVectorIterator keyValueIterator = PVector_Begin (bucket);
    while (keyValueIterator != PVector_End (bucket))
    {
        KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
        if (hashMap->KeyComparator (key, pair->key) == 0)
        {
            return keyValueIterator;
        }

        keyValueIterator = PVectorIterator_Next (keyValueIterator);
    }

    return PVector_End (bucket);
}

static KeyValuePair *PHashMap_GetPairInBucket (PHashMap *hashMap, PVectorHandle bucket, void *key)
{
    PVectorIterator keyValueIterator = PHashMap_GetPairIteratorInBucket (hashMap, bucket, key);
    if (keyValueIterator != PVector_End (bucket))
    {
        return *PVectorIterator_ValueAt (keyValueIterator);
    }
    else
    {
        return NULL;
    }
}

static KeyValuePair *PHashMap_GetPair (PHashMap *hashMap, void *key)
{
    return PHashMap_GetPairInBucket (hashMap, PHashMap_GetBucket (hashMap, key), key);
}

static void PHashMap_Rehash (PHashMap *hashMap)
{
    PVectorHandle oldBuckets = hashMap->buckets;
    PHashMap_InitBuckets (hashMap, (ulint) (PVector_Size (oldBuckets) * BUCKET_COUNT_INCREASE_FACTOR));
    PVectorIterator bucketIterator = PVector_Begin (oldBuckets);

    while (bucketIterator != PVector_End (oldBuckets))
    {
        PVectorHandle bucket = *PVectorIterator_ValueAt (bucketIterator);
        PVectorIterator keyValueIterator = PVector_Begin (bucket);

        while (keyValueIterator != PVector_End (bucket))
        {
            KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
            PVectorHandle newBucket = PHashMap_GetBucket (hashMap, pair->key);
            PVector_Insert (newBucket, PVector_End (newBucket), pair);
            keyValueIterator = PVectorIterator_Next (keyValueIterator);
        }

        bucketIterator = PVectorIterator_Next (bucketIterator);
    }

    PVector_Destruct (oldBuckets, PHashMap_BucketDestroyCallbackKeepPairs);
}

PHashMapHandle PHashMap_Create (ulint initialBucketCount, ulint maxBucketLoad,
        ulint (*HashKey) (void *key), lint (*KeyCompare) (void *first, void *second))
{
    PHashMap *hashMap = malloc (sizeof (PHashMap));
    hashMap->size = 0;
    hashMap->maxLoad = maxBucketLoad;

    hashMap->HashFunction = HashKey;
    hashMap->KeyComparator = KeyCompare;
    PHashMap_InitBuckets (hashMap, initialBucketCount);
    return (PHashMapHandle) hashMap;
}

void PHashMap_Destruct (PHashMapHandle handle, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value))
{
    PHashMap *hashMap = (PHashMap *) handle;
    PVectorIterator bucketIterator = PVector_Begin (hashMap->buckets);

    while (bucketIterator != PVector_End (hashMap->buckets))
    {
        PVectorHandle bucket = *PVectorIterator_ValueAt (bucketIterator);
        PVectorIterator keyValueIterator = PVector_Begin (bucket);

        while (keyValueIterator != PVector_End (bucket))
        {
            KeyValuePair *pair = *PVectorIterator_ValueAt (keyValueIterator);
            KeyDestructCallback (&(pair->key));
            ValueDestructCallback (&(pair->value));
            keyValueIterator = PVectorIterator_Next (keyValueIterator);
        }

        bucketIterator = PVectorIterator_Next (bucketIterator);
    }

    PVector_Destruct (hashMap->buckets, PHashMap_BucketDestroyCallback);
}

ulint PHashMap_Size (PHashMapHandle handle)
{
    PHashMap *hashMap = (PHashMap *) handle;
    return hashMap->size;
}

char PHashMap_ContainsKey (PHashMapHandle handle, void *key)
{
    PHashMap *hashMap = (PHashMap *) handle;
    return PHashMap_GetPair (hashMap, key) != NULL;
}

char PHashMap_Insert (PHashMapHandle handle, void *key, void *value)
{
    PHashMap *hashMap = (PHashMap *) handle;
    PVectorHandle bucket = PHashMap_GetBucket (hashMap, key);
    KeyValuePair *pair = PHashMap_GetPairInBucket (hashMap, bucket, key);

    if (pair != NULL)
    {
        return 0;
    }

    pair = malloc (sizeof (KeyValuePair));
    pair->key = key;
    pair->value = value;
    hashMap->size += 1;
    PVector_Insert (bucket, PVector_End (bucket), pair);

    if (PVector_Size (bucket) > hashMap->maxLoad)
    {
        PHashMap_Rehash (hashMap);
    }

    return 1;
}

void **PHashMap_GetValue (PHashMapHandle handle, void *key)
{
    PHashMap *hashMap = (PHashMap *) handle;
    KeyValuePair *pair = PHashMap_GetPair (hashMap, key);
    return pair != NULL ? &(pair->value) : NULL;
}

char PHashMap_Erase (PHashMapHandle handle, void *key, void (*KeyDestructCallback) (void **key),
        void (*ValueDestructCallback) (void **value))
{
    PHashMap *hashMap = (PHashMap *) handle;
    PVectorHandle bucket = PHashMap_GetBucket (hashMap, key);
    PVectorIterator iterator = PHashMap_GetPairIteratorInBucket (hashMap, bucket, key);

    if (iterator == PVector_End (bucket))
    {
        return 0;
    }
    else
    {
        hashMap->size -= 1;
        KeyValuePair *pair = *PVectorIterator_ValueAt (iterator);;

        KeyDestructCallback (&(pair->key));
        ValueDestructCallback (&(pair->value));

        PVector_Erase (bucket, iterator);
        free (pair);
        return 1;
    }
}

ISizedContainer *PHashMap_AsISizedContainer ()
{
    return &PHashMap_ISizedContainer;
}

IMapContainer *PHashMap_AsIMapContainer ()
{
    return &PHashMap_IMapContainer;
}
