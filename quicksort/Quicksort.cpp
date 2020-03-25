#pragma once

#include <iostream>
#include "IntArray.cpp"
#include "ProcessGroup.cpp"

using namespace std;

class Quicksort {
private:
    const int MAX_ARRAY_SIZE = 1000000;
    const int PRIMARY_PROCESS = 0;
    int currentProcess, totalProcesses;

    // MPI tags
    const int INITIALIZE = 0;
    const int PIVOT = 1;
    const int EXCHANGE = 2;
    const int REGROUP = 3;
    const int COLLECT = 4;

    int currentPivot;
    IntArray *currentArray = nullptr;

public:
    ProcessGroup *group = nullptr;

    Quicksort(int currentProcess, int totalProcesses)
            : currentProcess(currentProcess), totalProcesses(totalProcesses) {
        group = new ProcessGroup(0, totalProcesses - 1);
        cout << "Process " << currentProcess << ". Total processes: " << totalProcesses << endl;
    }

    void initialize(IntArray *array) {
        if (currentProcess == PRIMARY_PROCESS) {
            MPI_Request request;
            for (int process = 0; process < totalProcesses; process++) {
                auto part = array->getPart(process, totalProcesses);
                MPI_Isend(part->content, part->size, MPI_INT, process, INITIALIZE, MPI_COMM_WORLD, &request);
            }
        }

        MPI_Status status;
        int *buffer = new int[MAX_ARRAY_SIZE];
        MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, PRIMARY_PROCESS, INITIALIZE, MPI_COMM_WORLD, &status);

        int size;
        MPI_Get_count(&status, MPI_INT, &size);
        currentArray = new IntArray(buffer, size);

        log("Initialized: " + currentArray->toString());
    }

    void pivot() {
        if (currentProcess == group->master()) {
            int pivot = currentArray->pivot();
            MPI_Request request;
            for (int process = 0; process < totalProcesses; process++) {
                MPI_Isend(&pivot, 1, MPI_INT, process, PIVOT, MPI_COMM_WORLD, &request);
            }
        }

        MPI_Status status;
        MPI_Recv(&currentPivot, 1, MPI_INT, PRIMARY_PROCESS, PIVOT, MPI_COMM_WORLD, &status);

        log("New pivot: " + to_string(currentPivot));
    }

    void exchange() {
        int partner = group->partnerFor(currentProcess);

        IntArray *low, *high;
        currentArray->hoarPartition(currentPivot, &low, &high);
        log("Low = " + low->toString() + "; High = " + high->toString());

        MPI_Request request;
        if (group->isInLeftHalf(currentProcess)) {
            MPI_Isend(high->content, high->size, MPI_INT, partner, EXCHANGE, MPI_COMM_WORLD, &request);
        } else {
            MPI_Isend(low->content, low->size, MPI_INT, partner, EXCHANGE, MPI_COMM_WORLD, &request);
        }

        MPI_Status status;
        int *buffer = new int[MAX_ARRAY_SIZE];
        MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, partner, EXCHANGE, MPI_COMM_WORLD, &status);

        int size;
        MPI_Get_count(&status, MPI_INT, &size);
        if (group->isInLeftHalf(currentProcess)) {
            high = new IntArray(buffer, size);
        } else {
            low = new IntArray(buffer, size);
        }

        currentArray = new IntArray(low, high);

        log("New array: " + currentArray->toString());
    }

    void regroup() {
    }

    void sort() {
    }

    void collect() {
    }

private:
    void log(const string &message) {
        cout << "#" << currentProcess << " - " << message << endl;
    }
};
