#include "Matrix.cpp"

using namespace std;

int main() {
    srandom(5);
    generateAndPersistMatrices(5);
    Matrix A, B;
    tie(A, B) = fetchMatrices();
    A.writeToStdout();
    B.writeToStdout();
    return 0;
}
