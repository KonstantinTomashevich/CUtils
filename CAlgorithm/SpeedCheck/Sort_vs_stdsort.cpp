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

#define RAND_SEED 24315
#define TEST_AMOUNT 2000000

static lint Comparator (const void *first, const void *second)
{
    return (const lint) second - (const lint) first;
}

static PVectorHandle CreateVector ()
{
    PVectorHandle vector = PVector_Create (TEST_AMOUNT);
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        PVector_Insert (vector, PVector_End (vector), (void *) ((lint) rand ()));
    }

    return vector;
}

static clock_t Check_HeapSort ()
{
    PVectorHandle vector = CreateVector ();
    clock_t begin = clock ();

    HeapSort (vector, PVector_AsISizedContainer (), PVector_AsIIterableContainer (),
            PVectorIterator_AsIBiDirectionalIterator (), PVector_AsIMutableContainer (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static clock_t Check_MergeSort ()
{
    PVectorHandle vector = CreateVector ();
    clock_t begin = clock ();

    MergeSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIOneDirectionIterator (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static clock_t Check_InplaceMergeSort ()
{
    PVectorHandle vector = CreateVector ();
    clock_t begin = clock ();

    InplaceMergeSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIBiDirectionalIterator (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static clock_t Check_QuickSort ()
{
    PVectorHandle vector = CreateVector ();
    clock_t begin = clock ();

    QuickSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIBiDirectionalIterator (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static clock_t Check_IntroSort ()
{
    PVectorHandle vector = CreateVector ();
    clock_t begin = clock ();

    IntroSort (PVector_Begin (vector), PVector_End (vector), PVector_Size (vector),
            PVectorIterator_AsIBiDirectionalIterator (), Comparator);

    PVector_Destruct (vector, ContainerCallback_NoAction);
    return clock () - begin;
}

static std::vector <lint> *CreateStdVector ()
{
    auto *vector = new std::vector <lint> ();
    srand (RAND_SEED);

    for (int index = 0; index < TEST_AMOUNT; ++index)
    {
        vector->push_back ((lint) rand ());
    }

    return vector;
}

static clock_t Check_stdsort ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();
    std::sort (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

static clock_t Check_stdsortheap ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();

    std::make_heap (vector->begin (), vector->end ());
    std::sort_heap (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

static clock_t Check_stdstablesort ()
{
    auto *vector = CreateStdVector ();
    clock_t begin = clock ();
    std::stable_sort (vector->begin (), vector->end ());

    delete vector;
    return clock () - begin;
}

void Sort_vs_stdsort ()
{
    printf ("Sort: amount of items -- %d.\n", TEST_AMOUNT);
    printf ("HeapSort: %dms.\n", (int) (Check_HeapSort () * 1000 / CLOCKS_PER_SEC));
    printf ("MergeSort: %dms.\n", (int) (Check_MergeSort () * 1000 / CLOCKS_PER_SEC));
    printf ("InplaceMergeSort: %dms.\n", (int) (Check_InplaceMergeSort () * 1000 / CLOCKS_PER_SEC));
    printf ("QuickSort: %dms.\n", (int) (Check_QuickSort () * 1000 / CLOCKS_PER_SEC));
    printf ("IntroSort: %dms.\n", (int) (Check_IntroSort () * 1000 / CLOCKS_PER_SEC));

    printf ("std::sort: %dms.\n", (int) (Check_stdsort () * 1000 / CLOCKS_PER_SEC));
    printf ("std::sort_heap: %dms.\n", (int) (Check_stdsortheap () * 1000 / CLOCKS_PER_SEC));
    printf ("std::stable_sort: %dms.\n", (int) (Check_stdstablesort () * 1000 / CLOCKS_PER_SEC));
}
