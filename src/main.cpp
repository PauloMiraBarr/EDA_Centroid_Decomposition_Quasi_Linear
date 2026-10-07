#include <iostream>
#include <vector>
#include "centroid_decomposition.h"
#include "graph_utils.h"

using namespace std;


int main () {

    vector<vector<int>> graph = {{}, {2, 3}, {1}, {1}};

    CentroidDecompositionFolklore cdf(3, graph);
    vector<int> centroid_parent = cdf.get_centroid_decomposition();

    GraphUtils gu(3, graph);

    cout << "0: " << centroid_parent[0] << "\n";
    cout << "1: " << centroid_parent[1] << "\n";
    cout << "2: " << centroid_parent[2] << "\n";
    cout << "3: " << centroid_parent[3] << "\n";

    gu.build_lca_and_ancestor();
    cout << "1 & 2 lca: " << gu.lca(1, 2) << " should be 1\n";
    cout << "1 & 3 lca: " << gu.lca(1, 3) << " should be 1\n";
    cout << "3 & 2 lca: " << gu.lca(3, 2) << " should be 1\n";


    return 0;
}