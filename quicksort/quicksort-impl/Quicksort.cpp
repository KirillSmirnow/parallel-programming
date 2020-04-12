#pragma once

#include <iostream>
#include "IntArray.cpp"
#include "ProcessGroup.cpp"

using namespace std;

const int PRIMARY_PROCESS = 0;
const int MAX_ARRAY_SIZE = 10000000;

class Quicksort {
private:
    int currentPivot = 0;
    IntArray *currentArray = nullptr;

public:
    ProcessGroup *globalGroup;
    ProcessGroup *group;

    Quicksort() {
        globalGroup = new ProcessGroup(MPI_COMM_WORLD);
        group = globalGroup;
        log("Process " + to_string(group->currentProcess) + ". Total processes: " + to_string(group->totalProcesses));
    }

    void initialize(IntArray *array) {
        if (globalGroup->currentProcess == PRIMARY_PROCESS) {
            MPI_Request request;
            for (int process = 0; process < globalGroup->totalProcesses; process++) {
                auto part = array->getPart(process, globalGroup->totalProcesses);
                MPI_Isend(part->content, part->size, MPI_INT, process, 0, MPI_COMM_WORLD, &request);
            }
        }

        MPI_Status status;
        int *buffer = new int[MAX_ARRAY_SIZE];
        MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, PRIMARY_PROCESS, 0, MPI_COMM_WORLD, &status);

        int size;
        MPI_Get_count(&status, MPI_INT, &size);
        currentArray = new IntArray(buffer, size);

        log("Initialized: " + currentArray->toString());
    }

    void pivot() {
        if (group->currentProcess == PRIMARY_PROCESS) {
            currentPivot = currentArray->pivot();
        }
        MPI_Bcast(&currentPivot, 1, MPI_INT, PRIMARY_PROCESS, group->communicator);
        log("New pivot: " + to_string(currentPivot));
    }

    void exchange() {
        int partner = group->getPartner();

        IntArray *low, *high;
        currentArray->hoarPartition(currentPivot, &low, &high);
        log("Low = " + low->toString() + "; High = " + high->toString());

        MPI_Request request;
        if (group->isInLeftHalf()) {
            MPI_Isend(high->content, high->size, MPI_INT, partner, 0, group->communicator, &request);
        } else {
            MPI_Isend(low->content, low->size, MPI_INT, partner, 0, group->communicator, &request);
        }

        MPI_Status status;
        int *buffer = new int[MAX_ARRAY_SIZE];
        MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, partner, 0, group->communicator, &status);

        int size;
        MPI_Get_count(&status, MPI_INT, &size);
        if (group->isInLeftHalf()) {
            high = new IntArray(buffer, size);
        } else {
            low = new IntArray(buffer, size);
        }

        currentArray = new IntArray(low, high);

        log("New array: " + currentArray->toString());
    }

    void regroup() {
        int newGroup = group->isInLeftHalf() ? 0 : 1;
        MPI_Comm newCommunicator;
        MPI_Comm_split(group->communicator, newGroup, 0, &newCommunicator);
        group = new ProcessGroup(newCommunicator);
        log("New group: " + group->toString());
    }

    void sort() {
        qsort(currentArray->content, currentArray->size, sizeof(int), compareIntegers);
        log("Sorted: " + currentArray->toString());
    }

    IntArray *collect() {
        MPI_Request req;
        MPI_Isend(currentArray->content, currentArray->size, MPI_INT, PRIMARY_PROCESS, 0, MPI_COMM_WORLD, &req);

        if (globalGroup->currentProcess == PRIMARY_PROCESS) {
            IntArray *result = nullptr;
            for (int process = 0; process < globalGroup->totalProcesses; process++) {
                MPI_Status status;
                int *buffer = new int[MAX_ARRAY_SIZE];
                MPI_Recv(buffer, MAX_ARRAY_SIZE, MPI_INT, process, 0, MPI_COMM_WORLD, &status);

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
        ofstream stream(
                "qs." + to_string(globalGroup->currentProcess) + "-" + to_string(globalGroup->totalProcesses) + ".log",
                ios_base::app);
        stream << time(nullptr) << " - " << message << endl;
        stream.close();
    }

    static int compareIntegers(const void *a, const void *b) {
        int int1 = *((int *) a);
        int int2 = *((int *) b);
        return int1 - int2;
    }
};
