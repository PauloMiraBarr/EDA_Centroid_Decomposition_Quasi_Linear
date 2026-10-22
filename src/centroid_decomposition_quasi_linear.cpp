#include "centroid_decomposition_quasi_linear.h"

CentroidDecompositionQLinear::CentroidDecompositionQLinear(int n, const vector<vector<int>>& t): NRO_NODES(n + 1), graph(t) {
    centroid_parent.resize(NRO_NODES, 0);

    /* here calls routines */
    build_cover();
    centroid_construction();
}

void CentroidDecompositionQLinear::build_cover() {
    /* start cover information data */
    component_size_bound = log2(NRO_NODES - 1);
    alpha.resize(2 * NRO_NODES, 0);
    betta.resize(2 * NRO_NODES, 0);
    meta_tree.resize(2 * NRO_NODES);
    timer_node = NRO_NODES;
    /* subrutines for cover */
    // routine 1: start with cover root's
    int pcs_root = dfs_root_cover_assign(1, 0);
    if (pcs_root != 0) alpha[1] = 1;
    // routine 2: extend the cover of to root's to thier childs
    dfs_extend_root_cover(1, 0, alpha[1]);
    // routine 3: condense that into a meta tree
    dfs_condense_cover(1, 0);
}

void CentroidDecompositionQLinear::dfs_extend_root_cover(int u, int p, int cover) {
    if (alpha[u] == 0) alpha[u] = cover;
    for (auto v: graph[u]) {
        if (v == p) continue;
        if (alpha[v] != 0) dfs_extend_root_cover(v, u, alpha[v]);
        else dfs_extend_root_cover(v, u, alpha[u]);
    }
}

int CentroidDecompositionQLinear::dfs_condense_cover(int u, int p) {
    int subt_u = (betta[u] == u ? 1 : 0);
    int last_cover = 0;
    int carry_subt_v = 0;
    bool need_to_add = false;
    for (auto v: graph[u]) {
        if (v == p) continue;
        int subt_v = dfs_condense_cover(v, u);
        subt_u += subt_v;
        // two nodes of different cover
        if (alpha[u] != alpha[v]) {
            // case 1: is the first cover we met
            if (last_cover == 0) {
                last_cover = alpha[v];
                carry_subt_v = subt_v;
                need_to_add = true;
            }
            // case 2: we get the last cover again
            else if (alpha[v] == last_cover) {
                carry_subt_v += subt_v;
            }
            // case 3: is not the same last cover
            else if (alpha[v] != last_cover) {
                // update last_cover
                meta_tree[alpha[u]].push_back({last_cover, carry_subt_v});
                meta_tree[last_cover].push_back({alpha[u], (NRO_NODES - 1) - carry_subt_v});
                last_cover = alpha[v];
                carry_subt_v = subt_v;
            }
        }
    }
    // update last cover visited
    if (need_to_add) {
        meta_tree[alpha[u]].push_back({last_cover, carry_subt_v});
        meta_tree[last_cover].push_back({alpha[u], (NRO_NODES - 1) - carry_subt_v});
    }
    return subt_u;
}

int CentroidDecompositionQLinear::dfs_root_cover_assign(int u, int p) {
    // back node of u is u
    betta[u] = u;
    int root_pcs = 1;
    vector<pair<int, int>> child_pcs;
    
    for (auto v: graph[u]) {
        if (v == p) continue;
        int pcs_v = dfs_root_cover_assign(v, u);
        if (pcs_v == 0) continue;
        root_pcs += pcs_v;
        child_pcs.push_back({v, pcs_v});
    }

    // not enough to be a cover
    if (root_pcs < component_size_bound) return root_pcs;

    // could be one or more covers
    int current_pcs = 1;
    vector<int> current_cover_nodes;
    bool two_or_more = false;

    // root is always cover by himself
    alpha[u] = u;

    for (int i = 0; i < child_pcs.size(); i++) {
        auto [v, pcs_v] = child_pcs[i];
        current_pcs += pcs_v;
        current_cover_nodes.push_back(v);
        if (current_pcs >= component_size_bound - 1) {
            int remanent_pcs = root_pcs - current_pcs;
            if (remanent_pcs < component_size_bound - 1) {
                if (two_or_more == true) {
                    int pseudo_root = timer_node++;
                    // real black node of red node pseudo root
                    betta[pseudo_root] = u;
                    for (auto w: current_cover_nodes) alpha[w] = pseudo_root;
                    for (; i < child_pcs.size(); i++) alpha[child_pcs[i].first] = pseudo_root;
                }
                // otherwise node u is currently covering it
                break;
            } else {
                two_or_more = true;
                int pseudo_root = timer_node++;
                // real black node of red pseudo root
                betta[pseudo_root] = u;
                for (auto w: current_cover_nodes) alpha[w] = pseudo_root;
                current_pcs = 1;
                current_cover_nodes.clear();
            }
        }
    }
    
    // cover in some way
    return 0;
}

void CentroidDecompositionQLinear::centroid_construction() {
    /* split of cover to build centroid decomposition */
}

