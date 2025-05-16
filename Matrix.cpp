#include <cstddef>
#include <memory>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace std;

template<typename T>
class Matrix {
public:

    Matrix(int rows, int cols)
        : rows_(rows), cols_(cols), data_(make_unique<T[]>(rows * cols))
    {
        for (int i = 0; i < rows_ * cols_; ++i)
            data_[i] = T();
    }

    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_), data_(make_unique<T[]>(other.rows_ * other.cols_))
    {
        copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
    }

    Matrix(Matrix&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), data_(move(other.data_))
    {
        other.rows_ = other.cols_ = 0;
    }

    Matrix& operator=(const Matrix& other) {

        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = make_unique<T[]>(rows_ * cols_);
            copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
        }

        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {

        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = move(other.data_);
            other.rows_ = other.cols_ = 0;
        }

        return *this;
    }

    T& operator()(int i, int j) {

        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            cerr << "Error: Index out of bounds\n";
            return data_[0];
        }

        return data_[i * cols_ + j];
    }

    const T& operator()(int i, int j) const {

        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            cerr << "Error: Index out of bounds\n";
            return data_[0];
        }

        return data_[i * cols_ + j];
    }

    int rows() const noexcept { 
        return rows_; 
    }

    int cols() const noexcept { 
        return cols_; 
    }

    friend ostream& operator<<(ostream& os, const Matrix& m) {

        for (int i = 0; i < m.rows_; ++i) {
            for (int j = 0; j < m.cols_; ++j)
                os << m(i, j) << (j + 1 < m.cols_ ? ' ' : '\n');
        }

        return os;
    }

    friend Matrix operator+(const Matrix& a, const Matrix& b) {

        if (a.rows_ != b.rows_ || a.cols_ != b.cols_) {
            cerr << "Error: Incompatible dimensions for addition\n";
            return Matrix(0, 0);
        }

        Matrix result(a.rows_, a.cols_);

        for (int i = 0; i < a.rows_ * a.cols_; ++i)
            result.data_[i] = a.data_[i] + b.data_[i];

        return result;
    }

    friend Matrix operator-(const Matrix& a, const Matrix& b) {

        if (a.rows_ != b.rows_ || a.cols_ != b.cols_) {
            cerr << "Error: Incompatible dimensions for subtraction\n";
            return Matrix(0, 0);
        }

        Matrix result(a.rows_, a.cols_);

        for (int i = 0; i < a.rows_ * a.cols_; ++i)
            result.data_[i] = a.data_[i] - b.data_[i];

        return result;
    }

    Matrix operator*(const Matrix& b) const {

        if (cols_ != b.rows_) {
            cerr << "Error: Incompatible dimensions for multiplication\n";
            return Matrix(0, 0);
        }

        Matrix result(rows_, b.cols_);

        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < b.cols_; ++j) {
                T sum = T();
                for (int k = 0; k < cols_; ++k)
                    sum += (*this)(i, k) * b(k, j);
                result(i, j) = sum;
            }
        }

        return result;
    }

private:
    int rows_, cols_;
    unique_ptr<T[]> data_;
};

template<typename T>
Matrix<T> transpose(const Matrix<T>& m) {

    Matrix<T> result(m.cols(), m.rows());

    for (int i = 0; i < m.rows(); ++i)
        for (int j = 0; j < m.cols(); ++j)
            result(j, i) = m(i, j);

    return result;
}

template<typename T>
T trace(const Matrix<T>& m) {

    if (m.rows() != m.cols()) {
        cerr << "Error: Trace only defined for square matrices\n";
        return T();
    }

    T sum = T();
    for (int i = 0; i < m.rows(); ++i)
        sum += m(i, i);

    return sum;
}

int main() {

    int row1, col1, row2, col2;

    cout << "Enter rows and columns for matrix1: ";
    cin >> row1 >> col1;

    Matrix<int> matrix1(row1, col1);
    cout << "Enter elements of matrix1 (" << row1 << "x" << col1 << ") below:\n";
    for (int i = 0; i < row1; ++i)
        for (int j = 0; j < col1; ++j)
            cin >> matrix1(i, j);

    cout << "\nEnter rows and columns for matrix2: ";
    cin >> row2 >> col2;

    Matrix<int> matrix2(row2, col2);
    cout << "Enter elements of matrix2 (" << row2 << "x" << col2 << ") below:\n";
    for (int i = 0; i < row2; ++i)
        for (int j = 0; j < col2; ++j)
            cin >> matrix2(i, j);

    cout << "\n=== Results ===\n";

    cout << "\n1) Sum (matrix1 + matrix2):\n" << (matrix1 + matrix2);
    cout << "\n2) Difference (matrix1 - matrix2):\n" << (matrix1 - matrix2);
    cout << "\n3) Product (matrix1 * matrix2):\n" << (matrix1 * matrix2);

    cout << "\n4) Transpose of matrix1:\n" << transpose(matrix1);
    cout << "   Trace of matrix1: " << trace(matrix1) << "\n";

    cout << "\n5) Transpose of matrix2:\n" << transpose(matrix2);
    cout << "   Trace of matrix2: " << trace(matrix2) << "\n";

    return 0;
}
