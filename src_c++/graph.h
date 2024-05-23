#include <bits/stdc++.h>
using namespace std;


class Graph
{
    vector<string> nodes;
    vector<vector<string> > edges;

    public:

        map<string , bool> visited;
        vector<string> current_path;
        vector<vector<string> > all_paths;

        // constructor
        Graph(vector<string>, vector<vector<string> >);

        // add node
        void add_node(string);

        // bidirectional edge
        void add_edge_bidirectional(string, string);

        // unidirectional edge
        void add_edge_unidirectional(string, string);

        // getter node
        vector<string> get_nodes();

        // getter edges
        vector<vector<string> > get_edges();

        // get neighbors of a node
        vector<string> get_neighbors_of_a_node(string);

        // Depth First Search
        void dfs(string);


};

