#include "sorts/insertion_sort.hpp"
#include <vector>
#include <cstddef>

namespace sorting {

void insertionSort(std::vector<int>& arr) {
    for (std::size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        std::size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

}
