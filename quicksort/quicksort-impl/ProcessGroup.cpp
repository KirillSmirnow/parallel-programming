#pragma once

class ProcessGroup {
public:
    MPI_Comm communicator;
    int currentProcess;
    int totalProcesses;

    ProcessGroup(MPI_Comm communicator) : communicator(communicator) {
        MPI_Comm_rank(communicator, &currentProcess);
        MPI_Comm_size(communicator, &totalProcesses);
    }

    bool isInLeftHalf() {
        return currentProcess < totalProcesses / 2;
    }

    int getPartner() {
        if (isInLeftHalf()) {
            return currentProcess + totalProcesses / 2;
        }
        return currentProcess - totalProcesses / 2;
    }

    string toString() {
        return "(" + to_string(communicator) + ")";
    }
};
