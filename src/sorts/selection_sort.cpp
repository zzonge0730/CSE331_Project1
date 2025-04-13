#include <vector>
#include "sorts/selection_sort.hpp"

namespace sorting {

void selectionSort(std::vector<int>& arr) {
    std::size_t n = arr.size();

    for (std::size_t i = 0; i < n - 1; ++i) {
        std::size_t minIndex = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

}
