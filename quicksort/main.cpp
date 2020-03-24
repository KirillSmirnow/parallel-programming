#include <iostream>
#include <mpi.h>

using namespace std;

int main() {
    auto initResult = MPI_Init(nullptr, nullptr);
    cout << "Init result = " << initResult << endl;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "My rank is " << rank << endl;

    MPI_Finalize();
    return 0;
}
