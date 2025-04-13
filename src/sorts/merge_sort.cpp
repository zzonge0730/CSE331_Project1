#include <vector>
#include "sorts/merge_sort.hpp"

namespace sorting {

void merge(std::vector<int>& arr, std::size_t left, std::size_t mid, std::size_t right) {
    std::size_t n1 = mid - left + 1;
    std::size_t n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (std::size_t i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (std::size_t j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    std::size_t i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSortHelper(std::vector<int>& arr, std::size_t left, std::size_t right) {
    if (left < right) {
        std::size_t mid = left + (right - left) / 2;

        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<int>& arr) {
    if (arr.size() > 1)
        mergeSortHelper(arr, 0, arr.size() - 1);
}

}
