#include <array>
#include <iostream>
#include "linearSystem.h"

template <typename T>
void linearSystem3X3<T>::addEquation(T a1, T a2, T a3, T b1, int row)
{
    m_A[row][0] = a1;
    m_A[row][1] = a2;
    m_A[row][2] = a3;
    m_B[row][0] = b1;
}

template <typename T>
T linearSystem3X3<T>::det3X3(const Array2D<T, 3, 3> &A)
{
    T det{0};
    det = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) -
          A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0]) +
          A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
    return det;
}
template <typename T>
T linearSystem3X3<T>::det3X3(void)
{
    T det{0};
    det = m_A[0][0] * (m_A[1][1] * m_A[2][2] - m_A[1][2] * m_A[2][1]) -
          m_A[0][1] * (m_A[1][0] * m_A[2][2] - m_A[1][2] * m_A[2][0]) +
          m_A[0][2] * (m_A[1][0] * m_A[2][1] - m_A[1][1] * m_A[2][0]);
    return det;
}

template <typename T>
void linearSystem3X3<T>::solveCramers(void)
{
    int D = det3X3(m_A);
    if (D == 0)
        return; // not unique
    m_x[0][0] = det3X3(replaceCol(m_A, m_B, 0)) / D;
    m_x[1][0] = det3X3(replaceCol(m_A, m_B, 1)) / D;
    m_x[2][0] = det3X3(replaceCol(m_A, m_B, 2)) / D;
}
