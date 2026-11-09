#include "random_gen.h"

#include <functional>
#include <queue>
#include <random>

std::vector<std::vector<int>> generate_random_tree(int n, unsigned int seed) {
    std::vector<std::vector<int>> graph(n + 1);
    if (n <= 1) return graph;

    if (n == 2) {
        graph[1].push_back(2);
        graph[2].push_back(1);
        return graph;
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, n);

    std::vector<int> sequence(n - 2);
    std::vector<int> degree(n + 1, 1);

    for (int i = 0; i < n - 2; ++i) {
        sequence[i] = dist(rng);
        ++degree[sequence[i]];
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;
    for (int v = 1; v <= n; ++v) {
        if (degree[v] == 1) leaves.push(v);
    }

    for (int v : sequence) {
        const int leaf = leaves.top();
        leaves.pop();

        graph[leaf].push_back(v);
        graph[v].push_back(leaf);

        if (--degree[v] == 1) leaves.push(v);
    }

    const int first = leaves.top();
    leaves.pop();
    const int second = leaves.top();

    graph[first].push_back(second);
    graph[second].push_back(first);

    return graph;
}
