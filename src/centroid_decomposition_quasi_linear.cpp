#include "centroid_decomposition_quasi_linear.h"

CentroidDecompositionQLinear::CentroidDecompositionQLinear(int n, const vector<vector<int>> &t): NUM_NODES(n), original_tree(t) {
    build_cover();
    // centroid_construction();
}

void CentroidDecompositionQLinear::build_cover() {
    component_size_bound = log2(NUM_NODES);
    timer_node = NUM_NODES;
    alpha.resize(NUM_NODES + 1, 0);
    betta.resize(NUM_NODES + 1, 0);
    meta_tree.resize(NUM_NODES + 1);
    middle_tree.resize(NUM_NODES + 1);

    /* ----- auxiliary functions -----*/
    // step 1: assign cover roots
    int pcs_root = dfs_root_cover_assign(1, 0);
    if (pcs_root != 0) alpha[1] = 1;
    // step 2: create middle tre and extend cover
    dfs_extend_root_cover_and_middle_tree(1, 0, alpha[1]);
    for (int i = NUM_NODES + 1; i <= timer_node; ++i) {
        middle_tree[i].push_back(betta[i]);
        middle_tree[betta[i]].push_back(i);
    }
    // step 3: create meta tree
    dfs_build_meta_tree(1, 0);
}


int CentroidDecompositionQLinear::dfs_root_cover_assign(int u, int p) {
    // black node of u is initially u
    betta[u] = u;
    int root_pcs = 1;
    vector<pair<int, int>> child_pcs;
    for (auto v: original_tree[u]) {
        if (v == p) continue;
        int pcs_v = dfs_root_cover_assign(v, u);
        if (pcs_v == 0) continue;
        root_pcs += pcs_v;
        child_pcs.push_back({v, pcs_v});
    }

    // subtree size too small to form a cover
    if (root_pcs < component_size_bound) return root_pcs;

    // try to form one or more covers
    int current_pcs = 1;
    int consumed = 0;
    bool shared_root = false;
    vector<int> group;

    // assign node u as a cover root
    alpha[u] = u;

    for (int i = 0; i < (int)child_pcs.size(); i++) {
        auto [v, pcs_v] = child_pcs[i];
        current_pcs += pcs_v;
        group.push_back(v);
        if (current_pcs < component_size_bound - 1) continue;

        int remaining_pcs = root_pcs - consumed - current_pcs;
        if (remaining_pcs < component_size_bound - 1) {
            // this is the last group: it absorbs the leftover children
            if (shared_root) {
                int red = cover_add_node();
                betta[red] = u;
                alpha[red] = red;
                for (auto w: group) alpha[w] = red;
                for (i = i + 1; i < (int)child_pcs.size(); i++) alpha[child_pcs[i].first] = red;
            }
            // k == 1: nothing is shared, all of it stays in u's own subtree
            break;
        }
        shared_root = true;
        int red = cover_add_node();
        betta[red] = u;
        alpha[red] = red;
        for (auto w: group) alpha[w] = red;
        consumed += current_pcs - 1;
        current_pcs = 1;
        group.clear();
    }

    return 0;
}

void CentroidDecompositionQLinear::dfs_extend_root_cover_and_middle_tree(int u, int p, int cover_id) {
    if (alpha[u] == 0) alpha[u] = cover_id;
    for (auto v: original_tree[u]) {
        if (v == p) continue;
        bool red_connection = false;
        if (alpha[v] != 0) {
            dfs_extend_root_cover_and_middle_tree(v, u, alpha[v]);
            if (betta[alpha[v]] != v) red_connection = true;
        }
        else dfs_extend_root_cover_and_middle_tree(v, u, alpha[u]);
        if (red_connection) {
            middle_tree[v].push_back(alpha[v]);
            middle_tree[alpha[v]].push_back(v);
        } else {
            middle_tree[u].push_back(v);
            middle_tree[v].push_back(u);
        }
    }
}

int CentroidDecompositionQLinear::dfs_build_meta_tree(int u, int p) {
    int subt_u = (betta[u] == u ? 1 : 0);
    for (auto v: middle_tree[u]) {
        if (v == p) continue;
        int subt_v = dfs_build_meta_tree(v, u);
        if (alpha[v] != alpha[u]) {
            meta_tree[u].push_back({v, subt_v});
            meta_tree[v].push_back({u, NUM_NODES - subt_v});
        }
        subt_u += subt_v;
    }
    return subt_u;
}

/* super hidden functions */

int CentroidDecompositionQLinear::cover_add_node() {
    alpha.emplace_back(0);
    betta.emplace_back(0);
    meta_tree.emplace_back();
    middle_tree.emplace_back();
    return ++timer_node;
}