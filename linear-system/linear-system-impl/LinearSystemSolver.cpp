#pragma once

#include <fstream>

using namespace std;

const int PRIMARY_PROCESS = 0;

class LinearSystemSolver {
private:
    int currentProcess;
    int totalProcesses;

    int systemSize;
    double precision;
    double *A;        // systemSize x systemSize; A[i][j] = i * systemSize + j
    double *b;        // systemSize
    double *x;        // systemSize

public:
    LinearSystemSolver() {
        MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
        MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);
    }

    void initialize() {
        if (currentProcess == PRIMARY_PROCESS) {
            readParameters();
        }

        MPI_Bcast(&systemSize, 1, MPI_INT, PRIMARY_PROCESS, MPI_COMM_WORLD);
        MPI_Bcast(&precision, 1, MPI_DOUBLE, PRIMARY_PROCESS, MPI_COMM_WORLD);

        if (currentProcess != PRIMARY_PROCESS) {
            A = new double[systemSize * systemSize];
            b = new double[systemSize];
            x = new double[systemSize];
        }

        MPI_Bcast(A, systemSize * systemSize, MPI_DOUBLE, PRIMARY_PROCESS, MPI_COMM_WORLD);
        MPI_Bcast(b, systemSize, MPI_DOUBLE, PRIMARY_PROCESS, MPI_COMM_WORLD);
        MPI_Bcast(x, systemSize, MPI_DOUBLE, PRIMARY_PROCESS, MPI_COMM_WORLD);
    }

    void computeIteration() {
    }

    bool precisionReached() {
        return false;
    }

    void outputResult() {
        ofstream stream("linear-system.output");
        for (auto i = 0; i < systemSize; i++) {
            stream << x[i] << " ";
        }
        stream.close();
    }

private:
    void readParameters() {
        ifstream stream("linear-system.input");
        stream >> systemSize >> precision;
        x = new double[systemSize];
        for (auto i = 0; i < systemSize; i++) {
            stream >> x[i];
        }
        b = new double[systemSize];
        for (auto i = 0; i < systemSize; i++) {
            stream >> b[i];
        }
        A = new double[systemSize * systemSize];
        for (auto i = 0; i < systemSize; i++) {
            for (auto j = 0; j < systemSize; j++) {
                stream >> A[i * systemSize + j];
            }
        }
        stream.close();
    }
};
