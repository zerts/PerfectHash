#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <climits>
#include "testlib.h"

const unsigned long long _SUM_SQR_SIZE = 8ULL;

unsigned long long mysqr(const unsigned long long &x)
{
    return x * x;
}

class HashSet
{
public:
    virtual bool isPossibleKey(unsigned long long) = 0;
    virtual bool has(unsigned long long) = 0;
    virtual void insert(unsigned long long) = 0;
    virtual void erase(unsigned long long) = 0;
    virtual unsigned long long size() const = 0;
};

class Hash
{
    const unsigned long long PRIME = 4294967311LLU;
    unsigned long long firstCoefficient, secondCoefficient;
    unsigned int setSize;
public:
    void generateCoefficients()
    {
        firstCoefficient = rnd.next(1LLU, PRIME - 1LLU);
        secondCoefficient = rnd.next(0LLU, PRIME - 1LLU);
    }
    void getCoefficients(unsigned long long first, unsigned long long second)
    {
        firstCoefficient = first;
        secondCoefficient = second;
    }
    void makeSize(unsigned int size)
    {
        setSize = size;
    }
    unsigned long long operator()(unsigned long long curr)
    {
        return (((((firstCoefficient >> 32LLU) << 32LLU) * curr) % PRIME + ((firstCoefficient & UINT_MAX) * curr) % PRIME + secondCoefficient) % PRIME) % setSize;
    }
};

class PerfectHashSetLevel2 : public HashSet
{
    Hash hash;
    std::vector <unsigned long long> body, hashBody, used;
public:
    PerfectHashSetLevel2()
    {
        body.clear();
        hashBody.clear();
        used.clear();
    }
    unsigned long long operator[](const unsigned long long &index)
    {
        return body[index];
    }
    void push(const unsigned long long &key)
    {
        body.push_back(key);
    }
    bool hasCollide()
    {
        used.assign(mysqr(body.size()), 0);
        for (std::vector <unsigned long long>::iterator it = body.begin(); it != body.end(); it++)
        {
            unsigned long long curr = hash(*it);
            used[curr]++;
            if (used[curr] > 1)
                return true;
        }
        return false;
    }
    void createHashSet()
    {
        hash.makeSize(mysqr(body.size()));
        hash.generateCoefficients();
        while (hasCollide())
            hash.generateCoefficients();
        hashBody.resize(mysqr(body.size()));
        used.assign(mysqr(body.size()), 0);
        for (std::vector <unsigned long long>::iterator it = body.begin(); it != body.end(); it++)
        {
            hashBody[hash(*it)] = *it;
        }
    }
    bool isPossibleKey(unsigned long long curr)
    {
        return (hashBody.size() != 0 && hashBody[hash(curr)] == curr);
    }
    bool has(unsigned long long curr)
    {
        return (used[hash(curr)] == 1);
    }
    void insert(unsigned long long key)
    {
        used[hash(key)] = 1;
    }
    void erase(unsigned long long key)
    {
        used[hash(key)] = 0;
    }
    unsigned long long size() const
    {
        return body.size();
    }
    void out()
    {
        for (std::vector <unsigned long long>::iterator it = body.begin(); it != body.end(); it++)
            std::cout << *it << " ";
    }
};

class PerfectHashSet : public HashSet
{
private:
    Hash hash;
    unsigned long long _size, numberOfElements, a, b;
    std::vector < PerfectHashSetLevel2 > firstLevel;
    unsigned long long firstA, firstB;
    bool goodFirstLevel()
    {
        if (firstLevel.size() == 0)
            return false;
        unsigned long long sum = 0ULL;
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
            sum += mysqr(it->size());
        return sum <= (8 * numberOfElements);
    }
    bool hasEqualKeys()
    {
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        {
            if (it->size() == 3)
            {
                if ((*it)[0] == (*it)[1] || (*it)[0] == (*it)[2] || (*it)[1] == (*it)[2])
                {
                    return true;
                }
            }
            else if (it->size() > 0llu)
            {
                for (unsigned long long i = 0llu; i < it->size() - 1llu; i++)
                {
                    if ((*it)[i] == (*it)[i + 1llu])
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool stupidHasEqualKeys()
    {
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        {
            if (it->size() > 1)
            {
                for (unsigned long long i = 0llu; i < it->size() - 1llu; i++)
                {
                    for (unsigned long long j = i + 1; j < it->size(); j++)
                    {
                        if ((*it)[i] == (*it)[j])
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    void out()
    {
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        {
            it->out();
            std::cout << std::endl;
        }
    }
public:
    void init(std::vector <unsigned long long> &arr)
    {
        if (arr.size() == 0)
            return;
        _size = 0;
        numberOfElements = arr.size();
        hash.makeSize(numberOfElements);
        while (!goodFirstLevel())
        {
            firstLevel.resize(numberOfElements);
            hash.generateCoefficients();
            for (std::vector <unsigned long long>::iterator it = arr.begin(); it != arr.end(); it++)
                firstLevel[hash(*it)].push(*it);
            if (hasEqualKeys())
            {
                //std::cout << "Array has equal keys!\n";
                throw 1;
                return;
            }
        }
        if (stupidHasEqualKeys())
        {
            //std::cout << "Array has equal keys!\n";
            throw 1;
            return;
        }

        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
            it->createHashSet();
    }
    bool isPossibleKey(unsigned long long curr)
    {
        return firstLevel[hash(curr)].isPossibleKey(curr);
    }
    bool has(unsigned long long curr)
    {
        if (isPossibleKey(curr))
            return firstLevel[hash(curr)].has(curr);
        return false;
    }
    void insert(unsigned long long curr)
    {
        if (isPossibleKey(curr) && !(has(curr)))
        {
            firstLevel[hash(curr)].insert(curr);
            _size++;
        }
    }
    void erase(unsigned long long curr)
    {
        if (isPossibleKey(curr) && has(curr))
        {
            firstLevel[hash(curr)].erase(curr);
            _size--;
        }
    }
    unsigned long long size() const
    {
        return _size;
    }
    void write()
    {
        out();
    }
};
