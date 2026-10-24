#include "centroid_decomposition_folklore.h"

void CentroidDecompositionFolklore::dfs_traversal(int node, int parent) {
    subtree[node] = 1;
    for (auto neighbor: graph[node]) {
        // skip if neighbor is already assigned a centroid or is parent
        if (centroid_parent[neighbor] != -1) continue;
        if (neighbor == parent) continue;
        dfs_traversal(neighbor, node);
        subtree[node] += subtree[neighbor];
    }
}

void CentroidDecompositionFolklore::decompose(int node, int parent, int current_size, int prev_centroid) {
    // find the centroid of the current component
    for (auto neighbor: graph[node]) {
        if (neighbor == parent) continue;
        if (centroid_parent[neighbor] != -1) continue;
        if ((subtree[neighbor] << 1) <= current_size) continue;
        decompose(neighbor, node, current_size, prev_centroid);
        return;
    }
    // centroid found
    centroid_parent[node] = prev_centroid;
    // recurse into each connected component formed by removing the centroid
    for (auto neighbor: graph[node]) {
        if (centroid_parent[neighbor] != -1) continue;
        dfs_traversal(neighbor, node);
        decompose(neighbor, node, subtree[neighbor], node);
    }
}

CentroidDecompositionFolklore::CentroidDecompositionFolklore(int n, const vector<vector<int>> &t): NUM_NODES(n), graph(t) {
    // start from an arbitrary node (e.g., node 1)
    centroid_parent.resize(NUM_NODES + 1, -1);
    subtree.resize(NUM_NODES + 1, 0);
    dfs_traversal(1, 0);
    decompose(1, 0, subtree[1], 0);
}

vector<int> CentroidDecompositionFolklore::get_centroid_decomposition() const {
    return centroid_parent;
}