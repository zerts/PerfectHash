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
    virtual bool isPossibleKey(unsigned long long) const = 0;
    virtual bool has(unsigned long long) const = 0;
    virtual void insert(unsigned long long) = 0;
    virtual void erase(unsigned long long) = 0;
    virtual size_t size() const = 0;
    virtual void init(const std::vector <unsigned long long> &) = 0;
    virtual ~IHashSet(){};
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
    unsigned long long operator()(unsigned long long curr) const
    {
        return (((((firstCoefficient >> 32LLU) << 32LLU) * curr) % PRIME + ((firstCoefficient & UINT_MAX) * curr) % PRIME + secondCoefficient) % PRIME) % setSize;
    }
};

class EqualElements : public  std::exception
{
    std::string message;
public:
    explicit EqualElements(unsigned long long element)
    {
        message = "There are two or more " + std::to_string(element) + " elements\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

class ImpossibleElement : public  std::exception
{
    std::string message;
public:
    explicit ImpossibleElement(unsigned long long element)
    {
        message = "There are no " + std::to_string(element) + " element in basic array\n";
    }
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

void checkEqualElements(unsigned long long a, unsigned long long b)
{
    if (a == b)
        throw EqualElements(a);
}

class PerfectHashSetLevel2 : public IHashSet
{
    Hash hash;
    std::vector <unsigned long long> body, hashBody;
    std::vector<bool> used, hashBodyUsed;
    size_t _size;
    void stupidHasEqualKeys()
    {
        if (body.size() > 1)
        {
            for (size_t i = 0; i + 1 < body.size(); ++i)
            {
                for (size_t j = i + 1; j < body.size(); ++j)
                {
                    checkEqualElements(body[i], body[j]);
                }
            }
        }
    }
    bool goodInsertErase(unsigned long long key, bool isHas)
    {
        if (has(key) == isHas)
        {
            used[hash(key)] = (!isHas);
            return true;
        }
        return false;
    }
public:
    PerfectHashSetLevel2()
    {
        body.clear();
        hashBody.clear();
        used.clear();
    }
    const unsigned long long & operator[](const unsigned long long &index) const
    {
        return body[index];
    }
    void push(const unsigned long long &key)
    {
        body.push_back(key);
    }
    bool hasCollide()
    {
        used.assign(mysqr(body.size()), false);
        for (std::vector <unsigned long long>::iterator it = body.begin(); it != body.end(); it++)
        {
            unsigned long long curr = hash(*it);
            if (used[curr])
                return true;
            used[curr] = true;
        }
        return false;
    }
    void init(const std::vector<unsigned long long> &arr)
    {
        _size = 0;
        hash.setSetSize(mysqr(arr.size()));
        do
        hash.generateCoefficients();
        while (hasCollide());
        hashBody.resize(mysqr(arr.size()));
        hashBodyUsed.resize(mysqr(arr.size()), false);
        used.assign(mysqr(arr.size()), false);
        for (size_t i = 0; i < arr.size(); i++)
        {
            hashBody[hash(arr[i])] = arr[i];
            hashBodyUsed[hash(arr[i])] = true;
        }
    }
    void createHashSet()
    {
        stupidHasEqualKeys();
        init(body);
    }
    bool isPossibleKey(unsigned long long curr) const
    {
        return (!hashBody.empty() && hashBodyUsed[hash(curr)] && hashBody[hash(curr)] == curr);
    }
    bool has(unsigned long long curr) const
    {
        if (isPossibleKey(curr))
            return (used[hash(curr)]);
        throw ImpossibleElement(curr);
    }
    void insert(unsigned long long key)
    {
        if (goodInsertErase(key, false))
            _size++;
    }
    void erase(unsigned long long key)
    {
        if (goodInsertErase(key, true))
            _size--;
    }
    size_t tableSize() const
    {
        return body.size();
    }
    size_t size() const
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
    size_t _size;
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
    bool hasEqualKeys() const
    {
        for (std::vector < PerfectHashSetLevel2 >::const_iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
        if (it->tableSize() > 1u)
        for (size_t i = 0; i < it->tableSize(); i++)
            checkEqualElements((*it)[i], (*it)[(i + 1) % it->tableSize()]);
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
    bool goodInsertErase(unsigned long long key, bool isHas)
    {
        if (!isPossibleKey(key))
            throw ImpossibleElement(key);
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
            for (size_t i = 0; i < arr.size(); i++)
                firstLevel[hash(arr[i])].push(arr[i]);
            hasEqualKeys();
        }
        for (std::vector < PerfectHashSetLevel2 >::iterator it = firstLevel.begin(); it != firstLevel.end(); it++)
            it->createHashSet();
    }
    bool isPossibleKey(unsigned long long curr) const
    {
        return firstLevel[hash(curr)].isPossibleKey(curr);
    }
    bool has(unsigned long long curr) const
    {
        if (isPossibleKey(curr))
            return firstLevel[hash(curr)].has(curr);
        throw ImpossibleElement(curr);
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
    size_t size() const
    {
        return _size;
    }
    void write()
    {
        out();
    }
};
