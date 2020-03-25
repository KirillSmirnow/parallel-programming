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

    bool isInLeftHalf(int process) {
        return process < size() / 2;
    }

    int partnerFor(int process) {
        if (isInLeftHalf(process)) {
            return process + size() / 2;
        }
        return process - size() / 2;
    }
};
