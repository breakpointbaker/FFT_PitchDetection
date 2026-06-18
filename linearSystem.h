#ifndef LINEARSYSTEMS_H_
#define LINEARSYSTEMS_H_
#include <array>

template <typename T>
class linearSystem3X3
{
public:
    template <typename U, std::size_t Row, std::size_t Col>
    using Array2D = std::array<std::array<U, Col>, Row>;

private:
    Array2D<T, 3, 3> m_A{0};
    Array2D<T, 3, 1> m_x{0};
    Array2D<T, 3, 1> m_B{0};

    Array2D<T, 3, 3> replaceCol(Array2D<T, 3, 3> A, const Array2D<T, 3, 1> &d, int col)
    {
        A[0][col] = d[0][0];
        A[1][col] = d[1][0];
        A[2][col] = d[2][0];
        return A;
    }

public:
    linearSystem3X3(Array2D<T, 3, 3> A, Array2D<T, 3, 1> B) : m_A{A}, m_B{B} {}
    linearSystem3X3(void) = default;

    void addEquation(T a1, T a2, T a3, T b1, int row);

    Array2D<T, 3, 3> getA(void) { return m_A; }
    Array2D<T, 3, 1> getx(void) { return m_x; }
    Array2D<T, 3, 1> getB(void) { return m_B; }

    T det3X3(const Array2D<T, 3, 3> &A);
    T det3X3(void);
    void solveCramers(void);
};

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
    double D = det3X3(m_A);
    if (D == 0)
        return; // not unique
    m_x[0][0] = det3X3(replaceCol(m_A, m_B, 0)) / D;
    m_x[1][0] = det3X3(replaceCol(m_A, m_B, 1)) / D;
    m_x[2][0] = det3X3(replaceCol(m_A, m_B, 2)) / D;
}

#endif