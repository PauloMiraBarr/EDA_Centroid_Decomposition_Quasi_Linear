#include "complete_gen.h"

std::vector<std::vector<int>> generate_complete_binary_tree(int n) {
    std::vector<std::vector<int>> graph(n + 1);
    if (n <= 1) return graph;

    for (int v = 2; v <= n; ++v) {
        int p = v / 2;

        graph[p].push_back(v);
        graph[v].push_back(p);
    }

    return graph;
}
