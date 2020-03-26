#pragma once

#include <iostream>
#include "IntArray.cpp"
#include "ProcessGroup.cpp"

using namespace std;

const int PRIMARY_PROCESS = 0;

class Quicksort {
private:
    const int MAX_ARRAY_SIZE = 1000000;
    int currentProcess, totalProcesses;

    // MPI tags
    const int INITIALIZE = 0;
    const int PIVOT = 1;
    const int EXCHANGE = 2;
    const int REGROUP = 3;
    const int COLLECT = 4;

    int currentPivot = 0;
    IntArray *currentArray = nullptr;

public:
    ProcessGroup *group = nullptr;

    Quicksort(int currentProcess, int totalProcesses)
            : currentProcess(currentProcess), totalProcesses(totalProcesses) {
        group = new ProcessGroup(0, totalProcesses - 1);
        log("Process " + to_string(currentProcess) + ". Total processes: " + to_string(totalProcesses));
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
        MPI_Recv(&currentPivot, 1, MPI_INT, group->master(), PIVOT, MPI_COMM_WORLD, &status);

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
        if (currentProcess == group->master()) {
            MPI_Request request;
            for (int process = 0; process < totalProcesses; process++) {
                ProcessGroup *newGroup;
                if (group->isInLeftHalf(process)) {
                    newGroup = group->getLeftHalfGroup();
                } else {
                    newGroup = group->getRightHalfGroup();
                }
                MPI_Isend(serializeGroup(newGroup), 2, MPI_INT, process, REGROUP, MPI_COMM_WORLD, &request);
            }
        }

        int *buffer = new int[2];
        MPI_Status status;
        MPI_Recv(buffer, 2, MPI_INT, group->master(), REGROUP, MPI_COMM_WORLD, &status);
        group = deserializeGroup(buffer);

        log("New group: " + group->toString());
    }

    void sort() {
        qsort(currentArray->content, currentArray->size, sizeof(int), compareIntegers);
        log("Sorted: " + currentArray->toString());
    }

    IntArray *collect() {
        MPI_Request req;
        MPI_Isend(currentArray->content, currentArray->size, MPI_INT, PRIMARY_PROCESS, COLLECT, MPI_COMM_WORLD, &req);

        if (currentProcess == PRIMARY_PROCESS) {
            IntArray *result = nullptr;
            for (int process = 0; process < totalProcesses; process++) {
                MPI_Status status;
                int *buffer = new int[MAX_ARRAY_SIZE];
                MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, process, COLLECT, MPI_COMM_WORLD, &status);

                int size;
                MPI_Get_count(&status, MPI_INT, &size);

                auto *array = new IntArray(buffer, size);
                if (result == nullptr) {
                    result = array;
                } else {
                    result = new IntArray(result, array);
                }
            }
            log("Result collected: " + result->toString());
            return result;
        }

        return nullptr;
    }

private:
    void log(const string &message) {
        ofstream stream("qs." + to_string(currentProcess) + "-" + to_string(totalProcesses) + ".log", ios_base::app);
        stream << time(nullptr) << " - " << message << endl;
        stream.close();
    }

    static int compareIntegers(const void *a, const void *b) {
        int int1 = *((int *) a);
        int int2 = *((int *) b);
        return int1 - int2;
    }
};
