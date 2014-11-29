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

int main()
{
    int n, m;

    cout << "Write number of tests: ";
    std::cin >> n;
    testingInit(n);

    cout << "\nWrite size of array and number of tests: ";
    cin >> n >> m;
    testingIsPossibleKey(n, m);

    cout << "\nWrite size of array and number of tests: ";
    cin >> n >> m;
    testingHasInsertEraseSize(n, m);

    cout << "\n";
    system("pause");
	return 0;
}
