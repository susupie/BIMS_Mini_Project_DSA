#pragma once
#include <iostream>
using namespace std;

// Bubble Sort
template <typename T, typename KeyType, typename GetKey>
void bubbleSortByKey(T* array, int size, GetKey getKey) {
    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size - i - 1; ++j)
            if (getKey(array[j]) > getKey(array[j + 1]))
                swap(array[j], array[j + 1]);
}

// Selection Sort
template <typename T, typename KeyType, typename GetKey>
void selectionSortByKey(T* array, int size, GetKey getKey) {
    for (int i = 0; i < size - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < size; ++j)
            if (getKey(array[j]) < getKey(array[minIdx]))
                minIdx = j;
        if (minIdx != i)
            swap(array[i], array[minIdx]);
    }
}

// Insertion Sort
template <typename T, typename KeyType, typename GetKey>
void insertionSortByKey(T* array, int size, GetKey getKey) {
    for (int i = 1; i < size; ++i) {
        T keyItem = array[i];
        int j = i - 1;
        while (j >= 0 && getKey(array[j]) > getKey(keyItem)) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = keyItem;
    }
}

