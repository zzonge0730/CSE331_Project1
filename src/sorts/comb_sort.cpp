#include "sorts/comb_sort.hpp"
#include <vector>
#include <cmath>

namespace sorting {

void combSort(std::vector<int>& arr) {
    int n = arr.size();
    int gap = n;
    bool swapped = true;
    const double shrink = 1.3;

    while (gap > 1 || swapped) {
        gap = std::max(1, (int)(gap / shrink));
        swapped = false;

        for (int i = 0; i + gap < n; ++i) {
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}

}
