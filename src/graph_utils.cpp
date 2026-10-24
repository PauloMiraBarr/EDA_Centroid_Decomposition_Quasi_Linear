#include "graph_utils.h"

GraphUtils::GraphUtils(int n, const vector<vector<int>> &g): NUM_NODES(n), graph(g), timer(0) {}


void GraphUtils::dfs(int u, int p) {
    tin[u] = timer++;
    up[u][0] = p;
    for (int i = 1; i < 20; i++) up[u][i] = up[up[u][i-1]][i-1];
    for (auto v: graph[u]) if (v != p) dfs(v, u);
    tout[u] = timer++;
}

bool GraphUtils::is_ancestor(int u, int v) {
    return (tin[u] <= tin[v] and tout[u] >= tout[v]);
}

void GraphUtils::build_lca_and_ancestor() {
    tin.resize(NUM_NODES + 1);
    tout.resize(NUM_NODES + 1);
    up.resize(NUM_NODES + 1, vector<int>(20, 0));
    tin[0] = timer++;
    dfs(1, 0);
    tout[0] = timer++;
}

int GraphUtils::lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = 19; i >= 0; i--) {
        if (is_ancestor(up[u][i], v)) continue;
        u = up[u][i];
    } return up[u][0];
}