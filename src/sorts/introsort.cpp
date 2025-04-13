#include "sorts/introsort.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

namespace sorting {

static void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

static void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

static int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) std::swap(arr[++i], arr[j]);
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void introsortUtil(std::vector<int>& arr, int begin, int end, int depthLimit) {
    if (end - begin <= 16) {
        std::sort(arr.begin() + begin, arr.begin() + end + 1);
        return;
    }

    if (depthLimit == 0) {
        std::vector<int> sub(arr.begin() + begin, arr.begin() + end + 1);
        heapSort(sub);
        std::copy(sub.begin(), sub.end(), arr.begin() + begin);
        return;
    }

    int p = partition(arr, begin, end);
    introsortUtil(arr, begin, p - 1, depthLimit - 1);
    introsortUtil(arr, p + 1, end, depthLimit - 1);
}

void introsort(std::vector<int>& arr) {
    int depthLimit = 2 * std::log2(arr.size());
    introsortUtil(arr, 0, arr.size() - 1, depthLimit);
}

}
