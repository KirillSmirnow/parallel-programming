#pragma once

#include <iostream>
#include <fstream>
#include "Utils.cpp"

using namespace std;

class Matrix {
public:
    int rows;
    int columns;
    int **values;

    Matrix() : Matrix(0, 0) {}

    Matrix(int rows, int columns) : rows(rows), columns(columns) {
        values = new int *[rows];
        for (auto row = 0; row < rows; row++) {
            values[row] = new int[columns];
            for (auto column = 0; column < columns; column++) {
                values[row][column] = randomInt(-1000, 1000);
            }
        }
    }

    Matrix(string filename) {
        ifstream stream(filename);
        stream >> rows >> columns;
        values = new int *[rows];
        for (auto row = 0; row < rows; row++) {
            values[row] = new int[columns];
            for (auto column = 0; column < columns; column++) {
                stream >> values[row][column];
            }
        }
        stream.close();
    }

    void writeToStdout() {
        cout << rows << "×" << columns << endl;
        for (auto row = 0; row < rows; row++) {
            for (auto column = 0; column < columns; column++) {
                cout << values[row][column] << " ";
            }
            cout << endl;
        }
    }

    void writeToFile(string filename) {
        ofstream stream(filename);
        stream << rows << " " << columns << endl;
        for (auto row = 0; row < rows; row++) {
            for (auto column = 0; column < columns; column++) {
                stream << values[row][column] << " ";
            }
            stream << endl;
        }
        stream.close();
    }
};

void generateAndPersistMatrices(int height, int width) {
    auto b = 500;
    Matrix(height, b).writeToFile("A.matrix");
    Matrix(b, width).writeToFile("B.matrix");
}

void fetchMatrices(Matrix **A, Matrix **B) {
    *A = new Matrix("A.matrix");
    *B = new Matrix("B.matrix");
}
