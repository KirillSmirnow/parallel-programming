#include "Matrix.cpp"

using namespace std;

int main() {
    srandom(5);
    generateAndPersistMatrices(5);
    Matrix *A, *B;
    fetchMatrices(&A, &B);
    A->writeToStdout();
    B->writeToStdout();
    return 0;
}
