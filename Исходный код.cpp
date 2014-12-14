#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <set>
#include <climits>
#include <string>
#include "tests.h"

void testing(unsigned int n, unsigned int m)
{
    testingInit(m);
    testingIsPossibleKey(n, m);
    testingHasInsertEraseSize(n, m);
    testingInitOnCurrentSize(n);
}

int main()
{
    unsigned int n, m;
    std::cin >> n >> m;
    testing(n, m);
    std::cout << "\nFinish";
    system("pause");
	return 0;
}
