#include <iostream>
#include <vector>
#include "centroid_decomposition_folklore.h"
#include "centroid_decomposition_quasi_linear.h"
#include "graph_utils.h"

using namespace std;


void test_graph_utils() {
    vector<vector<int>> graph = {{}, {2, 3}, {1}, {1}};
    GraphUtils gu(3, graph);
    gu.build_lca_and_ancestor();
    cout << "1 & 2 lca: " << gu.lca(1, 2) << " should be 1\n";
    cout << "1 & 3 lca: " << gu.lca(1, 3) << " should be 1\n";
    cout << "3 & 2 lca: " << gu.lca(3, 2) << " should be 1\n";
}

void test_centroid_folklore_construction() {
    vector<vector<int>> graph = {{}, {2, 3}, {1}, {1}};
    CentroidDecompositionFolklore cdf(3, graph);
    vector<int> centroid_parent = cdf.get_centroid_decomposition();
    cout << "0: " << centroid_parent[0] << "\n";
    cout << "1: " << centroid_parent[1] << "\n";
    cout << "2: " << centroid_parent[2] << "\n";
    cout << "3: " << centroid_parent[3] << "\n";
}

void qlinear_print_alpha(vector<int> alpha, vector<int> betta, int N) {
    for (int i = 1; i <= N; i++) {
        if (alpha[i] == 0) continue;
        printf("Node [%2d]   Covered by [%2d]   Black node of cover [%2d]\n", i, alpha[i], betta[alpha[i]]);
    }
}

void qlinear_print_meta_tree(vector<vector<int>> g, vector<int> d) {
    printf(" -------------------- Condensed graph -------------------- \n");
    printf(" [INFO]  [From Node (u)] -> [To Node (v)]  ([Weight of component (u)])\n");
    for (int u = 0; u < g.size(); u++) {
        if (g[u].size() == 0) continue;
        for (auto v: g[u]) {
            printf("%2d -> %2d  (%2d)\n", u, v, d[u]);
        }
    }
}

void qlinear_print_centroid(vector<int> centroid_parent) {
    for (int i = 0; i < centroid_parent.size(); i++) {
        printf("[R] centroid parent of [%2d]  =>  [%2d]\n", i, centroid_parent[i]);
    }
}

void qlinear_print_middle_tree(vector<vector<int>> g) {
    for (int u = 0; u < g.size(); u++) {
        if (g[u].size() == 0) continue;
        for (auto v: g[u]) {
            printf("%2d -> %2d\n", u, v);
        }
    }
}

void qlinear_print_subtree(vector<int> g) {
    printf(" == [ Subtree size of cover ] ==\n");
    for (int i = 1; i < g.size(); i++) {
        if (g[i] == 0) continue;
        printf(" [%2d]  : %2d\n", i, g[i]);
    }
}


void test_centroid_quasi_linear_cover_and_construction() {
    printf(" ===== Graph v1 =====\n");
    // test graph
    vector<vector<int>> graph = {
        {},  
        {2, 9}, // 1 
        {1, 3}, // 2
        {2, 4}, // 3
        {3, 5, 6, 7}, // 4
        {4}, // 5
        {4}, // 6
        {4, 8}, // 7
        {7}, // 8
        {1, 10, 12, 18}, // 9
        {9, 11}, // 10
        {10}, // 11
        {9, 13, 14}, // 12  
        {12}, // 13
        {12, 15}, // 14
        {14, 16, 17}, // 15
        {15}, // 16
        {15}, // 17
        {9, 19, 20}, // 18 
        {18},  // 19
        {18} // 20
    };

    // Create centroid decomposition object
    CentroidDecompositionQLinear cdql(20, graph);
    
    // printf("[M] = %2d\n", cdql.component_size_bound);
    // qlinear_print_alpha(cdql.alpha, cdql.betta, cdql.NUM_NODES);
    // qlinear_print_middle_tree(cdql.middle_tree);
    qlinear_print_meta_tree(cdql.meta_tree, cdql.delta);
    qlinear_print_subtree(cdql.subtree);
    // qlinear_print_centroid(cdql.centroid_parent);
}

void test_centroid_quasi_linear_graph_v2() {
    printf(" ===== Graph v2 =====\n");
    // test graph
    vector<vector<int>> graph = {
        {},  
        {2, 9}, // 1 
        {1, 3}, // 2
        {2, 4}, // 3
        {3, 5, 6, 7}, // 4
        {4}, // 5
        {4}, // 6
        {4, 8}, // 7
        {7}, // 8
        {1, 10, 11, 17}, // 9
        {9}, // 10
        {9, 12, 13}, // 11
        {11}, // 12  
        {11, 14}, // 13
        {13, 15, 16}, // 14
        {14}, // 15
        {14}, // 16
        {9, 18, 19}, // 17
        {17}, // 18 
        {17},  // 19
    };

    // Create centroid decomposition object
    CentroidDecompositionQLinear cdql(19, graph);
    
    // printf("[M] = %2d\n", cdql.component_size_bound);
    // qlinear_print_alpha(cdql.alpha, cdql.betta, cdql.NUM_NODES);
    // qlinear_print_middle_tree(cdql.middle_tree);
    qlinear_print_meta_tree(cdql.meta_tree, cdql.delta);
    qlinear_print_subtree(cdql.subtree);
    // qlinear_print_centroid(cdql.centroid_parent);
}


int main () {

    // test_centroid_folklore_construction();
    // test_graph_utils();
    // test_centroid_quasi_linear_cover_and_construction();
    test_centroid_quasi_linear_graph_v2();

    return 0;
}