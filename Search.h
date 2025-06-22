#pragma once

#include <iostream>
using namespace std;

template <typename T, typename Predicate>
T* sequenceSearch(T* array, int size, Predicate pred);

template <typename T, typename KeyType, typename GetKey>
T* binarySearch(T* array, int size, const KeyType& key, GetKey getKey);


template <typename T, typename Predicate>
T* sequenceSearch(T* array, int size, Predicate pred) {
    for (int i = 0; i < size; ++i) {
        if (pred(array[i]))
            return &array[i];
    }
    return nullptr;
}

template <typename T, typename KeyType, typename GetKey>
T* binarySearch(T* array, int size, const KeyType& key, GetKey getKey) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        KeyType midKey = getKey(array[mid]);
        if (midKey == key) return &array[mid];
        else if (midKey < key) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}