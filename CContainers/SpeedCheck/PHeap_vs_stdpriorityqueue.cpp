#include "PHeap_vs_stdpriorityqueue.hpp"
#include <ctime>
#include <queue>
#include <cstdio>
#include <cstdlib>

extern "C"
{
#include <ShortTypes.h>
#include <CContainers/PHeap.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
}

#define SRAND_SEED 154632
#define TEST_AMOUNT 1000000
#define DEFAULT_INITIAL_CAPACITY 100

static void EmptyDestruct (void **item)
{

}

static lint Comparator (const void *first, const void *second)
{
    return (lint) first - (lint) second;
}

clock_t PHeap_Insert (bool reserve)
{
    clock_t begin = clock ();
    srand (SRAND_SEED);

    PHeapHandle heap = PHeap_Create (reserve ? TEST_AMOUNT : DEFAULT_INITIAL_CAPACITY, Comparator);
    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PHeap_Push (heap, (void *) rand ());
    }

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PHeap_Pop (heap);
    }

    PHeap_Destruct (heap, PVector_Destruct, EmptyDestruct);
    return clock () - begin;
}

clock_t stdpriorityqueue_Insert ()
{
    clock_t begin = clock ();
    srand (SRAND_SEED);
    auto queue = new std::priority_queue <int> ();

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        queue->push (rand ());
    }

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        queue->pop ();
    }

    delete queue;
    return clock () - begin;
}

void PHeap_vs_stdpriorityqueue ()
{
    printf ("PHeap: items to insert -- %d.\n", TEST_AMOUNT);
    printf ("PHeap, no reserve: %dms.\n", (int) (PHeap_Insert (false) * 1000 / CLOCKS_PER_SEC));
    printf ("PHeap, reserve: %dms.\n", (int) (PHeap_Insert (true) * 1000 / CLOCKS_PER_SEC));
    printf ("std::priority_queue: %dms.\n", (int) (stdpriorityqueue_Insert () * 1000 / CLOCKS_PER_SEC));
}
