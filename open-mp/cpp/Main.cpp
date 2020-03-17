#include "Matrix.cpp"
#include "Multiplication.cpp"

using namespace std;

void executeMultiplication(int rows, int columns, int mode) {
    auto b = 500;
    auto A = new Matrix(rows, b);
    auto B = new Matrix(b, columns);

    Multiplication *multiplication;
    switch (mode) {
        case 1:
            multiplication = new ParallelForMultiplication(A, B);
            break;
        case 2:
            multiplication = new ParallelTasksMultiplication(A, B);
            break;
        default:
            multiplication = new SerialMultiplication(A, B);
    }

    multiplication->execute();
}

int main(int argc, char **argv) {
    srandom(time(nullptr));
    if (argc != 4) {
        return 1;
    }
    executeMultiplication(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    return 0;
}
