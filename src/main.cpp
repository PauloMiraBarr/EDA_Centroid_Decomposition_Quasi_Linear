#include <iostream>
#include <vector>
#include "centroid_decomposition.h"

using namespace std;


int main () {

    vector<vector<int>> graph = {{}, {2, 3}, {1}, {1}};

    CentroidDecompositionFolklore cdf(3, graph);

    cout << "0: " << cdf.cparent[0] << "\n";
    cout << "1: " << cdf.cparent[1] << "\n";
    cout << "2: " << cdf.cparent[2] << "\n";
    cout << "3: " << cdf.cparent[3] << "\n";


    return 0;
}