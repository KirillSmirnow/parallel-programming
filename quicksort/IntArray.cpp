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

    IntArray(IntArray *low, IntArray *high) : size(low->size + high->size) {
        content = new int[size];
        int i = 0;
        for (i = 0; i < low->size; i++) {
            content[i] = low->content[i];
        }
        for (; i < size; i++) {
            content[i] = high->content[i - low->size];
        }
    }

    int pivot() {
        return content[0];
    }

    void hoarPartition(int pivot, IntArray **low, IntArray **high) {
        int i = -1, j = size;
        while (true) {
            do {
                i++;
            } while (content[i] < pivot);
            do {
                j--;
            } while (content[j] > pivot);
            if (i >= j) break;
            swap(content[i], content[j]);
        }
        j++;
        *low = new IntArray(content, j);
        *high = new IntArray(&content[j], size - j);
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
