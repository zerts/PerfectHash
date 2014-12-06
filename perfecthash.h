#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <climits>
#include "testlib.h"

const unsigned long long _SUM_SQR_SIZE = 3ULL;

unsigned long long mysqr(const unsigned long long &x)
{
    return x * x;
}

class IHashSet
{
public:
    virtual bool isPossibleKey(unsigned long long) = 0;
    virtual bool has(unsigned long long) = 0;
    virtual void insert(unsigned long long) = 0;
    virtual void erase(unsigned long long) = 0;
    virtual int size() const = 0;
    virtual void init(const std::vector <unsigned long long> &) = 0;
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
    void setCoefficients(unsigned long long first, unsigned long long second)
    {
        firstCoefficient = first;
        secondCoefficient = second;
    }
    void setSetSize(unsigned int size)
    {
        setSize = size;
    }
    unsigned long long operator()(unsigned long long curr)
    {
        return (((((firstCoefficient >> 32LLU) << 32LLU) * curr) % PRIME + ((firstCoefficient & UINT_MAX) * curr) % PRIME + secondCoefficient) % PRIME) % setSize;
    }
};

class FindEqualElements : public  std::exception
{
    std::string message;
public:
    explicit FindEqualElements(unsigned long long element)
    {
        message = "There are two or more " + std::to_string(element) + " elements\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

class FindImpossibleElement : public  std::exception
{
    std::string message;
public:
    explicit FindImpossibleElement(unsigned long long element)
    {
        message = "There are no " + std::to_string(element) + " element in basic array\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

class PerfectHashSetLevel2 : public IHashSet
{
    Hash hash;
    std::vector <unsigned long long> body, hashBody;
    std::vector<int> used;
    int _size;
    void stupidHasEqualKeys()
    {
        if (body.size() > 1)
        {
            for (int i = 0; i + 1 < body.size(); i++)
            {
                for (int j = i + 1; j < body.size(); j++)
                {
                    if (body[i] == body[j])
                    {
                        throw FindEqualElements(body[i]);
                    }
                }
            }
        }
    }
public:
    PerfectHashSetLevel2()
    {
        body.clear();
        hashBody.clear();
        used.clear();
    }
    const unsigned long long & operator[](const unsigned long long &index)
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
    void init(const std::vector<unsigned long long> &arr)
    {
        _size = 0;
        hash.setSetSize(mysqr(arr.size()));
        hash.generateCoefficients();
        while (hasCollide())
            hash.generateCoefficients();
        hashBody.resize(mysqr(arr.size()));
        used.assign(mysqr(arr.size()), 0);
        for (int i = 0; i < arr.size(); i++)
        {
            hashBody[hash(arr[i])] = arr[i];
        }
    }
    void createHashSet()
    {
        stupidHasEqualKeys();
        init(body);
    }
    bool isPossibleKey(unsigned long long curr)
    {
        return (!hashBody.empty() && hashBody[hash(curr)] == curr);
    }
    bool has(unsigned long long curr)
    {
        if (isPossibleKey(curr))
            return (used[hash(curr)] == 1);
    }
    void insert(unsigned long long key)
    {
        used[hash(key)] = 1;
        _size++;
    }
    void erase(unsigned long long key)
    {
        used[hash(key)] = 0;
        _size--;
    }
    int tableSize()
    {
        return body.size();
    }
    int size() const
    {
        return _size;
    }
    void out()
    {
        for (std::vector <unsigned long long>::iterator it = body.begin(); it != body.end(); it++)
            std::cout << *it << " ";
    }
};

class PerfectHashSet : public IHashSet
{
private:
    Hash hash;
    unsigned long long numberOfElements, a, b;
    int _size;
    std::vector < PerfectHashSetLevel2 > firstLevel;
    unsigned long long firstA, firstB;
    bool goodFirstLevel()
    {
        if (firstLevel.empty())
            return false;
        unsigned long long sum = 0ULL;
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
            sum += mysqr(it->tableSize());
        return sum <= (8 * numberOfElements);
    }
    bool hasEqualKeys()
    {
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        {
            if (it->tableSize() == 3)
            {
                if ((*it)[0] == (*it)[1])
                    throw FindEqualElements((*it)[0]);
                if ((*it)[0] == (*it)[2])
                    throw FindEqualElements((*it)[0]);
                if ((*it)[1] == (*it)[2])
                    throw FindEqualElements((*it)[1]);
            }
            else if (it->tableSize() > 0llu)
            {
                for (int i = 0; i + 1 < it->tableSize(); i++)
                {
                    if ((*it)[i] == (*it)[i + 1])
                    {
                        throw FindEqualElements((*it)[i]);
                    }
                }
            }
        }
    }
    void out()
    {
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        {
            it->out();
            std::cout << std::endl;
        }
    }
    bool goodInsertErase(unsigned long long key, bool isHas)
    {
        if (!isPossibleKey(key))
            throw FindImpossibleElement(key);
        return has(key) == isHas;
    }
public:
    void init(const std::vector <unsigned long long> &arr)
    {
        _size = 0;
        if (arr.empty())
        {
            firstLevel.clear();
            return;
        }
        numberOfElements = arr.size();
        hash.setSetSize(numberOfElements);
        while (!goodFirstLevel())
        {
            firstLevel.clear();
            firstLevel.resize(numberOfElements);
            hash.generateCoefficients();
            for (int i = 0; i < arr.size(); i++)
                firstLevel[hash(arr[i])].push(arr[i]);
            hasEqualKeys();
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
        if (goodInsertErase(curr, false))
        {
            firstLevel[hash(curr)].insert(curr);
            _size++;
        }
    }
    void erase(unsigned long long curr)
    {
        if (goodInsertErase(curr, true))
        {
            firstLevel[hash(curr)].erase(curr);
            _size--;
        }
    }
    int size() const
    {
        return _size;
    }
    void write()
    {
        out();
    }
};
