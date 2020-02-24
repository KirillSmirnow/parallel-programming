#pragma once

#include "Matrix.cpp"

class Multiplication {
public:
    Matrix *A;
    Matrix *B;

    Multiplication(Matrix *a, Matrix *b) : A(a), B(b) {}

    virtual Matrix *execute() = 0;

    virtual int computeElement(int row, int column) {
        int result = 0;
        for (int i = 0; i < A->columns; i++) {
            result += A->values[row][i] * B->values[i][column];
        }
        return result;
    }
};
