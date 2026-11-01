#ifndef CENTROID_DECOMPOSITION_QUASI_LINEAR
#define CENTROID_DECOMPOSITION_QUASI_LINEAR

#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;


struct CentroidDecompositionQLinear {

    /* classic parameters */
    int NUM_NODES;
    vector<int> centroid_parent;
    const vector<vector<int>> &original_tree;


    /* new algorithm parameters */
    int timer_node;
    int component_size_bound;
    vector<int> alpha; // root id of the component/cover containing the node
    vector<int> betta; // black node (actual node of a red node) for the cover
    vector<int> delta; // size of component/cover
    vector<int> subtree; // size uf subtree's cover
    vector<vector<int>> middle_tree;
    vector<vector<int>> meta_tree; // adjacency list of the meta-tree: node -> {(neighbor, weight), ...}

    /* -------------------- methods -------------------- */
    /* Constructor */
    CentroidDecompositionQLinear(int n, const vector<vector<int>> &t);
    // only main routines are called from the constructor

    /* ------- building cover section ------- */
    // Main function
    void build_cover();
    // auxiliary functions
    // * hidden functions
    int cover_add_node();
    // * main functions
    int dfs_root_cover_assign(int u, int p);
    void dfs_extend_root_cover_and_middle_tree(int u, int p, int cover);
    void dfs_build_meta_tree(int u, int p);

    /* ------- processing cover section ------- */
    // Main function
    void centroid_construction();
    // auxiliary functions
    // * hidden functions
    void subtree_into_cover(int u, int p);
    int search_into_cover(int u, int p, int current_size);
    void split_meta_tree(int u, int p);
    // * main functions
    void dfs_calculate_subtree(int u, int p);
    void dfs_search_centroid(int u, int p, int current_size, int last_centroid);
    

    /* ------- other methods of folklore ------- */
    vector<int> get_centroid_decomposition() const;

};

#endif