#include "Matrix.cpp"
#include "Multiplication.cpp"

using namespace std;

void generateMatrices(int height, int width) {
    generateAndPersistMatrices(5);
}

void executeMultiplication(int mode) {
    Matrix *A, *B;
    fetchMatrices(&A, &B);

    Multiplication *multiplication;
    switch (mode) {
    }

    multiplication->execute();
}

int main(int argc, char **argv) {
    srandom(5);
    if (argc < 2) {
        return 1;
    }
    if (atoi(argv[1]) == 1) {
        if (argc != 4) {
            return 1;
        }
        generateMatrices(atoi(argv[2]), atoi(argv[3]));
    } else if (atoi(argv[1]) == 2) {
        if (argc != 3) {
            return 1;
        }
        executeMultiplication(atoi(argv[2]));
    } else {
        return 1;
    }
    return 0;
}
