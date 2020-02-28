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

    Matrix *toMatrix(int **values) {
        auto matrix = new Matrix();
        matrix->rows = A->rows;
        matrix->columns = B->columns;
        matrix->values = values;
        return matrix;
    }
};

class SerialMultiplication : public Multiplication {
public:
    SerialMultiplication(Matrix *a, Matrix *b) : Multiplication(a, b) {}

    Matrix *execute() override {
        int rows = A->rows;
        int columns = B->columns;
        int **result = new int *[rows];
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                result[row][column] = computeElement(row, column);
            }
        }
        return toMatrix(result);
    }
};
