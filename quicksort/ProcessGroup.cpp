#pragma once

class ProcessGroup {
public:
    const int first;
    const int last;

    ProcessGroup(const int first, const int last) : first(first), last(last) {}

    int master() {
        return first;
    }

    int size() {
        return last - first + 1;
    }

    ProcessGroup *getLeftHalfGroup() {
        return new ProcessGroup(first, first + size() / 2 - 1);
    }

    ProcessGroup *getRightHalfGroup() {
        return new ProcessGroup(first + size() / 2, last);
    }

    bool isInLeftHalf(int process) {
        return process - first < size() / 2;
    }

    int partnerFor(int process) {
        if (isInLeftHalf(process)) {
            return (process - first + size() / 2) + first;
        }
        return (process - first - size() / 2) + first;
    }

    string toString() {
        return "(" + to_string(first) + ", " + to_string(last) + ")";
    }
};

int *serializeGroup(ProcessGroup *group) {
    return new int[2]{group->first, group->last};
}

ProcessGroup *deserializeGroup(int *data) {
    return new ProcessGroup(data[0], data[1]);
}
