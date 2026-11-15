#include "graph_utils.h"

GraphUtils::GraphUtils(int n, const vector<vector<int>>& g)
    : NUM_NODES(n), graph(g), timer(0) {}

void GraphUtils::dfs(int u, int p) {
    depth[u] = depth[p] + 1;
    tin[u] = timer++;
    up[u][0] = p;
    for (int i = 1; i < 20; i++) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    for (auto v : graph[u]) {
        if (v != p) dfs(v, u);
    }
    tout[u] = timer++;
}

bool GraphUtils::is_ancestor(int u, int v) {
    return (tin[u] <= tin[v] && tout[u] >= tout[v]);
}

void GraphUtils::build_lca_and_ancestor() {
    tin.resize(NUM_NODES + 1);
    tout.resize(NUM_NODES + 1);
    depth.assign(NUM_NODES + 1, 0);
    up.assign(NUM_NODES + 1, vector<int>(20, 0));

    tin[0] = timer++;
    dfs(1, 0);
    tout[0] = timer++;
}

int GraphUtils::lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = 19; i >= 0; i--) {
        if (!is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    return up[u][0];
}

int GraphUtils::dist(int u, int v) {
    int p = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[p];
}

void GraphUtils::build(const vector<int>& c_parent) {
    centroid_parent = c_parent;
    ans.assign(NUM_NODES + 1, INF);
}

void GraphUtils::update(int u) {
    int curr = u;
    while (curr != 0) {
        ans[curr] = min(ans[curr], dist(u, curr));
        curr = centroid_parent[curr];
    }
}

int GraphUtils::query(int u) {
    int res = INF;
    int curr = u;
    while (curr != 0) {
        res = min(res, dist(u, curr) + ans[curr]);
        curr = centroid_parent[curr];
    }
    return res;
}