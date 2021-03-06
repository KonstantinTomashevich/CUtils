#include "PVector.h"
#include "Utils.h"
#include "Errors.h"

#include <ShortTypes.h>
#include <stdlib.h>

#define BUFFER_INCREASE_MODIFIER 1

IOneDirectionIterator PVectorIterator_IOneDirectionIterator =
        {
            PVectorIterator_Next,
            PVectorIterator_Jump,
            PVectorIterator_ValueAt
        };

IBiDirectionalIterator PVectorIterator_IBiDirectionalIterator =
        {
                PVectorIterator_Next,
                PVectorIterator_Jump,
                PVectorIterator_Previous,
                PVectorIterator_JumpBack,
                PVectorIterator_ValueAt
        };

ISizedContainer PVector_ISizedContainer =
        {
            PVector_Size
        };

IIterableContainer PVector_IIterableContainer =
        {
            PVector_Begin,
            PVector_End,
            PVector_At
        };

IMutableContainer PVector_IMutableContainer =
        {
            PVector_Insert,
            PVector_Erase
        };

typedef struct
{
    ulint capacity;
    ulint size;
    void **buffer;
} PVector;

void PVectorCallback_MoveLeft (void **item)
{
    *item = *(item + 1);
}

void PVectorCallback_MoveRight (void **item)
{
    *item = *(item - 1);
}

PVectorHandle PVector_Create (ulint initialCapacity)
{
    PVector *vector = malloc (sizeof (PVector));
    vector->capacity = initialCapacity;
    vector->size = 0;
    vector->buffer = malloc (sizeof (void *) * initialCapacity);
    return (PVectorHandle) vector;
}

void PVector_Destruct (PVectorHandle handle, void (*DestructCallback) (void **item))
{
    PVector *vector = (PVector *) handle;
    CContainers_ForEach (PVector_Begin (handle), PVector_End (handle), 
            PVectorIterator_AsIOneDirectionIterator (), DestructCallback);
    
    free (vector->buffer);
    free (vector);
}

ulint PVector_Size (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return vector->size;
}

PVectorIterator PVector_Begin (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return (PVectorIterator) vector->buffer;
}

PVectorIterator PVector_End (PVectorHandle handle)
{
    PVector *vector = (PVector *) handle;
    return (PVectorIterator) (vector->buffer + vector->size);
}

PVectorIterator PVector_At (PVectorHandle handle, ulint index)
{
    PVector *vector = (PVector *) handle;
    if (index >= vector->size)
    {
        CContainers_SetLastError (PVECTOR_ERROR_OUT_OF_BOUNDS);
        return NULL;
    }

    return PVectorIterator_Jump ((PVectorIterator) vector->buffer, index);
}

PVectorIterator PVector_Insert (PVectorHandle handle, PVectorIterator where, void *value)
{
    PVector *vector = (PVector *) handle;
    if (where < PVector_Begin (handle) || where > PVector_End (handle))
    {
        CContainers_SetLastError (PVECTOR_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    vector->size += 1;
    if (vector->size > vector->capacity)
    {
        ulint index = where - PVector_Begin (handle);
        vector->capacity += (ulint) (vector->capacity * BUFFER_INCREASE_MODIFIER);
        vector->buffer = realloc (vector->buffer, sizeof (void **) * vector->capacity);
        where = PVector_Begin (handle) + index;
    }

    if (where < PVectorIterator_Previous (PVector_End (handle)))
    {
        CContainers_ForEachReversed (PVectorIterator_Next (where), PVectorIterator_Previous (PVector_End (handle)),
                PVectorIterator_AsIBiDirectionalIterator (), PVectorCallback_MoveRight);
    }
    
    *PVectorIterator_ValueAt (where) = value;
    return where;
}

PVectorIterator PVector_Erase (PVectorHandle handle, PVectorIterator iterator)
{
    PVector *vector = (PVector *) handle;
    if (iterator < PVector_Begin (handle) || iterator >= PVector_End (handle))
    {
        CContainers_SetLastError (PVECTOR_ERROR_INCORRECT_ITERATOR);
        return NULL;
    }

    vector->size -= 1;
    CContainers_ForEach (iterator, PVector_End (handle),
            PVectorIterator_AsIOneDirectionIterator (), PVectorCallback_MoveLeft);

    if (iterator >= PVector_End (handle))
    {
        return PVector_End (handle);
    }
    else
    {
        return iterator;
    }
}

PVectorIterator PVectorIterator_Next (PVectorIterator iterator)
{
    return ++iterator;
}

PVectorIterator PVectorIterator_Jump (PVectorIterator iterator, ulint distance)
{
    return iterator + distance;
}

PVectorIterator PVectorIterator_Previous (PVectorIterator iterator)
{
    return --iterator;
}

PVectorIterator PVectorIterator_JumpBack (PVectorIterator iterator, ulint distance)
{
    return iterator - distance;
}

void **PVectorIterator_ValueAt (PVectorIterator iterator)
{
    return iterator;
}

IOneDirectionIterator *PVectorIterator_AsIOneDirectionIterator ()
{
    return &PVectorIterator_IOneDirectionIterator;
}

IBiDirectionalIterator *PVectorIterator_AsIBiDirectionalIterator ()
{
    return &PVectorIterator_IBiDirectionalIterator;
}

ISizedContainer *PVector_AsISizedContainer ()
{
    return &PVector_ISizedContainer;
}

IIterableContainer *PVector_AsIIterableContainer ()
{
    return &PVector_IIterableContainer;
}

IMutableContainer *PVector_AsIMutableContainer ()
{
    return &PVector_IMutableContainer;
}
