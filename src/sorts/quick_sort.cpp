#include <vector>
#include "sorts/quick_sort.hpp"

namespace sorting {

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // 마지막 요소를 피벗으로 선택
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low < high) {

        // ---- Median-of-Three Pivot Selection 추가 ----
        int mid = low + (high - low) / 2;

        // arr[low], arr[mid], arr[high] 중에서 중간값을 찾아서 arr[high] 위치로 보냅니다.
        // (간단한 비교와 swap 사용)
        if (arr[mid] < arr[low])
            std::swap(arr[mid], arr[low]);
        if (arr[high] < arr[low])
            std::swap(arr[high], arr[low]);
        // 이제 arr[low]는 세 값 중 가장 작은 값입니다.
        // arr[mid]와 arr[high]를 비교하여 더 작은 값(즉, 중간값)을 arr[high]로 보냅니다.
        if (arr[mid] < arr[high]) 
             std::swap(arr[mid], arr[high]);

        int pi = partition(arr, low, high);

        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

}
