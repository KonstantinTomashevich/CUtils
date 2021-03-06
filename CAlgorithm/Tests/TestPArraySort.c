#include "TestPArraySort.h"
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/CUnitCI.h>

#include <CAlgorithm/PArraySort.h>
#include <CContainers/PVector.h>
#include <CContainers/Utils.h>

#define FIRST_TEST_SIZE 7
static int FirstTest_Input [] = {1, 2, 3, 4, 5, 6, 7};
static int FirstTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define SECOND_TEST_SIZE 7
static int SecondTest_Input [] = {7, 6, 5, 4, 3, 2, 1};
static int SecondTest_Output [] = {1, 2, 3, 4, 5, 6, 7};

#define THIRD_TEST_SIZE 8
static int ThirdTest_Input [] = {34, 12, 31, 66, 98, 45, 23, 11};
static int ThirdTest_Output [] = {11, 12, 23, 31, 34, 45, 66, 98};

#define FOURTH_TEST_SIZE 10
static int FourthTest_Input [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};
static int FourthTest_Output [] = {1, 1, 1, 2, 3, 4, 4, 5, 6, 7};

#define FIFTH_TEST_SIZE 10
static int FifthTest_Input [] = {7, 7, 7, 6, 5, 5, 4, 3, 2, 1};
static int FifthTest_Output [] = {1, 2, 3, 4, 5, 5, 6, 7, 7, 7};

#define SIXTH_TEST_SIZE 11
static int SixthTest_Input [] = {34, 12, 34, 31, 66, 11, 45, 98, 45, 23, 11};
static int SixthTest_Output [] = {11, 11, 12, 23, 31, 34, 34, 45, 45, 66, 98};

static lint Comparator (const void *first, const void *second)
{
    return (lint) (*(const int *) second - *(const int *) first);
}

static void PArraySortSuite_Helper_FillVector (void **vector, uint size, int *input)
{
    while (size--)
    {
        *vector = input;
        ++vector;
        ++input;
    }
}

void PArraySortSuite_Helper_CheckVector (void **vector, uint size, const int *expectedOutput)
{
    int index = 0;
    while (size--)
    {
        int actual = **(int **) vector;
        int expected = *expectedOutput;

        if (actual != expected)
        {
            printf ("\n    Error at index %d (e/a): %d, %d.", index, expected, actual);
            CU_FAIL ("Incorrect order found!");
        }

        ++index;
        ++vector;
        ++expectedOutput;
    }
}

static void PArraySortSuite_Helper_MergeSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PArraySortSuite_Helper_FillVector (vector, size, input);
    PArrayMergeSort (vector, vector + size, Comparator);

    PArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PArraySortSuite_Helper_InplaceMergeSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PArraySortSuite_Helper_FillVector (vector, size, input);
    PArrayInplaceMergeSort (vector, vector + size, Comparator);

    PArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PArraySortSuite_Helper_HeapSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PArraySortSuite_Helper_FillVector (vector, size, input);
    PArrayHeapSort (vector, vector + size, Comparator);

    PArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PArraySortSuite_Helper_QuickSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PArraySortSuite_Helper_FillVector (vector, size, input);
    PArrayQuickSort (vector, vector + size, Comparator);

    PArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PArraySortSuite_Helper_IntroSortTest (uint size, int *input, const int *expectedOutput)
{
    void **vector = malloc (sizeof (void *) * size);
    PArraySortSuite_Helper_FillVector (vector, size, input);
    PArrayIntroSort (vector, vector + size, Comparator);

    PArraySortSuite_Helper_CheckVector (vector, size, expectedOutput);
    free (vector);
}

static void PArraySortSuite_Setup ()
{

}

static void PArraySortSuite_Teardown ()
{

}

static void PArraySortSuite_MergeFirst ()
{
    PArraySortSuite_Helper_MergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PArraySortSuite_MergeSecond ()
{
    PArraySortSuite_Helper_MergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PArraySortSuite_MergeThird ()
{
    PArraySortSuite_Helper_MergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PArraySortSuite_MergeFourth ()
{
    PArraySortSuite_Helper_MergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PArraySortSuite_MergeFifth ()
{
    PArraySortSuite_Helper_MergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PArraySortSuite_MergeSixth ()
{
    PArraySortSuite_Helper_MergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PArraySortSuite_InplaceMergeFirst ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PArraySortSuite_InplaceMergeSecond ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PArraySortSuite_InplaceMergeThird ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PArraySortSuite_InplaceMergeFourth ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PArraySortSuite_InplaceMergeFifth ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PArraySortSuite_InplaceMergeSixth ()
{
    PArraySortSuite_Helper_InplaceMergeSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PArraySortSuite_HeapFirst ()
{
    PArraySortSuite_Helper_HeapSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PArraySortSuite_HeapSecond ()
{
    PArraySortSuite_Helper_HeapSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PArraySortSuite_HeapThird ()
{
    PArraySortSuite_Helper_HeapSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PArraySortSuite_HeapFourth ()
{
    PArraySortSuite_Helper_HeapSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PArraySortSuite_HeapFifth ()
{
    PArraySortSuite_Helper_HeapSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PArraySortSuite_HeapSixth ()
{
    PArraySortSuite_Helper_HeapSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PArraySortSuite_QuickFirst ()
{
    PArraySortSuite_Helper_QuickSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PArraySortSuite_QuickSecond ()
{
    PArraySortSuite_Helper_QuickSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PArraySortSuite_QuickThird ()
{
    PArraySortSuite_Helper_QuickSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PArraySortSuite_QuickFourth ()
{
    PArraySortSuite_Helper_QuickSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PArraySortSuite_QuickFifth ()
{
    PArraySortSuite_Helper_QuickSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PArraySortSuite_QuickSixth ()
{
    PArraySortSuite_Helper_QuickSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

static void PArraySortSuite_IntroFirst ()
{
    PArraySortSuite_Helper_IntroSortTest (FIRST_TEST_SIZE, FirstTest_Input, FirstTest_Output);
}

static void PArraySortSuite_IntroSecond ()
{
    PArraySortSuite_Helper_IntroSortTest (SECOND_TEST_SIZE, SecondTest_Input, SecondTest_Output);
}

static void PArraySortSuite_IntroThird ()
{
    PArraySortSuite_Helper_IntroSortTest (THIRD_TEST_SIZE, ThirdTest_Input, ThirdTest_Output);
}

static void PArraySortSuite_IntroFourth ()
{
    PArraySortSuite_Helper_IntroSortTest (FOURTH_TEST_SIZE, FourthTest_Input, FourthTest_Output);
}

static void PArraySortSuite_IntroFifth ()
{
    PArraySortSuite_Helper_IntroSortTest (FIFTH_TEST_SIZE, FifthTest_Input, FifthTest_Output);
}

static void PArraySortSuite_IntroSixth ()
{
    PArraySortSuite_Helper_IntroSortTest (SIXTH_TEST_SIZE, SixthTest_Input, SixthTest_Output);
}

void RegisterPArraySortSuite ()
{
    CU_CI_DEFINE_SUITE ("Sort", NULL, NULL, PArraySortSuite_Setup, PArraySortSuite_Teardown);
    CUNIT_CI_TEST (PArraySortSuite_MergeFirst);
    CUNIT_CI_TEST (PArraySortSuite_MergeSecond);
    CUNIT_CI_TEST (PArraySortSuite_MergeThird);
    CUNIT_CI_TEST (PArraySortSuite_MergeFourth);
    CUNIT_CI_TEST (PArraySortSuite_MergeFifth);
    CUNIT_CI_TEST (PArraySortSuite_MergeSixth);

    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeFirst);
    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeSecond);
    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeThird);
    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeFourth);
    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeFifth);
    CUNIT_CI_TEST (PArraySortSuite_InplaceMergeSixth);

    CUNIT_CI_TEST (PArraySortSuite_HeapFirst);
    CUNIT_CI_TEST (PArraySortSuite_HeapSecond);
    CUNIT_CI_TEST (PArraySortSuite_HeapThird);
    CUNIT_CI_TEST (PArraySortSuite_HeapFourth);
    CUNIT_CI_TEST (PArraySortSuite_HeapFifth);
    CUNIT_CI_TEST (PArraySortSuite_HeapSixth);

    CUNIT_CI_TEST (PArraySortSuite_QuickFirst);
    CUNIT_CI_TEST (PArraySortSuite_QuickSecond);
    CUNIT_CI_TEST (PArraySortSuite_QuickThird);
    CUNIT_CI_TEST (PArraySortSuite_QuickFourth);
    CUNIT_CI_TEST (PArraySortSuite_QuickFifth);
    CUNIT_CI_TEST (PArraySortSuite_QuickSixth);

    CUNIT_CI_TEST (PArraySortSuite_IntroFirst);
    CUNIT_CI_TEST (PArraySortSuite_IntroSecond);
    CUNIT_CI_TEST (PArraySortSuite_IntroThird);
    CUNIT_CI_TEST (PArraySortSuite_IntroFourth);
    CUNIT_CI_TEST (PArraySortSuite_IntroFifth);
    CUNIT_CI_TEST (PArraySortSuite_IntroSixth);
}
