#include "linearSystem.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <array>
#include <iostream>

int main()
{
    // Test should get x0 = 2, x1 = -1, x2 = 3
    linearSystem3X3<double>::Array2D<double, 3, 3> A{{{3, -4, 8}, {4, 1, -2}, {-6, -13, 20}}};
    linearSystem3X3<double>::Array2D<double, 3, 1> B{{{34}, {1}, {61}}};

    linearSystem3X3<double> test(A, B);
    test.solveCramers();
    std::cout << test.getx()[0][0] << " " << test.getx()[1][0] << " " << test.getx()[2][0];

    // test creation
    linearSystem3X3<double> test2;
    test2.addEquation(1, 2, 3, 4, 0);
    std::cout << test2.getA()[0][0] << " " << test2.getA()[0][1] << " " << test2.getA()[0][2] << " " << test2.getB()[0][0];
    return 0;
}