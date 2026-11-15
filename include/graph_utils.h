#ifndef GRAPH_UTILS
#define GRAPH_UTILS

#include <vector>
#include <algorithm>

using namespace std;

struct GraphUtils {
    const int INF = 1e8 + 5;

    const vector<vector<int>>& graph;
    int NUM_NODES;

    vector<int> tin;
    vector<int> tout;
    vector<int> depth; // Necesario para calcular dist(u, v)
    int timer;

    vector<int> ans;
    vector<vector<int>> up;
    vector<int> centroid_parent;

    GraphUtils(int n, const vector<vector<int>>& g);

    void dfs(int u, int p);
    void build_lca_and_ancestor();
    bool is_ancestor(int u, int v);
    int lca(int u, int v);
    int dist(int u, int v);

    void build(const vector<int>& c_parent);
    void update(int u);
    int query(int u);
};

#endif