#include "sorts/bubble_sort.hpp"
#include <cstdlib>

namespace sorting {

void bubbleSort(std::vector<int>& arr) {
    size_t n = arr.size();
    bool swapped;

    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

} // namespace sorting
