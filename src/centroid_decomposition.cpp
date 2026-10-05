#include "centroid_decomposition.h"

void CentroidDecompositionFolklore::dfs_traversal(int node, int parent) {
    subtree[node] = 1;
    for (auto neighbor: graph[node]) {
        // neighbor is a centroid OR neighbor is the parent
        if (cparent[neighbor] != -1) continue;
        if (neighbor == parent) continue;
        // everything is ok
        dfs_traversal(neighbor, node);
        subtree[node] += subtree[neighbor];
    }
}

void CentroidDecompositionFolklore::decompose(int node, int parent, int current_size, int prev_centroid) {
    // search_centroid for current tree
    for (auto neighbor: graph[node]) {
        if (neighbor == parent) continue;
        if (cparent[neighbor] != -1) continue;
        if ((subtree[neighbor] << 1) <= current_size) continue;
        decompose(neighbor, node, current_size, prev_centroid);
        return;
    }
    // we find our centroid
    cparent[node] = prev_centroid;
    // repeat process for connected components of node
    for (auto neighbor: graph[node]) {
        if (cparent[neighbor] != -1) continue;
        dfs_traversal(neighbor, node);
        decompose(neighbor, node, subtree[neighbor], node);
    }
}

CentroidDecompositionFolklore::CentroidDecompositionFolklore(int n, vector<vector<int>> &t): NRO_NODES(n + 1), graph(t) {
    // we asume an arbitrary centroid like u = 1
    cparent.resize(NRO_NODES, -1);
    subtree.resize(NRO_NODES, 0);
    dfs_traversal(1, 0);
    decompose(1, 0, subtree[1], 0);
}