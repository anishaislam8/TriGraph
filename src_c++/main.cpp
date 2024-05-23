# include <bits/stdc++.h>
# include "graph.h"
using namespace std;


bool comparator(const string& a, const string& b, const map<string, string>& y) {
    return y.at(a) < y.at(b);
}

int main(){

    /*
    // vector<string> nodes = {"A", "B", "C", "D"};
    // vector<vector<string> > edges = {{"A", "B"}, {"B", "C"}, {"C", "D"}};


    vector<string> nodes = {"a", "b" , "c", "d","e", "f", "g", "h"};
    vector<vector<string> > edges = {{"a", "b"}, {"a", "c"}, {"a", "d"}, {"b", "e"}, {"b", "f"}, {"c", "g"}, {"c", "h"}, {"b", "a"}, {"c", "a"}, {"d", "a"}, {"e", "b"}, {"f", "b"}, {"g", "c"}, {"h", "c"}};

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

    set<vector<string> > three_node_subgraphs_set;
    
    for (string node: nodes){
        // clear visited and current_path for each node
        graph.visited.clear();
        graph.current_path.clear();
        graph.dfs(node);
    }

    // G.all_paths has all the paths, but there might be duplicates
    for (auto subgraph: graph.all_paths){

        set<string> path_set(subgraph.begin(), subgraph.end());
        vector<string> path(path_set.begin(), path_set.end());
        sort(path.begin(), path.end());

        if (path.size() == 3){
            three_node_subgraphs_set.insert(path);
        }

    }

    vector<vector<string> > three_node_subgraphs(three_node_subgraphs_set.begin(), three_node_subgraphs_set.end());
    
    cout << "Three node subgraphs: \n";
    for(auto subgraph: three_node_subgraphs){
        for(auto node: subgraph){
            cout << node << " ";
        }
        cout << endl;
    }

    */


    // test sorting vector

    vector<string> x = {"PD_ROOT_obj_0", "PD_ROOT_obj_1", "PD_ROOT_obj_2", "PD_ROOT_obj_3", "PD_ROOT_obj_4"};

    // Example map that maps strings to other strings
    map<string, string> y = {
       {"PD_ROOT_obj_0", "msg"},
       {"PD_ROOT_obj_1", "msg"},
       {"PD_ROOT_obj_2", "floatatom"},
       {"PD_ROOT_obj_3", "tgl"},
       {"PD_ROOT_obj_4", "r"}
    };

    // Sorting the vector x based on the values in map y
    sort(x.begin(), x.end(), [&y](const string& a, const string& b) {
        return comparator(a, b, y);
    });

    // Output the sorted vector
    for (const auto& str : x) {
        cout << str << " ";
    }
    cout << endl;

    return 0;
}