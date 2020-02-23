#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

class Matrix {
public:
    int rows;
    int columns;
    int **values;

    Matrix(int rows, int columns) : rows(rows), columns(columns) {
        values = new int *[rows];
        for (auto row = 0; row < rows; row++) {
            values[row] = new int[columns];
            for (auto column = 0; column < columns; column++) {
                values[row][column] = 4;
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

void generateAndPersistMatrices() {
    auto a = 3, b = 4, c = 5;
    Matrix(a, b).writeToFile("A.matrix");
    Matrix(b, c).writeToFile("B.matrix");
}

tuple<Matrix, Matrix> fetchMatrices() {
    return make_tuple(Matrix("A.matrix"), Matrix("B.matrix"));
}
