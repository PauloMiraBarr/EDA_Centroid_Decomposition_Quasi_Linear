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

void test_centroid_quasi_linear_cover_and_construction() {
    /* Note:
     * Either the algorithm proposed for the cover in the original paper,
     * or the authors' internal interpretation was wrong.
     * In any case, the pseudocode does not reflect what the authors drew.
     * Nor does the proof satisfy the conditions stated in the pseudocode.
     * For this reason, we follow the pseudocode strictly, not the proof
     * or the authors' interpretations.
     * */

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
    
    /*
    for (int i = 1; i <= 20; i++) {
        if (cdql.alpha[i] == 0) continue;
        printf("Node [%2d]   Covered by [%2d]   Black node of cover [%2d]\n", i, cdql.alpha[i], cdql.betta[cdql.alpha[i]]);
    }

    // Result of cover and meta-tree
    printf(" -------------------- Condensed graph -------------------- \n");
    printf(" [INFO]  [From Node] -> [To Node]  ([Weight of connection])\n");
    vector<vector<pair<int, int>>> cg = cdql.meta_tree;
    for (int u = 0; u < cg.size(); u++) {
        if (cg[u].size() == 0) continue;
        for (auto [v, w]: cg[u]) {
            printf("%2d -> %2d  (%2d)\n", u, v, w);
        }
    }
    // ------------------------- 
    // */

    /*
    vector<int> cdql_centroid = cdql.get_centroid_decomposition();
    for (int i = 0; i < cdql_centroid.size(); i++) {
        printf("[R] centroid parent of [%2d]  =>  [%2d]\n", i, cdql_centroid[i]);
    }
    // */
}

void test_centroid_quasi_linear_graph_v2() {
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
    
    /*  
    // Internal state debugging --------------------------
    for (int i = 1; i <= 19; i++) {
        if (cdql.alpha[i] == 0) continue;
        printf("Node [%2d]   Covered by [%2d]   Black node of cover [%2d]\n", i, cdql.alpha[i], cdql.betta[cdql.alpha[i]]);
    }

    // Result of cover and meta-tree
    printf(" -------------------- Condensed graph -------------------- \n");
    printf(" [INFO]  [From Node] -> [To Node]  ([Weight of connection])\n");
    vector<vector<pair<int, int>>> cg = cdql.meta_tree;
    for (int u = 0; u < cg.size(); u++) {
        if (cg[u].size() == 0) continue;
        for (auto [v, w]: cg[u]) {
            printf("%2d -> %2d  (%2d)\n", u, v, w);
        }
    }
    // ------------------------- 
    // */

    vector<int> cdql_centroid = cdql.get_centroid_decomposition();
    /*
    for (int i = 0; i < cdql_centroid.size(); i++) {
        printf("[R] centroid parent of [%2d]  =>  [%2d]\n", i, cdql_centroid[i]);
    }
    // */
}


int main () {

    // test_centroid_folklore_construction();
    // test_graph_utils();
    test_centroid_quasi_linear_cover_and_construction();
    // test_centroid_quasi_linear_graph_v2();

    return 0;
}