#ifndef CENTROID_DECOMPOSITION_FOLKLORE
#define CENTROID_DECOMPOSITION_FOLKLORE

#include <vector>
#include <algorithm>
using namespace std;

struct CentroidDecompositionFolklore {

    int NUM_NODES;
    const vector<vector<int>> &graph;
    vector<int> centroid_parent; // parent of node u in the centroid tree
    vector<int> subtree; // size of subtree rooted at node u in the current component


    void dfs_traversal(int node, int parent);

    void decompose(int node, int parent, int current_size, int prev_centroid);

    CentroidDecompositionFolklore(int n, const vector<vector<int>> &t);

    vector<int> get_centroid_decomposition() const;

};

#endif