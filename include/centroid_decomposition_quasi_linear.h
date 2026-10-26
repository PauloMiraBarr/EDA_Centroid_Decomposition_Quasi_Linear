#ifndef CENTROID_DECOMPOSITION_QUASI_LINEAR
#define CENTROID_DECOMPOSITION_QUASI_LINEAR

#include <math.h>
#include <stdio.h>
#include <stack>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;


struct CentroidDecompositionQLinear {

    /* classic parameters */
    int NUM_NODES;
    vector<int> centroid_parent;
    const vector<vector<int>> &original_tree;


    /* tree covering parameters */
    int timer_node;
    int component_size_bound;
    vector<int> alpha; // root id of the component/cover containing the node
    vector<int> betta; // black node (actual node of a red node) for the cover
    vector<vector<pair<int, int>>> meta_tree; // adjacency list of the meta-tree: node -> {(neighbor, weight), ...}
    
    /* processing cover parameters */
    vector<vector<int>> middle_tree;
    vector<int> subtree;

    /* -------------------- methods -------------------- */
    /* Constructor */
    CentroidDecompositionQLinear(int n, const vector<vector<int>> &t);
    // only main routines are called from the constructor

    /* ------- building cover section ------- */
    // main function
    void build_cover();
    // auxiliary functions
    int cover_add_node();
    int dfs_root_cover_assign(int u, int p);
    void dfs_extend_root_cover_and_middle_tree(int u, int p, int cover);
    int dfs_build_meta_tree(int u, int p);

    /* ------- processing cover section ------- */
    // main function
    void centroid_construction();
    // auxiliary functions
    

    /* ------- other methods of folklore ------- */
    vector<int> get_centroid_decomposition() const;

};

#endif