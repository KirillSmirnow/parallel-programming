#include <fstream>
#include <mpi.h>
#include "Quicksort.cpp"

using namespace std;

IntArray *readArray();

int main() {
    MPI_Init(nullptr, nullptr);
    int currentProcess, totalProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);

    Quicksort quicksort(currentProcess, totalProcesses);
    quicksort.initialize(readArray());
    while (true) {
        if (quicksort.group->size() == 1) {
            quicksort.sort();
            break;
        }
        quicksort.pivot();
        quicksort.exchange();
        quicksort.regroup();
    }
    quicksort.collect();

    MPI_Finalize();
    return 0;
}

IntArray *readArray() {
    ifstream stream("../A00.input");
    int *content, size;
    stream >> size;
    content = new int[size];
    for (auto i = 0; i < size; i++) {
        stream >> content[i];
    }
    stream.close();
    return new IntArray(content, size);
}
