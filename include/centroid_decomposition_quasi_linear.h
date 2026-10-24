#ifndef CENTROID_DECOMPOSITION_QUASI_LINEAR
#define CENTROID_DECOMPOSITION_QUASI_LINEAR

#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>
#include <stdio.h>
using namespace std;


struct CentroidDecompositionQLinear {

    /* classic parameters */
    int NUM_NODES;
    vector<int> centroid_parent;
    const vector<vector<int>> &graph;



    /* tree covering parameters */
    int timer_node;
    int component_size_bound;
    vector<int> alpha; // root id of the component/cover containing the node
    vector<int> betta; // black node (cover root representative) for the cover
    vector<vector<pair<int, int>>> meta_tree; // adjacency list of the meta-tree: node -> {(neighbor, weight), ...}
    

    /* -------------------- methods -------------------- */
    /* Constructor */
    CentroidDecompositionQLinear(int n, const vector<vector<int>> &t);
    // only main routines are called from the constructor

    /* ------- building cover section ------- */
    // main function
    void build_cover();
    // auxiliary function
    int dfs_root_cover_assign(int u, int p);
    void dfs_extend_root_cover(int u, int p, int cover);
    // condense cover and convert it into a meta-tree
    int dfs_condense_cover(int u, int p);

    /* ------- processing cover section ------- */
    // main function
    void centroid_construction();
    // auxiliary functions
    void search_cover_of_centroid(int u, int p, const int size, const int last_centroid);
    int search_centroid_into_cover(int cover_id);

    /* ------- other classic methods ------- */
    vector<int> get_centroid_decomposition() const;

};

#endif