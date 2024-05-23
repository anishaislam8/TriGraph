#include "graph.h"


// constructor
Graph::Graph(vector<string> nodes, vector<vector<string> > edges){
    this->nodes = nodes;
    this->edges = edges;
}

// add node
void Graph::add_node(string node){
    this->nodes.push_back(node);
}

// bidirectional edge
void Graph::add_edge_bidirectional(string node1, string node2){
    this->edges.push_back({node1, node2});
    this->edges.push_back({node2, node1});
}

// unidirectional edge
void Graph::add_edge_unidirectional(string node1, string node2){
    this->edges.push_back({node1, node2});
}

// getter node
vector<string> Graph::get_nodes(){
    return this->nodes;
}

// getter edges
vector<vector<string> > Graph::get_edges(){
    return this->edges;
}

// get neighbors of a node
vector<string> Graph::get_neighbors_of_a_node(string node){
    vector<string> neighbors;
    
    for (vector<string> edge: this->edges){
        // unidirectional edge: if source node is node, then destination node is neighbor
        if (edge[0] == node){
            neighbors.push_back(edge[1]);
        }
    }
    return neighbors;
}


// two length dfs from a node
void Graph::dfs(string node){
    // mark node as visited
    visited[node] = true;
    current_path.push_back(node);

    if (current_path.size() == 3){
        all_paths.push_back(current_path);
    } else {
        vector<string> neighbors = this->get_neighbors_of_a_node(node);
        for (string neighbor: neighbors){
            if (!visited[neighbor]){
                dfs(neighbor);
            }
        }
    }

    visited[node] = false;
    current_path.pop_back();
   
}



