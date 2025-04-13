#include <vector>
#include "sorts/heap_sort.hpp"

namespace sorting {

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;       // 루트
    int left = 2 * i + 1;  // 왼쪽 자식
    int right = 2 * i + 2; // 오른쪽 자식

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest); // 재귀적으로 heapify
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Max-heap 빌드
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);

    // 하나씩 루트 제거해서 정렬
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);     // 현재 루트를 끝으로 보냄
        heapify(arr, i, 0);            // 남은 힙 재정렬
    }
}

}
