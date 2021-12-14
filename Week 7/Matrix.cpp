// Copyright (C) John Doll

#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <cassert>
#include <vector>
#include "Matrix.h"

Matrix::Matrix(const size_t row, const size_t col, const Val initVal) :
    std::vector<std::vector<Val>>(row, std::vector<Val>(col, initVal)) {
}

// input stream operator overload
std::istream&
operator>>(std::istream& is, Matrix& matrix) {
    // create variables for row and col and read them in
    int rows, cols;
    is >> rows >> cols;

    // for each row in the matrix, add a vector (column)
    for (int i = 0; i < rows; i++) {
        matrix.push_back(std::vector<Val>());
        for (int j = 0; j < cols; j++) {
            // at each location, get the value from the istream and add it to
            // the matrix
            Val val;
            is >> val;
            matrix[i].push_back(val);
        }
    }
    return is;
}

// addition operator overload
Matrix
Matrix::operator+(const Matrix& rhs) const {
    // if dimensions are different, throw exception
    if (rhs.height() != this->height() || rhs.width() != this->width()) {
        throw std::exception();
    }
    // create return matrix
    Matrix ret(rhs.height(), rhs.width());
    for (int i = 0; i < rhs.height(); i++) {
        for (int j = 0; j < rhs.width(); j++) {
            // add matrices together
            ret[i][j] = (*this)[i][j] + rhs[i][j];
        }
    }
    return ret;
}

// subtraction operator overload
Matrix
Matrix::operator-(const Matrix& rhs) const {
    // if dimensions are different, throw exception
    if (rhs.height() != this->height() || rhs.width() != this->width()) {
        throw std::exception();
    }
    // create return matrix
    Matrix ret(rhs.height(), rhs.width());
    for (int i = 0; i < rhs.height(); i++) {
        for (int j = 0; j < rhs.width(); j++) {
            // subtract matrice values
            ret[i][j] = (*this)[i][j] - rhs[i][j];
        }
    }
    return ret;
}

// multiplication operator overload
Matrix
Matrix::operator*(const Matrix& rhs) const {
    // if dimensions are different, throw exception
    if (rhs.height() != this->height() || rhs.width() != this->width()) {
        throw std::exception();
    }
    // create return matrix
    Matrix ret(rhs.height(), rhs.width());
    for (int i = 0; i < rhs.height(); i++) {
        for (int j = 0; j < rhs.width(); j++) {
            // multiply matrice values
            ret[i][j] = (*this)[i][j] * rhs[i][j];
        }
    }
    return ret;
}

// multiply matrix by given value
Matrix
Matrix::operator*(const Val val) const {
    Matrix ret(this->height(), this->width());
    for (int i = 0; i < this->height(); i++) {
        for (int j = 0; j < this->width(); j++) {
            // multiply matrix by given value at each location
            ret[i][j] = (*this)[i][j] * val;
        }
    }
    return ret;
}

// method to do dot product of matrix
Matrix Matrix::dot(const Matrix& rhs) const {
    // if matrix dimensions aren't correct, throw exception
    if (rhs.height() != this->width()) {
        throw std::exception();
    }
    // create matrix
    Matrix ret(this->height(), rhs.width());
    for (int i = 0; i < this->height(); i++) {
        for (int j = 0; j < rhs.width(); j++) {
            for (int k = 0; k < rhs.height(); k++) {
                // apply dot product at each location
                ret[i][j] = ret[i][j] + ((*this)[i][k] * rhs[k][j]);
            }
        }
    }
    return ret;
}

// transpose matrix
Matrix Matrix::transpose() const {
    // create return matrix
    Matrix ret(this->width(), this->height());
    for (int i = 0; i < this->width(); i++) {
        for (int j = 0; j < this->height(); j++) {
            // switch matrix values at each location
            ret[i][j] = (*this)[j][i];
        }
    }
    return ret;
}

// Operator to write the matrix to a given output stream
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    // Print the number of rows and columns to ease reading
    os << matrix.height() << " " << matrix.width() << '\n';
    // Print each entry to the output stream.
    for (auto& row : matrix) {
        for (auto& val : row) {
            os << val << " ";
        }
        // Print a new line at the end of each row just to format the
        // output a bit nicely.
        os << '\n';
    }
    return os;
}



#endif
