#include "Matrix.cpp"

using namespace std;

int main() {
    generateAndPersistMatrices(5);
    Matrix A(0, 0), B(0, 0);
    tie(A, B) = fetchMatrices();
    A.writeToStdout();
    B.writeToStdout();
    return 0;
}
