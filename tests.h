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
    return rnd.next(0ull, (unsigned long long)(LLONG_MAX) - 2ull);
}

bool isEqualInTest(int type)
{
    switch (type)
    {
    case 1:
        return false;
    case 2:
        return false;
    case 3:
        return true;
    case 4:
        return true;
    case 5:
        return true;
    case 6:
        return true;
    case 7:
        return true;
    case 8:
        return true;
    case 9:
        return false;
    default:
        return false;
    }
}

void generateSet(set<unsigned long long> &s, unsigned int size)
{
    for (unsigned int i = 0u; i < size; i++)
        s.insert(getRand());
}

void setToArray(set<unsigned long long> &s, vector<unsigned long long> &arr)
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
    case 1:
    {
              arr_size = rnd.next(0ULL, 15ULL);
              for (unsigned int i = 0u; i < arr_size; i++)
                  arr.push_back((unsigned long long) i);
              break;
    }
    case 2:
    {
              generateSet(s, arr_size);
              setToArray(s, arr);
              break;
    }
    case 3:
    {
              unsigned long long k = getRand();
              for (unsigned long long i = 0; i < arr_size; i++)
                  arr.push_back(k);
              break;
    }
    case 4:
    {
              unsigned long long k0 = getRand() / 3, k1 = k0 + getRand() / 3;
              for (unsigned int i = 0; i < arr_size; i++)
              {
                  if (i % 2)
                      arr.push_back(k0);
                  else
                      arr.push_back(k1);
              }
              break;
    }
    case 5:
    {
              generateSet(s, arr_size);
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
              {
                  arr.push_back(*it);
                  arr.push_back(*it);
              }
              break;
    }
    case 6:
    {
              generateSet(s, arr_size);
              setToArray(s, arr);
              arr.push_back(arr[0]);
              break;
    }
    case 7:
    {
              generateSet(s, arr_size);
              setToArray(s, arr);
              arr.push_back(arr[0]);
              arr.push_back(arr[1]);
              break;
    }
    case 8:
    {
              arr_size = rnd.next(10u, 1000u);
              generateSet(s, arr_size);
              for (unsigned long long i = 0; i < arr_size; i++)
                  setToArray(s, arr);
              break;
    }
    case 9:
    {
              for (unsigned long long i = 0; i < arr_size; i++)
                  s.insert(getRand() + LLONG_MAX);
              setToArray(s, arr);
              break;
    }
    default:
        break;
    }
    return arr;
}

void testingInitOnCurrentArray(vector <unsigned long long> &arr, int type)
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
            cout << "+";
        else
        {
            cout << "-NO";
        }
        return;
    }
    if (!isEqualInTest(type))
        cout << "+";
    else
    {
        cout << "-NO";
    }
}

bool needAllPermutations(int type, unsigned int size)
{
    return (size < 13) && (type == 1 || type == 4 || type == 5 || type == 6 || type == 7);
}

int fact(int n)
{
    int r = 1;
    for (int i = 2; i + 5 <= n; i++)
        r *= i;
    return r;
}

void testingInit(int n)
{
    //1 - Без одинаковых ключей. Маленькие размеры – все перестановки.
    //2 - Без одинаковых ключей. Случайный.
    //3 - k0, k0, k0, k0,..., k0
    //4 - k0, k1, k0, k1, …, k0, k1. Так + random_shuffle + все перестановки малых
    //5 - k0, k0, k1, k1, k2, k2,...,kn/2, kn/2. Те же.
    //6 - k0, k1, k2, …, kn-2, k0. Те же.
    //7 - k0, k1, k2, …, kn-3, k0, k1. Те же
    //8 - (k0, k1, k2, …, k sqrt(n)) * sqrt(n). Те же + random_shuffle кусков.
    //9 - Большие значения ключей.
    for (int type = 1; type < 10; type++)
    {
        for (int i = 0; i < n; i++)
        {
            vector< unsigned long long > arr = generateTest(type);
            testingInitOnCurrentArray(arr, type);
            if (needAllPermutations(type, arr.size()))
            {
                int numberOfPermutations = fact(arr.size());
                for (int i = 0; i < numberOfPermutations; i++)
                {
                    next_permutation(arr.begin(), arr.end());
                    testingInitOnCurrentArray(arr, type);
                }
            }
            else
            {
                testingInitOnCurrentArray(arr, type);
            }
        }
        cout << endl;
    }
}

void testingIsPossibleKey(int n, int m)
{
    set <unsigned long long> keys;
    while (keys.size() != n)
    {
        unsigned long long newKey = getRand();
        keys.insert(newKey);
    }
    vector <unsigned long long> arr;
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        arr.push_back(*it);
    }
    PerfectHashSet PHS;
    PHS.init(arr);
    cout << "set is readyt\n";
    for (vector<unsigned long long>::iterator it = arr.begin(); it != arr.end(); it++)
    {
        if (PHS.isPossibleKey(*it))
            cout << "+";
        else
            cout << "-NO";
    }
    cout << endl;
    for (int i = 0; i < m; i++)
    {
        unsigned long long newKey = getRand();
        if (keys.find(newKey) == keys.end())
        {
            if (!PHS.isPossibleKey(newKey))
                cout << "+";
            else
                cout << "-NO";
        }
    }
}

void checkElementInArray(unsigned long long elem, set <unsigned long long> &keys)
{
    if (keys.find(elem) == keys.end())
        cout << "+";
    else
        cout << "-NO";
}

void tryInsertErase(int type, unsigned long long elem, PerfectHashSet &PHS, set <unsigned long long> &keys)
{
    if (type == 1)
    {
        try
        {
            PHS.insert(elem);
        }
        catch (exception &)
        {
            checkElementInArray(elem, keys);
        }
    }
    if (type == 2)
    {
        try
        {
            PHS.erase(elem);
        }
        catch (exception &)
        {
            checkElementInArray(elem, keys);
        }
    }
}

void testingHasInsertEraseSize(int n, int m)
{
    set <unsigned long long> keys;
    while (keys.size() != n)
    {
        unsigned long long newKey = getRand();
        keys.insert(newKey);
    }
    vector <unsigned long long> arr;
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        arr.push_back(*it);
    }
    PerfectHashSet PHS;
    PHS.init(arr);
    cout << "set is readyt\n";

    for (vector<unsigned long long>::iterator it = arr.begin(); it != arr.end(); it++)
    {
        if (!PHS.has(*it))
        {
            tryInsertErase(1, *it, PHS, keys);
            if (PHS.has(*it))
            {
                tryInsertErase(2, *it, PHS, keys);
                if (!PHS.has(*it))
                    cout << "+";
                else
                    cout << "-NO";
            }
            else
            {
                cout << "-NO";
            }
        }
        else
        {
            cout << "-NO";
        }
    }
    cout << endl;
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        tryInsertErase(1, *it, PHS, keys);
        tryInsertErase(1, *it, PHS, keys);
    }
    if (PHS.size() == keys.size())
        cout << "+";
    else
        cout << "-NO";
    PHS.erase(arr[0]);
    PHS.erase(arr[0]);
    if (PHS.size() == keys.size() - 1)
        cout << "+";
    else
        cout << "-NO";
    cout << endl;

    for (int i = 0; i < m; i++)
    {
        for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
        {
            tryInsertErase(2, *it, PHS, keys);
        }
        int type = rnd.next(0, 1), key = rnd.next(0u, arr.size() - 1u);
        set <unsigned long long> s;
        if (type == 0)
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
            cout << "+";
        else
            cout << "-NO";
    }
}

void testingInitOnCurrentSize(int n)
{
    clock_t t;
    set <unsigned long long> keys;
    while (keys.size() != n)
    {
        unsigned long long newKey = getRand();
        keys.insert(newKey);
    }
    vector <unsigned long long> arr;
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        arr.push_back(*it);
    }
    PerfectHashSet PHS;
    cout << "Array is ready\n";
    t = clock();
    PHS.init(arr);
    cout << clock() - t << endl;
}
