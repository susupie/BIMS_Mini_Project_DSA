#pragma once 



#include <iostream> 

using namespace std;



// Simple bubble sort by key 

template <typename T, typename KeyType, typename GetKey>

void sortByKey(T* array, int size, GetKey getKey) {

    for (int i = 0; i < size - 1; ++i)

        for (int j = 0; j < size - i - 1; ++j)

            if (getKey(array[j]) > getKey(array[j + 1]))

                swap(array[j], array[j + 1]);

}

