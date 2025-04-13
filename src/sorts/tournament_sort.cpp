#include "sorts/tournament_sort.hpp"
#include <vector>
#include <climits>

namespace sorting {

struct TreeNode {
    int value;
    int index;
};

void buildTree(const std::vector<int>& arr, std::vector<TreeNode>& tree) {
    int n = arr.size();
    int leafStart = tree.size() / 2;

    // Leaf에 값 삽입
    for (int i = 0; i < n; ++i) {
        tree[leafStart + i] = {arr[i], i};
    }

    // 남은 leaf는 무한대로 채움
    for (int i = leafStart + n; i < tree.size(); ++i) {
        tree[i] = {INT_MAX, -1};
    }

    // 위로 올라가며 이긴 값을 설정
    for (int i = leafStart - 1; i > 0; --i) {
        TreeNode left = tree[2 * i];
        TreeNode right = tree[2 * i + 1];
        tree[i] = (left.value < right.value) ? left : right;
    }
}

void replaceAndUpdate(std::vector<TreeNode>& tree, int leafStart, int index) {
    int i = leafStart + index;
    tree[i].value = INT_MAX;

    // 위로 올라가며 이긴 값을 갱신
    while (i > 1) {
        int parent = i / 2;
        TreeNode left = tree[2 * parent];
        TreeNode right = tree[2 * parent + 1];
        tree[parent] = (left.value < right.value) ? left : right;
        i = parent;
    }
}

void tournamentSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // 다음 2의 거듭제곱 크기
    int leafCount = 1;
    while (leafCount < n) leafCount *= 2;

    // 전체 트리 크기 = 2 * leafCount
    std::vector<TreeNode> tree(2 * leafCount);

    buildTree(arr, tree);
    int leafStart = leafCount;

    std::vector<int> result;
    result.reserve(n);

    for (int i = 0; i < n; ++i) {
        int winnerIndex = tree[1].index;
        result.push_back(arr[winnerIndex]);

        replaceAndUpdate(tree, leafStart, winnerIndex);
    }

    arr = std::move(result);
}

}
