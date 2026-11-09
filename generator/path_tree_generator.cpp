#include "path_gen.h"

std::vector<std::vector<int>> generate_path_tree(int n) {
    std::vector<std::vector<int>> graph(n + 1);
    if (n <= 1) return graph;

    for (int v = 2; v <= n; ++v) {
        graph[v - 1].push_back(v);
        graph[v].push_back(v - 1);
    }

    return graph;
}
