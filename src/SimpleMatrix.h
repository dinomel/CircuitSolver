//
//  SimpleMatrix.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 3. 7. 2025..
//

#pragma once
#include <vector>
#include <complex>
#include <stdexcept>
#include <iostream>

class SimpleMatrix
{
private:
    size_t rows_, cols_;
    std::vector<std::complex<double>> data;

public:
    SimpleMatrix() : rows_(0), cols_(0) {}

    SimpleMatrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data(rows * cols, std::complex<double>{0, 0}) {}

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    void resize(size_t rows, size_t cols)
    {
        rows_ = rows;
        cols_ = cols;
        data.assign(rows * cols, std::complex<double>{0, 0});
    }

    void reset()
    {
        std::fill(data.begin(), data.end(), std::complex<double>{0, 0});
    }

    std::complex<double> &operator()(size_t row, size_t col)
    {
        if (row >= rows_ || col >= cols_)
            throw std::out_of_range("Index out of bounds");
        return data[row * cols_ + col];
    }

    const std::complex<double> &operator()(size_t row, size_t col) const
    {
        if (row >= rows_ || col >= cols_)
            throw std::out_of_range("Index out of bounds");
        return data[row * cols_ + col];
    }

    SimpleMatrix transpose() const
    {
        SimpleMatrix result(cols_, rows_);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t j = 0; j < cols_; ++j)
                result(j, i) = (*this)(i, j);
        return result;
    }

    // Addition
    SimpleMatrix operator+(const SimpleMatrix &rhs) const
    {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("Dimension mismatch in matrix addition");
        SimpleMatrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i)
            result.data[i] = data[i] + rhs.data[i];
        return result;
    }

    // Subtraction
    SimpleMatrix operator-(const SimpleMatrix &rhs) const
    {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::runtime_error("Dimension mismatch in matrix subtraction");
        SimpleMatrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i)
            result.data[i] = data[i] - rhs.data[i];
        return result;
    }

    // Multiplication
    SimpleMatrix operator*(const SimpleMatrix &rhs) const
    {
        if (cols_ != rhs.rows_)
            throw std::runtime_error("Dimension mismatch in matrix multiplication");
        SimpleMatrix result(rows_, rhs.cols_);
        for (size_t i = 0; i < rows_; ++i)
        {
            for (size_t j = 0; j < rhs.cols_; ++j)
            {
                std::complex<double> sum{0, 0};
                for (size_t k = 0; k < cols_; ++k)
                {
                    sum += (*this)(i, k) * rhs(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }
    // Scalar multiplication: matrix * scalar
    SimpleMatrix operator*(const std::complex<double> &scalar) const
    {
        SimpleMatrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i)
        {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    // Scalar multiplication: scalar * matrix (friend function)
    friend SimpleMatrix operator*(const std::complex<double> &scalar, const SimpleMatrix &matrix)
    {
        return matrix * scalar; // reuse member operator*
    }

    // Matrix inversion (Gaussian elimination) - square matrices only
    SimpleMatrix inverse() const
    {
        if (rows_ != cols_)
            throw std::runtime_error("Inverse requires a square matrix");
        size_t n = rows_;

        SimpleMatrix A(*this);
        SimpleMatrix inv(n, n);
        for (size_t i = 0; i < n; ++i)
            inv(i, i) = std::complex<double>{1, 0};

        for (size_t i = 0; i < n; ++i)
        {
            // Pivot
            size_t pivot = i;
            double maxVal = std::abs(A(i, i));
            for (size_t r = i + 1; r < n; ++r)
            {
                if (std::abs(A(r, i)) > maxVal)
                {
                    maxVal = std::abs(A(r, i));
                    pivot = r;
                }
            }
            if (maxVal == 0)
            {
                // Singular matrix: return NaN matrix
                SimpleMatrix nanMat(n, n);
                std::complex<double> nanVal(std::numeric_limits<double>::quiet_NaN(),
                                            std::numeric_limits<double>::quiet_NaN());
                for (size_t i = 0; i < n * n; ++i)
                    nanMat.data[i] = nanVal;
                return nanMat;
            }

            if (pivot != i)
            {
                for (size_t c = 0; c < n; ++c)
                {
                    std::swap(A(i, c), A(pivot, c));
                    std::swap(inv(i, c), inv(pivot, c));
                }
            }

            std::complex<double> pivotVal = A(i, i);
            for (size_t c = 0; c < n; ++c)
            {
                A(i, c) /= pivotVal;
                inv(i, c) /= pivotVal;
            }

            for (size_t r = 0; r < n; ++r)
            {
                if (r == i)
                    continue;
                std::complex<double> factor = A(r, i);
                for (size_t c = 0; c < n; ++c)
                {
                    A(r, c) -= factor * A(i, c);
                    inv(r, c) -= factor * inv(i, c);
                }
            }
        }
        return inv;
    }
};
