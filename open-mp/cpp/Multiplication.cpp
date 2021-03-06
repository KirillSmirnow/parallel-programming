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
            result[row] = new int[columns];
            for (int column = 0; column < columns; column++) {
                result[row][column] = computeElement(row, column);
            }
        }
        return toMatrix(result);
    }
};

class ParallelForMultiplication : public Multiplication {
public:
    ParallelForMultiplication(Matrix *a, Matrix *b) : Multiplication(a, b) {}

    Matrix *execute() override {
        int rows = A->rows;
        int columns = B->columns;
        int **result = new int *[rows];

#pragma omp parallel for schedule(static)
        for (int row = 0; row < rows; row++) {
            result[row] = new int[columns];
            for (int column = 0; column < columns; column++) {
                result[row][column] = computeElement(row, column);
            }
        }

        return toMatrix(result);
    }
};

class ParallelTasksStaticScheduleMultiplication : public Multiplication {
public:
    ParallelTasksStaticScheduleMultiplication(Matrix *a, Matrix *b) : Multiplication(a, b) {}

    Matrix *execute() override {
        int rows = A->rows;
        int columns = B->columns;

        int **result = new int *[rows];
#pragma omp parallel for schedule(static)
        for (int row = 0; row < rows; row++) {
            result[row] = new int[columns];
        }

        int tasks = rows * columns;
#pragma omp parallel for schedule(static)
        for (int task = 0; task < tasks; task++) {
            int row = task / columns;
            int column = task % columns;
            result[row][column] = computeElement(row, column);
        }

        return toMatrix(result);
    }
};

class ParallelTasksDynamicScheduleMultiplication : public Multiplication {
public:
    ParallelTasksDynamicScheduleMultiplication(Matrix *a, Matrix *b) : Multiplication(a, b) {}

    Matrix *execute() override {
        int rows = A->rows;
        int columns = B->columns;

        int **result = new int *[rows];
#pragma omp parallel for schedule(static)
        for (int row = 0; row < rows; row++) {
            result[row] = new int[columns];
        }

        int tasks = rows * columns;
#pragma omp parallel for schedule(dynamic, 1000)
        for (int task = 0; task < tasks; task++) {
            int row = task / columns;
            int column = task % columns;
            result[row][column] = computeElement(row, column);
        }

        return toMatrix(result);
    }
};

class ParallelTasksGuidedScheduleMultiplication : public Multiplication {
public:
    ParallelTasksGuidedScheduleMultiplication(Matrix *a, Matrix *b) : Multiplication(a, b) {}

    Matrix *execute() override {
        int rows = A->rows;
        int columns = B->columns;

        int **result = new int *[rows];
#pragma omp parallel for schedule(static)
        for (int row = 0; row < rows; row++) {
            result[row] = new int[columns];
        }

        int tasks = rows * columns;
#pragma omp parallel for schedule(guided, 1000)
        for (int task = 0; task < tasks; task++) {
            int row = task / columns;
            int column = task % columns;
            result[row][column] = computeElement(row, column);
        }

        return toMatrix(result);
    }
};
