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
    int NRO_NODES;
    vector<int> centroid_parent;
    const vector<vector<int>> &graph;



    /* tree covering parameters */
    int timer_node; // [done]
    int component_size_bound; // [done]
    vector<int> alpha; // component's root [done]
    vector<int> betta; // real black node [done]
    vector<vector<pair<int, int>>> meta_tree; // meta_tree's form [node : {node, weight}, ...]
    

    /* -------------------- methods -------------------- */
    /* Constructor */
    CentroidDecompositionQLinear(int n, const vector<vector<int>> &t);
    /* Only main functions are called in constructor */

    /* ------- building cover section ------- */
    // main function
    void build_cover();
    // auxiliar function
    int dfs_root_cover_assign(int u, int p);
    void dfs_extend_root_cover(int u, int p, int cover);
    // condense cover and convert it into a meta tree
    int dfs_condense_cover(int u, int p);

    /* ------- Processing cover section ------- */
    // main function
    void centroid_construction();

};

#endif