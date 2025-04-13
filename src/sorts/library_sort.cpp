#include <vector>
#include <algorithm>
#include <cmath>
#include <utility> // for std::swap
#include "sorts/library_sort.hpp"

#define EMPTY -1

namespace sorting {

// findInsertPos: (이전과 동일)
int findInsertPos(const std::vector<int>& shelf, int x) {
    // shelf의 첫 요소가 EMPTY거나 비어있으면 탐색 시작 위치 조정 필요
    int first_non_empty = -1;
    for(int i = 0; i < shelf.size(); ++i) {
        if (shelf[i] != EMPTY) {
            first_non_empty = i;
            break;
        }
    }
    if (first_non_empty == -1) return 0; // 전부 비어있음

    int low = first_non_empty;
    int high = shelf.size() - 1; // high는 마지막 인덱스까지
    int pos = shelf.size(); // 기본값: 맨 끝

    // 이진 탐색 전, x가 가장 작은 값인지 확인
    if (x <= shelf[first_non_empty]) {
        return first_non_empty;
    }

    // 이진 탐색으로 적절한 위치 찾기
    while (low <= high) {
        int mid_idx = low + (high - low) / 2;

        // mid_idx 근처의 non-EMPTY 값을 찾음 (왼쪽 우선 탐색)
        int actual_mid_idx = mid_idx;
        while (actual_mid_idx >= low && shelf[actual_mid_idx] == EMPTY) {
            actual_mid_idx--;
        }

        // 만약 low ~ mid_idx 구간이 전부 EMPTY 였다면, non-EMPTY 값은 mid_idx 오른쪽에 있음
        if (actual_mid_idx < low) {
             // 오른쪽 구간에서 non-EMPTY 탐색
             actual_mid_idx = mid_idx + 1; // mid_idx 다음부터 탐색 시작
             while(actual_mid_idx <= high && shelf[actual_mid_idx] == EMPTY) {
                 actual_mid_idx++;
             }
             // 오른쪽에도 non-EMPTY가 없다면 탐색 종료 (이 경우는 거의 없지만 안전하게)
             if (actual_mid_idx > high) {
                  break; // 모든 범위 탐색 완료
             }
             // 오른쪽에서 찾은 non-EMPTY 기준으로 비교
             if (shelf[actual_mid_idx] < x) {
                 low = actual_mid_idx + 1;
             } else {
                 pos = actual_mid_idx;
                 high = actual_mid_idx -1; // 여기서 high 조절 필요
             }
        } else {
            // 왼쪽에서 non-EMPTY 값을 찾은 경우 (기존 로직과 유사)
            if (shelf[actual_mid_idx] < x) {
                low = mid_idx + 1; // low 는 원래 mid 기준 + 1
            } else {
                pos = actual_mid_idx;
                high = actual_mid_idx - 1; // high 는 찾은 non-EMPTY 값 기준 -1
            }
        }
    }
    return pos;
}


// findProperEmpty: (이전과 동일)
int findProperEmpty(const std::vector<int>& shelf, int pos, int x) {
    int n = shelf.size();
    // pos가 범위를 벗어난 경우, 올바른 범위 내로 조정
    if (pos >= n) pos = n - 1;
    // shelf[pos]가 이미 비어있다면 pos를 반환하기보다 pos 근처 빈칸 탐색이 더 일반적
    // 하지만 현재 로직 유지 시 큰 문제는 없을 수 있음

    // x <= shelf[pos] 인지 확인해야 하는데, shelf[pos]가 EMPTY일 수 있음.
    // pos 위치의 non-EMPTY 값을 찾아 비교하거나, pos 자체를 기준으로 탐색 방향 결정
    int reference_val = EMPTY;
    if (pos >= 0 && pos < n && shelf[pos] != EMPTY) {
        reference_val = shelf[pos];
    } else {
         // pos가 비어있거나 범위를 벗어났다면, pos 기준 좌우 탐색
         // 여기서는 단순화하여 pos 기준 탐색 (개선 여지 있음)
    }


    // x가 작거나 같으면 왼쪽 우선 (pos 포함)
    // reference_val != EMPTY && x <= reference_val 이거나, reference_val == EMPTY 일때 왼쪽 우선 탐색 가능
    // 여기서는 단순화: pos 기준 왼쪽 우선 탐색
    for (int i = pos; i >= 0; --i) {
        if (shelf[i] == EMPTY)
            return i;
    }
    // 왼쪽에 빈 슬롯이 없으면 오른쪽 탐색
    for (int i = pos + 1; i < n; ++i) {
        if (shelf[i] == EMPTY)
            return i;
    }


    // 위에서 못 찾으면 빈 슬롯 없음
    return -1;
}

// rebalance: (이전과 동일)
void rebalance(std::vector<int>& shelf, int filled, double /*gap_factor*/) {
    int oldSize = shelf.size();
    int newSize = oldSize * 2;
    std::vector<int> newShelf(newSize, EMPTY);

    std::vector<int> values;
    values.reserve(filled); // 미리 용량 확보
    for (int x : shelf) {
        if (x != EMPTY)
            values.push_back(x);
    }
    // rebalance 시에는 반드시 정렬된 상태여야 findInsertPos가 작동
    // std::sort(values.begin(), values.end()); // 값들이 이미 정렬되어 있다는 가정 제거 (rebalance에서 정렬)

    // filled 가 0인 경우 예외 처리
    if (filled == 0) {
        shelf = std::move(newShelf);
        return;
    }

    int newGap = newSize / filled; // 새 간격 계산
    if (newGap < 1) newGap = 1;

    // 값을 새 shelf에 균등 배치
    int j = newGap / 2; // 중앙에 가깝게 시작 (개선된 시작점)
    for (int val : values) {
        if (j >= newSize) { // 배열 범위 초과 방지
             // 이 경우 newSize가 너무 작거나 filled가 잘못 계산된 경우
             // 비상: 남은 값들을 뒤쪽 빈칸에 채우기 (드문 경우)
             for (int k=0; k<newSize; ++k) if(newShelf[k] == EMPTY) {newShelf[k]=val; break;}

        } else {
            // j 위치가 비어있는지 확인 후 배치 (더 안전함)
            while(j < newSize && newShelf[j] != EMPTY) j++;
            if (j < newSize) newShelf[j] = val;
            else { /* 비상 처리 */ for (int k=0; k<newSize; ++k) if(newShelf[k] == EMPTY) {newShelf[k]=val; break;} }

        }
        j += newGap;
    }
    shelf = std::move(newShelf);
}

// --- Helper functions for local shifting ---
// Find the index of the previous non-EMPTY element
int find_prev_non_empty(const std::vector<int>& shelf, int index) {
    for (int i = index - 1; i >= 0; --i) {
        if (shelf[i] != EMPTY) {
            return i;
        }
    }
    return -1; // Not found
}

// Find the index of the next non-EMPTY element
int find_next_non_empty(const std::vector<int>& shelf, int index) {
    int n = shelf.size();
    for (int i = index + 1; i < n; ++i) {
        if (shelf[i] != EMPTY) {
            return i;
        }
    }
    return -1; // Not found
}
// --- End Helper functions ---


void librarySort(std::vector<int>& arr) {
    if (arr.empty())
        return;

    double gap_factor = 2.0; // Shelf size will be roughly gap_factor * n
    int n = arr.size();
    int filled = 0;

    // Initial shelf size calculation
    int shelfSize = std::max((int)std::ceil(n * gap_factor), 2); // Ensure shelf size is at least 2
    std::vector<int> shelf(shelfSize, EMPTY);

    // Insert the first element near the middle
    int startIndex = shelfSize / 2;
    shelf[startIndex] = arr[0];
    filled = 1;

    // Insert remaining elements
    for (int i = 1; i < n; ++i) {
        // Check if rebalancing is needed before insertion
        // Condition: filled count is more than half the shelf size (adjust factor if needed)
        if (filled * gap_factor > shelf.size()) { // Rebalance if density is high
            rebalance(shelf, filled, gap_factor);
            shelfSize = shelf.size(); // Update shelfSize after rebalance
        }

        int value_to_insert = arr[i];
        int pos = findInsertPos(shelf, value_to_insert); // Find logical position

        // Find an empty slot for insertion, preferably near 'pos'
        int emptyPos = findProperEmpty(shelf, pos, value_to_insert);

        // If no empty slot is found nearby, rebalance and retry current element
        if (emptyPos == -1) {
            rebalance(shelf, filled, gap_factor);
            shelfSize = shelf.size(); // Update shelfSize
            --i; // Decrement i to retry inserting the current element in the next iteration
            continue;
        }

        // --- Perform Insertion with Local Shifting ---
        int current_idx = emptyPos;
        shelf[current_idx] = value_to_insert; // Tentatively place the value

        // Use insertion sort like shifting to place the element correctly among non-EMPTY neighbors
        int final_pos = current_idx; // Track the final position after shifting

        // Shift larger elements to the right
        int prev_idx = find_prev_non_empty(shelf, current_idx);
        while (prev_idx != -1 && shelf[prev_idx] > value_to_insert) {
             // Need to find an empty slot to the right of current_idx to move shelf[prev_idx] into.
             // This simple swap assumes the slot to the right (current_idx) is the target hole.
             shelf[current_idx] = shelf[prev_idx]; // Move larger element to the right (into the current hole)
             shelf[prev_idx] = EMPTY; // Mark the original position as empty
             final_pos = prev_idx;      // Update the potential final position
             current_idx = prev_idx;    // The hole moved left
             prev_idx = find_prev_non_empty(shelf, current_idx); // Find next left element
        }

        // Place the value in its final correct position (final_pos)
        shelf[final_pos] = value_to_insert;


        // Optional: Shift smaller elements to the left (if needed, often handled by the above)
        // Reset current_idx to the final_pos where the element was placed
        current_idx = final_pos;
        int next_idx = find_next_non_empty(shelf, current_idx);
        while (next_idx != -1 && shelf[next_idx] < value_to_insert) {
            shelf[current_idx] = shelf[next_idx]; // Move smaller element left
            shelf[next_idx] = EMPTY; // Mark original spot empty
            final_pos = next_idx;     // Update potential final position
            current_idx = next_idx;   // The hole moved right
            next_idx = find_next_non_empty(shelf, current_idx);
        }
        shelf[final_pos] = value_to_insert; // Place value in final correct position


        filled++;
        // --- End Insertion with Local Shifting ---

    }

    // Copy sorted elements back to the original array
    arr.clear();
    arr.reserve(filled); // Reserve capacity
    for (int x : shelf) {
        if (x != EMPTY) {
            arr.push_back(x);
        }
    }
}

} // namespace sorting