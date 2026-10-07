#include "graph_utils.h"

GraphUtils::GraphUtils(int n, const vector<vector<int>> &g): NRO_NODES(n + 1), graph(g) {}


void GraphUtils::dfs(int u, int p) {
    GraphUtils::tin[u] = timer++;
    spg[u][0] = p;
    for (int i = 1; i < 20; i++) spg[u][i] = spg[spg[u][i-1]][i-1];
    for (auto v: graph[u]) if (v != p) dfs(v, u);
    GraphUtils::tout[u] = timer++;
}

bool GraphUtils::is_ancestor(int u, int v) {
    return (tin[u] <= tin[v] and tout[u] >= tout[v]);
}

void GraphUtils::build_lca_and_ancestor() {
    tin.resize(NRO_NODES);
    tout.resize(NRO_NODES);
    spg.resize(NRO_NODES, vector<int>(20, 0));
    tin[0] = timer++;
    dfs(1, 0);
    tout[0] = timer++;
}

int GraphUtils::lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = 19; i >= 0; i--) {
        if (is_ancestor(spg[u][i], v)) continue;
        u = spg[u][i];
    } return spg[u][0];
}