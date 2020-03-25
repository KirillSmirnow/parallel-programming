#pragma once

#include <string>

using namespace std;

class IntArray {
public:
    int *content;
    int size;

    IntArray(const int *buffer, int size) : size(size) {
        content = new int[size];
        for (int i = 0; i < size; i++) {
            content[i] = buffer[i];
        }
    }

    int pivot() {
        return content[0];
    }

    IntArray *getPart(int part, int totalParts) {
        int partSize = size / totalParts;
        int begin = part * partSize;
        if (part == totalParts - 1) {
            // last part
            partSize = size - begin;
        }
        return new IntArray(&content[begin], partSize);
    }

    string toString() {
        string contentString;
        if (size < 20) {
            for (int i = 0; i < size; i++) {
                contentString += to_string(content[i]);
                if (i < size - 1) contentString += " ";
            }
        } else {
            for (int i = 0; i < 5; i++) {
                contentString += to_string(content[i]) + " ";
            }
            contentString += " ... ";
            for (int i = size - 5; i < size; i++) {
                contentString += to_string(content[i]);
                if (i < size - 1) contentString += " ";
            }
        }
        return "|" + to_string(size) + "|[" + contentString + "]";
    }
};
