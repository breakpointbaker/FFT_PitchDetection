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

#endif