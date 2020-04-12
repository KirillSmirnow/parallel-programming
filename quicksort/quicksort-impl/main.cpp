#include <fstream>
#include <mpi.h>
#include "Quicksort.cpp"

using namespace std;

IntArray *readArray();

void writeArray(IntArray *array);

int main() {
    MPI_Init(nullptr, nullptr);
    Quicksort quicksort;

    if (quicksort.globalGroup->currentProcess == PRIMARY_PROCESS) {
        quicksort.initialize(readArray());
    } else {
        quicksort.initialize(nullptr);
    }

    while (true) {
        if (quicksort.group->totalProcesses == 1) {
            quicksort.sort();
            break;
        }
        quicksort.pivot();
        quicksort.exchange();
        quicksort.regroup();
    }

    if (quicksort.globalGroup->currentProcess == PRIMARY_PROCESS) {
        writeArray(quicksort.collect());
    } else {
        quicksort.collect();
    }

    MPI_Finalize();
    return 0;
}

IntArray *readArray() {
    ifstream stream("quicksort.input");
    int *content, size;
    stream >> size;
    content = new int[size];
    for (auto i = 0; i < size; i++) {
        stream >> content[i];
    }
    stream.close();
    return new IntArray(content, size);
}

void writeArray(IntArray *array) {
    ofstream stream("quicksort.output");
    stream << array->size << endl;
    for (auto i = 0; i < array->size; i++) {
        stream << array->content[i] << " ";
    }
    stream.close();
}
