#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <set>
#include <climits>
#include "perfecthash.h"

using namespace std;

unsigned long long getRand()
{
    return rnd.next(0ull, (unsigned long long)(INT_MAX) / 2);
}

const unsigned int NUMBER_OF_TESTS = 10;

enum EInitTestType {
    EITT_SMALL_TEST_NO_EQUAL = 1u,
    EITT_TEST_NO_EQUAL = 2u,
    EITT_ALL_EQUAL = 3u,
    EITT_TWO_DIFFERENT = 4u,
    EITT_PAIRS_ELEMENTS = 5u,
    EITT_LAST_EQUAL = 6u,
    EITT_TWO_LAST_EQUAL = 7u,
    EITT_SQRT_PARTS_OF_EQUAL = 8u,
    EITT_BIG_NUMBERS = 9u
};

bool isEqualInTest(unsigned int type)
{
    switch (type)
    {
    case EITT_SMALL_TEST_NO_EQUAL:
        return false;
    case EITT_TEST_NO_EQUAL:
        return false;
    case EITT_ALL_EQUAL:
        return true;
    case EITT_TWO_DIFFERENT:
        return true;
    case EITT_PAIRS_ELEMENTS:
        return true;
    case EITT_LAST_EQUAL:
        return true;
    case EITT_TWO_LAST_EQUAL:
        return true;
    case EITT_SQRT_PARTS_OF_EQUAL:
        return true;
    case EITT_BIG_NUMBERS:
        return false;
    default:
        return false;
    }
}

void generateSet(set<unsigned long long> &s, const unsigned int size)
{
    for (unsigned int i = 0u; i < size; i++)
        s.insert(getRand());
}

void setToArray(set<unsigned long long> const &s, vector<unsigned long long> &arr)
{
    for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
        arr.push_back(*it);
}

vector <unsigned long long> generateTest(int type)
{
    vector <unsigned long long> arr;
    unsigned int arr_size = rnd.next(10u, 50000u);
    set <unsigned long long> s;
    switch (type)
    {
    case EITT_SMALL_TEST_NO_EQUAL:
    {
        arr_size = rnd.next(0ULL, 15ULL);
        for (unsigned int i = 0u; i < arr_size; i++)
            arr.push_back((unsigned long long) i);
        break;
    }
    case EITT_TEST_NO_EQUAL:
    {
        generateSet(s, arr_size);
        setToArray(s, arr);
        break;
    }
    case EITT_ALL_EQUAL:
    {
        unsigned long long k = getRand();
        for (unsigned long long i = 0u; i < arr_size; i++)
            arr.push_back(k);
        break;
    }
    case EITT_TWO_DIFFERENT:
    {
        unsigned long long k0 = getRand() / 3, k1 = k0 + getRand() / 3;
        for (unsigned int i = 0u; i < arr_size; i++)
        {
            if (i % 2)
                arr.push_back(k0);
            else
                arr.push_back(k1);
        }
        break;
    }
    case EITT_PAIRS_ELEMENTS:
    {
        generateSet(s, arr_size);
        for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
        {
            arr.push_back(*it);
            arr.push_back(*it);
        }
        break;
    }
    case EITT_LAST_EQUAL:
    {
        generateSet(s, arr_size);
        setToArray(s, arr);
        arr.push_back(arr[0]);
        break;
    }
    case EITT_TWO_LAST_EQUAL:
    {
        generateSet(s, arr_size);
        setToArray(s, arr);
        arr.push_back(arr[0]);
        arr.push_back(arr[1]);
        break;
    }
    case EITT_SQRT_PARTS_OF_EQUAL:
    {
        arr_size = rnd.next(10u, 1000u);
        generateSet(s, arr_size);
        for (unsigned long long i = 0; i < arr_size; i++)
            setToArray(s, arr);
        break;
    }
    case EITT_BIG_NUMBERS:
    {
        for (unsigned long long i = 0; i < arr_size; i++)
            s.insert(getRand() + (unsigned long long)(INT_MAX / 2));
        setToArray(s, arr);
        break;
    }
    default:
        break;
    }
    return arr;
}

void testingInitOnCurrentArray(const vector <unsigned long long> &arr, int type)
{
    int n;
    PerfectHashSet PHS;
    try
    {
        PHS.init(arr);
    }
    catch (exception &)
    {
        if (isEqualInTest(type))
            cerr << "+";
        else
            cerr << "-NO";
        return;
    }
    if (!isEqualInTest(type))
        cerr << "+";
    else
        cerr << "-NO";
}

bool needAllPermutations(int type, unsigned int size)
{
    return (size < 13) && (type == EITT_SMALL_TEST_NO_EQUAL || type == EITT_TWO_DIFFERENT ||
        type == EITT_PAIRS_ELEMENTS || type == EITT_LAST_EQUAL || type == EITT_TWO_LAST_EQUAL);
}

void testingInit(unsigned int n)
{
    for (unsigned int type = 1u; type < NUMBER_OF_TESTS; type++)
    {
        for (unsigned int i = 0u; i < n; i++)
        {
            vector< unsigned long long > arr = generateTest(type);
            testingInitOnCurrentArray(arr, type);
            if (needAllPermutations(type, arr.size()))
            {
                do
                    testingInitOnCurrentArray(arr, type);
                while (next_permutation(arr.begin(), arr.end()));
            }
            else
            {
                testingInitOnCurrentArray(arr, type);
            }
        }
        cerr << endl;
    }
}

void testingIsPossibleKey(unsigned int n, unsigned int m)
{
    set <unsigned long long> keys;
    generateSet(keys, n);
    vector <unsigned long long> arr;
    setToArray(keys, arr);
    PerfectHashSet PHS;
    PHS.init(arr);
    cerr << "set is readyt\n";
    for (vector<unsigned long long>::iterator it = arr.begin(); it != arr.end(); it++)
    {
        if (PHS.isPossibleKey(*it))
            cerr << "+";
        else
            cerr << "-NO";
    }
    cerr << endl;
    for (unsigned int i = 0u; i < m; i++)
    {
        unsigned long long newKey = getRand();
        if (keys.find(newKey) == keys.end())
        {
            if (!PHS.isPossibleKey(newKey))
                cerr << "+";
            else
                cerr << "-NO";
        }
    }
}

void checkElementInArray(unsigned long long elem, const set <unsigned long long> &keys)
{
    if (keys.find(elem) == keys.end())
        cerr << "+";
    else
        cerr << "-NO";
}

void tryInsertErase(bool isInsert, unsigned long long elem, PerfectHashSet &PHS, set <unsigned long long> &keys)
{
    try
    {
        if (isInsert)
            PHS.insert(elem);
        else
            PHS.erase(elem);
    }
    catch (exception &)
    {
        checkElementInArray(elem, keys);
    }
}

void checkHas(unsigned long long curr, set <unsigned long long> &keys, PerfectHashSet &PHS)
{
    if (!PHS.has(curr))
    {
        tryInsertErase(true, curr, PHS, keys);
        if (PHS.has(curr))
        {
            tryInsertErase(false, curr, PHS, keys);
            if (!PHS.has(curr))
            {
                cerr << "+";
                return;
            }
        }
    }
    cerr << "-NO";
}

void testingEraseAfterInsert(set <unsigned long long> &keys, const vector <unsigned long long> &arr, PerfectHashSet &PHS)
{
    for (vector<unsigned long long>::const_iterator it = arr.cbegin(); it != arr.cend(); it++)
    {
        checkHas(*it, keys, PHS);
    }
    cerr << endl;
}

void testingDoubleErase(set <unsigned long long> &keys, const vector <unsigned long long> &arr, PerfectHashSet &PHS)
{
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        tryInsertErase(true, *it, PHS, keys);
        tryInsertErase(true, *it, PHS, keys);
    }
    if (PHS.size() == keys.size())
        cerr << "+";
    else
        cerr << "-NO";
    PHS.erase(arr[0]);
    PHS.erase(arr[0]);
    if (PHS.size() == keys.size() - 1)
        cerr << "+";
    else
        cerr << "-NO";
    cerr << endl;
}

void testingInsertEraseInRandomOrder(set <unsigned long long> &keys, const vector <unsigned long long> &arr, PerfectHashSet &PHS, unsigned int number_of_tests)
{
    for (unsigned int i = 0u; i < number_of_tests; i++)
    {
        for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
        {
            tryInsertErase(false, *it, PHS, keys);
        }
        int key = rnd.next(0u, arr.size() - 1u);
        set <unsigned long long> s;
        if (rnd.next(0, 1) == 0)
        {
            s.insert(arr[key]);
            PHS.insert(arr[key]);
        }
        else
        {
            s.erase(arr[key]);
            PHS.erase(arr[key]);
        }
        bool isWorkCorrect = true;
        if (s.size() != PHS.size())
            isWorkCorrect = false;
        for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
        {
            if (!PHS.has(*it))
                isWorkCorrect = false;
        }

        if (isWorkCorrect)
            cerr << "+";
        else
            cerr << "-NO";
    }
}

void testingHasInsertEraseSize(unsigned int n, unsigned int m)
{
    set <unsigned long long> keys;
    generateSet(keys, n);
    vector <unsigned long long> arr;
    setToArray(keys, arr);
    PerfectHashSet PHS;
    PHS.init(arr);
    cerr << "set is readyt\n";

    testingEraseAfterInsert(keys, arr, PHS);
    testingDoubleErase(keys, arr, PHS);
    testingInsertEraseInRandomOrder(keys, arr, PHS, m);
}

void testingInitOnCurrentSize(unsigned int n)
{
    clock_t t;
    set <unsigned long long> keys;
    generateSet(keys, n);
    vector <unsigned long long> arr;
    setToArray(keys, arr);
    PerfectHashSet PHS;
    cerr << "Array is ready\n";
    t = clock();
    PHS.init(arr);
    cerr << clock() - t << endl;
}
