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




