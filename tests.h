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

void generateTest(vector <unsigned long long> &arr, int type)
{
    arr.clear();
    unsigned int arr_size = rnd.next(1000u, 50000u);
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
              for (unsigned int i = 0u; i < arr_size; i++)
                  s.insert(getRand());
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
                  arr.push_back(*it);
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
              for (unsigned int i = 0; i < arr_size; i++)
                  s.insert(getRand());
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
              {
                  arr.push_back(*it);
                  arr.push_back(*it);
              }
              break;
    }
    case 6:
    {
              for (unsigned long long i = 0; i < arr_size; i++)
                  s.insert(getRand());
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
                  arr.push_back(*it);
              arr.push_back(arr[0]);
              break;
    }
    case 7:
    {
              for (unsigned long long i = 0; i < arr_size; i++)
                  s.insert(getRand());
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
                  arr.push_back(*it);
              arr.push_back(arr[0]);
              arr.push_back(arr[1]);
              break;
    }
    case 8:
    {
              arr_size = rnd.next(10u, 1000u);
              for (unsigned long long i = 0; i < arr_size; i++)
                  s.insert(getRand());
              for (unsigned long long i = 0; i < arr_size; i++)
                  for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
                      arr.push_back(*it);
              break;
    }
    case 9:
    {
              for (unsigned long long i = 0; i < arr_size; i++)
                  s.insert(getRand() + LLONG_MAX);
              for (set<unsigned long long>::iterator it = s.begin(); it != s.end(); it++)
                  arr.push_back(*it);
              break;
    }
    default:
        false;
    }
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
            vector< unsigned long long > arr;
            generateTest(arr, type);
            PerfectHashSet PHS;
            try
            {
                PHS.init(arr);
            }
            catch (int e)
            {
                if (isEqualInTest(type))
                    cout << "+";
                else
                    cout << "-";
                continue;
            }
            if (!isEqualInTest(type))
                cout << "+";
            else
                cout << "-";
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
        {
            cout << "-";
        }
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
                cout << "-";
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
            PHS.insert(*it);
            if (PHS.has(*it))
            {
                PHS.erase(*it);
                if (!PHS.has(*it))
                {
                    cout << "+";
                }
                else
                {
                    cout << "-";
                }
            }
            else
            {
                cout << "-";
            }
        }
        else
        {
            cout << "-";
        }
    }
    cout << endl;
    for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
    {
        PHS.insert(*it);
        PHS.insert(*it);
    }
    if (PHS.size() == keys.size())
    {
        cout << "+";
    }
    else
    {
        cout << "-";
    }
    PHS.erase(arr[0]);
    PHS.erase(arr[0]);
    if (PHS.size() == keys.size() - 1)
    {
        cout << "+";
    }
    else
    {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < m; i++)
    {
        for (set<unsigned long long>::iterator it = keys.begin(); it != keys.end(); it++)
        {
            PHS.erase(*it);
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
            cout << "-";
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
