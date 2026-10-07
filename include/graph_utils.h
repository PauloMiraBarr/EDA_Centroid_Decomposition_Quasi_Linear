#ifndef GRAPH_UTILS
#define GRAPH_UTILS

#include <vector>
using namespace std;

struct GraphUtils {

    const vector<vector<int>>& graph;
    int NRO_NODES;
    
    vector<int> tin;
    vector<int> tout;
    int timer;

    vector<vector<int>> spg;

    GraphUtils(int n, const vector<vector<int>> &g);

    void dfs(int u, int p);
    void build_lca_and_ancestor();
    bool is_ancestor(int u, int v);
    int lca(int u, int v);

};

#endif