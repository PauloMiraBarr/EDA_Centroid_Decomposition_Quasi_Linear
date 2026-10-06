#include <iostream>
#include <vector>
#include "centroid_decomposition.h"

using namespace std;


int main () {

    vector<vector<int>> graph = {{}, {2, 3}, {1}, {1}};

    CentroidDecompositionFolklore cdf(3, graph);
    vector<int> centroid_parent = cdf.get_centroid_decomposition();

    cout << "0: " << centroid_parent[0] << "\n";
    cout << "1: " << centroid_parent[1] << "\n";
    cout << "2: " << centroid_parent[2] << "\n";
    cout << "3: " << centroid_parent[3] << "\n";


    return 0;
}