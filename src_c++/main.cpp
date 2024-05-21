# include <bits/stdc++.h>
# include "utils.h"
using namespace std;

int main(){
    vector<string> nodes = {"A", "B", "C", "D"};
    vector<vector<string> > edges = {{"A", "B"}, {"B", "C"}, {"C", "D"}};

    Graph graph(nodes, edges);

    cout << "Nodes: \n";
    for(auto node: graph.get_nodes()){
        cout << node << " ";
    }
    cout << endl;

    cout << "Edges: \n";
    for(auto edge: graph.get_edges()){
        cout << edge[0] << " " << edge[1] << endl;
    }
    cout << endl;


    return 0;
}