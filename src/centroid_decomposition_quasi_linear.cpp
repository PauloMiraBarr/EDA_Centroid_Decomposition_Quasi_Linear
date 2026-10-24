#include "centroid_decomposition_quasi_linear.h"

CentroidDecompositionQLinear::CentroidDecompositionQLinear(int n, const vector<vector<int>>& t): NUM_NODES(n), graph(t) {
    centroid_parent.resize(NUM_NODES + 1, -1);

    // call main routines
    build_cover();
    centroid_construction();
}

void CentroidDecompositionQLinear::build_cover() {
    // initialize cover data structures
    component_size_bound = log2(NUM_NODES);
    alpha.resize(2 * NUM_NODES + 1, 0);
    betta.resize(2 * NUM_NODES + 1, 0);
    meta_tree.resize(2 * NUM_NODES + 1);
    timer_node = NUM_NODES + 1;
    // cover construction routines
    // step 1: assign initial cover roots
    int pcs_root = dfs_root_cover_assign(1, 0);
    if (pcs_root != 0) alpha[1] = 1;
    // step 2: extend each cover to its children
    dfs_extend_root_cover(1, 0, alpha[1]);
    // step 3: condense covers into the meta-tree
    dfs_condense_cover(1, 0);
}


/* Section: Building cover ---------------------------------- */

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
        // two nodes in different covers
        if (alpha[u] != alpha[v]) {
            // first cover encountered
            if (last_cover == 0) {
                last_cover = alpha[v];
                carry_subt_v = subt_v;
                need_to_add = true;
            }
            // same cover as last encountered
            else if (alpha[v] == last_cover) {
                carry_subt_v += subt_v;
            }
            // different cover from last encountered
            else if (alpha[v] != last_cover) {
                // update adjacency between covers
                meta_tree[alpha[u]].push_back({last_cover, carry_subt_v});
                meta_tree[last_cover].push_back({alpha[u], NUM_NODES - carry_subt_v});
                last_cover = alpha[v];
                carry_subt_v = subt_v;
            }
        }
    }
    // add edge for the last cover encountered
    if (need_to_add) {
        meta_tree[alpha[u]].push_back({last_cover, carry_subt_v});
        meta_tree[last_cover].push_back({alpha[u], NUM_NODES - carry_subt_v});
    }
    return subt_u;
}

int CentroidDecompositionQLinear::dfs_root_cover_assign(int u, int p) {
    // black node of u is initially u
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

    // subtree size too small to form a cover
    if (root_pcs < component_size_bound) return root_pcs;

    // try to form one or more covers
    int current_pcs = 1;
    vector<int> current_cover_nodes;
    bool two_or_more = false;

    // assign node u as a cover root
    alpha[u] = u;

    for (int i = 0; i < child_pcs.size(); i++) {
        auto [v, pcs_v] = child_pcs[i];
        current_pcs += pcs_v;
        current_cover_nodes.push_back(v);
        if (current_pcs >= component_size_bound - 1) {
            int remaining_pcs = root_pcs - current_pcs;
            if (remaining_pcs < component_size_bound - 1) {
                if (two_or_more == true) {
                    int pseudo_root = timer_node++;
                    // set black node of pseudo-root cover to u
                    betta[pseudo_root] = u;
                    for (auto w: current_cover_nodes) alpha[w] = pseudo_root;
                    for (; i < child_pcs.size(); i++) alpha[child_pcs[i].first] = pseudo_root;
                }
                // remaining nodes remain covered by u
                break;
            } else {
                two_or_more = true;
                int pseudo_root = timer_node++;
                // set black node of pseudo-root cover to u
                betta[pseudo_root] = u;
                for (auto w: current_cover_nodes) alpha[w] = pseudo_root;
                current_pcs = 1;
                current_cover_nodes.clear();
            }
        }
    }
    
    // nodes already assigned to existing covers
    return 0;
}


/* Section: Processing cover ---------------------------------- */

void CentroidDecompositionQLinear::search_cover_of_centroid(int u, int p, const int size, const int last_centroid) {
    for (auto [v, subt_v]: meta_tree[u]) {
        if (v == p) continue;
        if (subt_v <= size / 2) continue;
        search_cover_of_centroid(v, u, size, last_centroid);
        return;
    }

    printf("Cover of centroid [%2d]   &  Last centroid [%2d]\n", u, last_centroid);

    int centroid = search_centroid_into_cover(u);

    printf(" [->] Centroid of cover [%2d]\n", centroid);

    // TODO: declare and implement split_and_delete_centroid_of_cover in header/cpp
}

int CentroidDecompositionQLinear::search_centroid_into_cover(int cover_id) {
    // find centroid inside the given cover
    return cover_id;
}

void CentroidDecompositionQLinear::centroid_construction() {
    // start centroid construction from root cover
    search_cover_of_centroid(alpha[1], 0, NUM_NODES, 0);
}

vector<int> CentroidDecompositionQLinear::get_centroid_decomposition() const {
    return centroid_parent;
}
