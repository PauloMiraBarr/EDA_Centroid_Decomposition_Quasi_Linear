#ifndef CENTROID_DECOMPOSITION_QUASI_LINEAR
#define CENTROID_DECOMPOSITION_QUASI_LINEAR

#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>
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
    vector<vector<pair<int, int>>> meta_tree; // meta_tree
    

    /* -------------------- methods -------------------- */
    /* Constructor */
    CentroidDecompositionQLinear(int n, const vector<vector<int>> &t);
    
    /* Building cover */
    void build_cover();
    int dfs_root_cover_assign(int u, int p);
    void dfs_extend_root_cover(int u, int p, int cover);

    /* Processing cover */
    void centroid_construction();

};

#endif