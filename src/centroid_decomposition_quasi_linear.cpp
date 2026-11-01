#include "centroid_decomposition_quasi_linear.h"

CentroidDecompositionQLinear::CentroidDecompositionQLinear(int n, const vector<vector<int>> &t): NUM_NODES(n), original_tree(t) {
    build_cover();
    centroid_construction();
}


/* ------------------------- section of building... ------------------------- */

void CentroidDecompositionQLinear::build_cover() {
    component_size_bound = log2(NUM_NODES);
    timer_node = NUM_NODES;
    alpha.resize(NUM_NODES + 1, 0);
    betta.resize(NUM_NODES + 1, 0);
    delta.resize(NUM_NODES + 1, 0);
    subtree.resize(NUM_NODES + 1, 0);
    middle_tree.resize(NUM_NODES + 1);
    meta_tree.resize(NUM_NODES + 1);

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
    dfs_build_meta_tree(alpha[1], 0);
}


int CentroidDecompositionQLinear::dfs_root_cover_assign(int u, int p) {
    // black node of u is initially u
    betta[u] = u;
    int root_pcs = 1;
    vector<pair<int, int>> child_pcs;
    for (int v: original_tree[u]) {
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
                for (int w: group) alpha[w] = red;
                for (i = i + 1; i < (int)child_pcs.size(); i++) alpha[child_pcs[i].first] = red;
            }
            // k == 1: nothing is shared, all of it stays in u's own subtree
            break;
        }
        shared_root = true;
        int red = cover_add_node();
        betta[red] = u;
        alpha[red] = red;
        for (int w: group) alpha[w] = red;
        consumed += current_pcs - 1;
        current_pcs = 1;
        group.clear();
    }

    return 0;
}

void CentroidDecompositionQLinear::dfs_extend_root_cover_and_middle_tree(int u, int p, int cover_id) {
    if (alpha[u] == 0) alpha[u] = cover_id;
    for (int v: original_tree[u]) {
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

void CentroidDecompositionQLinear::dfs_build_meta_tree(int u, int p) {
    delta[alpha[u]] += (betta[u] == u ? 1 : 0);
    for (int v: middle_tree[u]) {
        if (v == p) continue;
        dfs_build_meta_tree(v, u);
        if (alpha[v] != alpha[u]) {
            meta_tree[alpha[u]].push_back(alpha[v]);
            meta_tree[alpha[v]].push_back(alpha[u]);
        }
    }
}

/* super hidden functions */

int CentroidDecompositionQLinear::cover_add_node() {
    alpha.emplace_back(0);
    betta.emplace_back(0);
    delta.emplace_back(0);
    subtree.emplace_back(0);
    meta_tree.emplace_back();
    middle_tree.emplace_back();
    return ++timer_node;
}


/* ------------------------- Section of decomposition... ------------------------- */

void CentroidDecompositionQLinear::centroid_construction() {
    centroid_parent.resize(NUM_NODES + 1, -1);
    dfs_calculate_subtree(alpha[1], 0);
    dfs_search_centroid(alpha[1], 0, NUM_NODES, 0);
}

void CentroidDecompositionQLinear::dfs_calculate_subtree(int u, int p) {
    subtree[u] = delta[u];
    for (int v: meta_tree[u]) {
        if (v == p) continue;
        if (centroid_parent[betta[v]] != -1) continue;
        dfs_calculate_subtree(v, u);
        subtree[u] += subtree[v];
    }
}

void CentroidDecompositionQLinear::dfs_search_centroid(int u, int p, int current_size, int last_centroid) {
    for (int v: meta_tree[u]) {
        if (v == p) continue;
        if (centroid_parent[betta[v]] != -1) continue;
        if ((subtree[v] << 1) <= current_size) continue;
        dfs_search_centroid(v, u, current_size, last_centroid);
        return;
    }
    
    subtree[p] = current_size - subtree[u];
    // search centroid into cover
    subtree_into_cover(u, 0);
    int centroid = search_into_cover(u, 0, current_size);
    centroid_parent[betta[centroid]] = last_centroid;

    printf(" [centroid founded] [Red: %2d] [Black :%2d]\n", centroid, betta[centroid]);
    

    // get balck and red nodes
    vector<int> removed = {betta[centroid]};
    for (int r: middle_tree[betta[centroid]]) {
        if (betta[r] == betta[centroid]) {
            removed.push_back(r);
        }
    }

    // quitar las conexiones hacia centroide de los vecinos en el meta tree (quitar desde el destino)
    for (int x: removed) {
        for (int w: middle_tree[x]) {
            if (centroid_parent[betta[w]] != -1) continue;
            if (alpha[w] == alpha[x]) continue;
            auto &tree = meta_tree[alpha[w]];
            tree.erase(remove(tree.begin(), tree.end(), alpha[x]), tree.end());
        }
    }

    // como aun tenemos las conexiones desde el origen, vamos a hacer el split de ello
    vector<int> entires;
    for (int x: removed) 
        for (int w: middle_tree[x]) 
            if (centroid_parent[betta[w]] == -1 and alpha[w] == alpha[x]) 
                entires.push_back(w);
    for (int w: entires) split_meta_tree(w, 0);

    // ahora tenemos los puntos de entrada de cada componente
    vector<int> subroots;
    for (int x: removed) 
        for (int w: middle_tree[x])
            if (centroid_parent[betta[w]] == -1)
                subroots.push_back(alpha[w]);
    
    // recursion sobre cada componente
    for (int r: subroots) {
        dfs_calculate_subtree(r, 0);
        dfs_search_centroid(r, 0, subtree[r], betta[centroid]);
    }
}

/* super hidden functions */

void CentroidDecompositionQLinear::split_meta_tree(int u, int p) {
    delta[u] = (betta[u] == u ? 1 : 0);
    meta_tree[u].clear();
    for (int v: middle_tree[u]) {
        if (v == p) continue;
        if (centroid_parent[betta[v]] != -1) continue;
        if (alpha[u] == alpha[v]) {
            split_meta_tree(v, u);
            meta_tree[u].push_back(v);
            meta_tree[v].push_back(u);
        } else {
            auto &tree = meta_tree[alpha[v]];
            replace(tree.begin(), tree.end(), alpha[u], u);
            meta_tree[u].push_back(alpha[v]);
        }
    }
    alpha[u] = u;
}

int CentroidDecompositionQLinear::search_into_cover(int u, int p, int current_size) {
    for (int v: middle_tree[u]) {
        if (v == p) continue;
        if (centroid_parent[betta[v]] != -1) continue;
        if (alpha[u] != alpha[v]) continue;
        if ((subtree[v] << 1) <= current_size) continue;
        return search_into_cover(v, u, current_size);
    }
    return u;
}

void CentroidDecompositionQLinear::subtree_into_cover(int u, int p) {
    subtree[u] = (betta[u] == u ? 1 : 0);
    for (int v: middle_tree[u]) {
        if (v == p) continue;
        if (centroid_parent[betta[v]] != -1) continue;
        if (alpha[u] == alpha[v]) {
            subtree_into_cover(v, u);
            subtree[u] += subtree[v];
        } else {
            subtree[u] += subtree[alpha[v]];
        }
    }
}