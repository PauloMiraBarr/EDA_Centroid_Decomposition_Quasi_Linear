#ifndef CENTROID_DECOMPOSITION_FOLKLORE
#define CENTROID_DECOMPOSITION_FOLKLORE

#include <vector>
#include <algorithm>
using namespace std;

struct CentroidDecompositionFolklore {

    int NRO_NODES;
    const vector<vector<int>> &graph;
    vector<int> cparent; // centroid parent for a given node u
    vector<int> subtree; // subtree size for a given node u


    void dfs_traversal(int node, int parent);

    void decompose(int node, int parent, int current_size, int prev_centroid);

    CentroidDecompositionFolklore(int n, const vector<vector<int>> &t);

    vector<int> get_centroid_decomposition() const;

};

#endif