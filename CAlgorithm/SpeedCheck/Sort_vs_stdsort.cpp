#include "Sort_vs_stdsort.hpp"
#include <algorithm>
#include <ctime>
#include <vector>
#include <cstdio>

extern "C"
{
#include <CAlgorithm/Sort.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>
}

#define RAND_SEED 147312
#define TEST_AMOUNT 1000000

static lint Comparator (const void *first, const void *second)
{
    return (const lint) second - (const lint) first;
}

static clock_t Check_HeapSort ()
{
    PVectorHandle vector = PVector_Create (TEST_AMOUNT);
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PVector_Insert (vector, PVector_End (vector), (void *) ((lint) rand ()));
    }

    clock_t begin = clock ();
    HeapSort (vector, PVector_AsISizedContainer (), PVector_AsIIterableContainer (),
            PVectorIterator_AsIOneDirectionIterator (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static clock_t Check_stdsort ()
{
    auto *vector = new std::vector <lint> ();
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        vector->push_back ((lint) rand ());
    }

    clock_t begin = clock ();
    std::sort (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

void Sort_vs_stdsort ()
{
    printf ("Sort: amount of items -- %d.\n", TEST_AMOUNT);
    // FIXME: Heap sort speed is too low. Maybe it is because of extensive copying?
    printf ("HeapSort: %dms.\n", (int) (Check_HeapSort () * 1000 / CLOCKS_PER_SEC));
    printf ("std::sort: %dms.\n", (int) (Check_stdsort () * 1000 / CLOCKS_PER_SEC));
}