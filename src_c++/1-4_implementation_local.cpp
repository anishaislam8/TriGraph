# include <vector>
# include <map>
# include <string>
# include <iostream>
# include <fstream>
# include <set>
# include <algorithm>
# include <utility>
# include <time.h>
# include <openssl/sha.h>
// # include <sqlite3.h>
# include "graph.h"
# include "json.hpp"
using json = nlohmann::json;
using namespace std;


// sha256 function collected from https://stackoverflow.com/a/10632725
string sha256(const string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}



map<string, string> create_object_dict(const json &data){
    map<string, string> object_dict;
    auto all_objects = data["all_objects"];

    for (auto object: all_objects){
        if(object["box"]["object_type"] == "list"){
            string object_text = object["box"]["text"];
            vector<string> tokens;
            string token;
            istringstream tokenStream(object_text);
            while (getline(tokenStream, token, ' ')){
                tokens.push_back(token);
            }
            string object_text_string;
            if (tokens.size() > 1){
                object_text_string =  tokens[0] + "_" + tokens[1];
            }
            else{
                object_text_string = tokens[0];
            }
            object_dict[object["box"]["id"]] = object_text_string;


        }
        else{
            object_dict[object["box"]["id"]] = object["box"]["object_type"];
        }
    }

    return object_dict;
}


set<string> get_unique_tokens(vector<string> nodes, const map<string, string> &object_dict){
    set<string> unique_tokens;
    for (string node: nodes){
        unique_tokens.insert(object_dict.at(node));
    }

    return unique_tokens;
}

map<string, int> get_frequency_1_gram(set<string> unique_tokens, const map<string, string> &object_dict, vector<string> nodes){
    map<string, int> frequency_1_gram;
    for (string token: unique_tokens){
        frequency_1_gram[token] = 0;
    }
    for (string node: nodes){
        frequency_1_gram[object_dict.at(node)] += 1;
    }
    

    return frequency_1_gram;
}


int* create_two_node_adjacency_matrix(string node_0, string node_1, Graph G){

    // we are considering only 1 edge, so entries of adjacency matrix will be 0 or 1

    int* adjacency_matrix_2_gram = new int[4];
    
    vector<string> neighbors_of_node_0 = G.get_neighbors_of_a_node(node_0); // where node_0 is source
    vector<string> neighbors_of_node_1 = G.get_neighbors_of_a_node(node_1); // where node_1 is source
    
    int entry_00 = 0;
    int entry_01 = 0;
    int entry_10 = 0;
    int entry_11 = 0;

    // entry_00: if node_0 is in neighbirs_of_node_0, then entry_00 is 1 else 0
    if (find(neighbors_of_node_0.begin(), neighbors_of_node_0.end(), node_0) != neighbors_of_node_0.end()){ // found
        entry_00 = 1;
    }
    // entry_01
    if (find(neighbors_of_node_0.begin(), neighbors_of_node_0.end(), node_1) != neighbors_of_node_0.end()){ // found
        entry_01 = 1;
    }
    // entry_10
    if (find(neighbors_of_node_1.begin(), neighbors_of_node_1.end(), node_0) != neighbors_of_node_1.end()){ // found
        entry_10 = 1;
    }
    // entry_11
    if (find(neighbors_of_node_1.begin(), neighbors_of_node_1.end(), node_1) != neighbors_of_node_1.end()){ // found
        entry_11 = 1;
    }

    adjacency_matrix_2_gram[0] = entry_00;
    adjacency_matrix_2_gram[1] = entry_01;
    adjacency_matrix_2_gram[2] = entry_10;
    adjacency_matrix_2_gram[3] = entry_11;

    return adjacency_matrix_2_gram;
   

}



map<string, int> get_frequency_2_grams(const vector<vector<string> > &connections, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, Graph G){

    map<string, int> frequncy_2_grams;

    for (auto connection: connections){
        string source = object_dict.at(connection[0]); // this is msg, tgl etc
        string destination = object_dict.at(connection[1]);

        if (source == "" || destination == ""){
            continue;
        }
        
        vector<string> nodes;
        nodes.push_back(source);
        nodes.push_back(destination);
        sort(nodes.begin(), nodes.end());

        string node_0;
        string node_1;

        if (nodes[0] == source){
            node_0 = connection[0]; // has to be original node name like PD-ROOT_obj-0
            node_1 = connection[1];
        }
        else{
            node_0 = connection[1];
            node_1 = connection[0];
        }

        int* adjacency_matrix = create_two_node_adjacency_matrix(node_0, node_1, G);
        int vocab_index[unique_tokens_train.size() + 1];

        int vocab_index_size  = unique_tokens_train.size() + 1;
        int adjacency_matrix_size = 4; // 2 by 2 matrix

        // initialize with 0
        for (int i = 0; i < vocab_index_size; i++){
            vocab_index[i] = 0;
        }
        

        int node_0_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), nodes[0]) - unique_tokens_train.begin(); // msg is where in unique_tokens
        int node_1_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), nodes[1]) - unique_tokens_train.begin();

        vocab_index[node_0_index] += 1;
        vocab_index[node_1_index] += 1;


        // concatenate adjacency matrix and vocab_index
        int initial_key[vocab_index_size + adjacency_matrix_size];
        

        for (int i = 0; i < vocab_index_size; i++){
            initial_key[i] = vocab_index[i];
        }
        
        for (int i = 0; i < adjacency_matrix_size; i++){
            initial_key[vocab_index_size + i] = adjacency_matrix[i];
        }

        // convert this array to string
        string key = "";
        int initial_key_size = vocab_index_size + adjacency_matrix_size;
        for (int i = 0; i < initial_key_size; i++){
            key += to_string(initial_key[i]);
        }

        // now calculate sha256 of this key
        string key_sha256 = sha256(key);

        // update the frequency_2_grams
        if (frequncy_2_grams.find(key_sha256) == frequncy_2_grams.end()){ // could not find it in our map
            frequncy_2_grams[key_sha256] = 1;
        }
        else{
            frequncy_2_grams[key_sha256] += 1;
        }

        // freeing the allocated memory
        delete[] adjacency_matrix;
        
    }
    return frequncy_2_grams;
   
}


vector<vector<string> > get_three_node_subgraphs(vector<string> nodes, Graph G){
    
    set<vector<string> > three_node_subgraphs_set;
    
    for (string node: nodes){
        // clear visited and current_path for each node
        G.visited.clear();
        G.current_path.clear();
        G.dfs(node);
    }

    // G.all_paths has all the paths, but there might be duplicates
    for (auto subgraph: G.all_paths){

        set<string> path_set(subgraph.begin(), subgraph.end());
        vector<string> path(path_set.begin(), path_set.end());
        sort(path.begin(), path.end());

        if (path.size() == 3){
            three_node_subgraphs_set.insert(path);
        }

    }

    vector<vector<string> > three_node_subgraphs(three_node_subgraphs_set.begin(), three_node_subgraphs_set.end());
    return three_node_subgraphs;
}

int* create_three_node_adjacency_matrix(string node_0, string node_1, string node_2, Graph G){


    int* adjacency_matrix_3_gram = new int[9];
    
    vector<string> neighbors_of_node_0 = G.get_neighbors_of_a_node(node_0); // where node_0 is source
    vector<string> neighbors_of_node_1 = G.get_neighbors_of_a_node(node_1); // where node_1 is source
    vector<string> neighbors_of_node_2 = G.get_neighbors_of_a_node(node_2); // where node_2 is source
    
    int entry_00 = 0;
    int entry_01 = 0;
    int entry_02 = 0;
    int entry_10 = 0;
    int entry_11 = 0;
    int entry_12 = 0;
    int entry_20 = 0;
    int entry_21 = 0;
    int entry_22 = 0;

    // entry_00: if node_0 is in neighbors_of_node_0, then entry_00 is 1 else 0
    if (find(neighbors_of_node_0.begin(), neighbors_of_node_0.end(), node_0) != neighbors_of_node_0.end()){ // found
        entry_00 = 1;
    }
    // entry_01
    if (find(neighbors_of_node_0.begin(), neighbors_of_node_0.end(), node_1) != neighbors_of_node_0.end()){ // found
        entry_01 = 1;
    }

    // entry_02
    if (find(neighbors_of_node_0.begin(), neighbors_of_node_0.end(), node_2) != neighbors_of_node_0.end()){ // found
        entry_02 = 1;
    }

    // entry_10
    if (find(neighbors_of_node_1.begin(), neighbors_of_node_1.end(), node_0) != neighbors_of_node_1.end()){ // found
        entry_10 = 1;
    }
    // entry_11
    if (find(neighbors_of_node_1.begin(), neighbors_of_node_1.end(), node_1) != neighbors_of_node_1.end()){ // found
        entry_11 = 1;
    }

    // entry_12
    if (find(neighbors_of_node_1.begin(), neighbors_of_node_1.end(), node_2) != neighbors_of_node_1.end()){ // found
        entry_12 = 1;
    }

    // entry_20
    if (find(neighbors_of_node_2.begin(), neighbors_of_node_2.end(), node_0) != neighbors_of_node_2.end()){ // found
        entry_20 = 1;
    }

    // entry_21
    if (find(neighbors_of_node_2.begin(), neighbors_of_node_2.end(), node_1) != neighbors_of_node_2.end()){ // found
        entry_21 = 1;
    }

    // entry_22
    if (find(neighbors_of_node_2.begin(), neighbors_of_node_2.end(), node_2) != neighbors_of_node_2.end()){ // found
        entry_22 = 1;
    }


    adjacency_matrix_3_gram[0] = entry_00;
    adjacency_matrix_3_gram[1] = entry_01;
    adjacency_matrix_3_gram[2] = entry_02;
    adjacency_matrix_3_gram[3] = entry_10;
    adjacency_matrix_3_gram[4] = entry_11;
    adjacency_matrix_3_gram[5] = entry_12;
    adjacency_matrix_3_gram[6] = entry_20;
    adjacency_matrix_3_gram[7] = entry_21;
    adjacency_matrix_3_gram[8] = entry_22;



    return adjacency_matrix_3_gram;
   

}


// comparator code modified from https://www.geeksforgeeks.org/how-to-sort-vector-using-custom-comparator-in-cpp/
bool comparator(const string& a, const string& b, const map<string, string>& y) {
    return y.at(a) < y.at(b);
}

map<string, int> get_frequency_3_grams(vector<vector<string> > three_node_subgraphs, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, Graph G){

    map<string, int> frequncy_3_grams;

    for (auto subgraph: three_node_subgraphs){

        vector<string> subgraph_nodes = subgraph;

        // code modified from https://www.geeksforgeeks.org/how-to-sort-vector-using-custom-comparator-in-cpp/
        sort(subgraph_nodes.begin(), subgraph_nodes.end(), [&object_dict](const string& a, const string& b) {
            return comparator(a, b, object_dict);
        });

        // object dict equivalent
        vector<string> subgraph_nodes_object_dict;
        for (string node: subgraph_nodes){
            subgraph_nodes_object_dict.push_back(object_dict.at(node));
        }

        if (subgraph_nodes_object_dict[0] == "" || subgraph_nodes_object_dict[1] == "" || subgraph_nodes_object_dict[2] == ""){
            continue;
        }
        

        int* adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G);
        int vocab_index[unique_tokens_train.size() + 1];

        int vocab_index_size  = unique_tokens_train.size() + 1;
        int adjacency_matrix_size = 9; // 3 by 3 matrix


        // initialize with 0
        for (int i = 0; i < vocab_index_size; i++){
            vocab_index[i] = 0;
        }
        

        int node_0_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[0]) - unique_tokens_train.begin(); // msg is where in unique_tokens
        int node_1_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[1]) - unique_tokens_train.begin();
        int node_2_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[2]) - unique_tokens_train.begin();

        vocab_index[node_0_index] += 1;
        vocab_index[node_1_index] += 1;
        vocab_index[node_2_index] += 1;

        // concatenate adjacency matrix and vocab_index
        int initial_key[vocab_index_size + adjacency_matrix_size];
        

        for (int i = 0; i < vocab_index_size; i++){
            initial_key[i] = vocab_index[i];
        }
        
        for (int i = 0; i < adjacency_matrix_size; i++){
            initial_key[vocab_index_size + i] = adjacency_matrix[i];
        }

        // convert this array to string
        string key = "";
        int initial_key_size = vocab_index_size + adjacency_matrix_size;
        for (int i = 0; i < initial_key_size; i++){
            key += to_string(initial_key[i]);
        }

        // now calculate sha256 of this key
        string key_sha256 = sha256(key);

        // update the frequency_3_grams
        if (frequncy_3_grams.find(key_sha256) == frequncy_3_grams.end()){ // could not find it in our map
            frequncy_3_grams[key_sha256] = 1;
        }
        else{
            frequncy_3_grams[key_sha256] += 1;
        }

        // freeing the allocated memory
        delete[] adjacency_matrix;



    }

    return frequncy_3_grams;
}

float get_score(vector<string> subgraph_nodes, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, Graph G){
    float score;
    float discount_factor = 0.05;

    
    // sum of all values of frequency_1_gram
    int sum_frequency_1_gram = 0;
    for (auto token: frequency_1_gram){
        sum_frequency_1_gram += token.second;
    }

    // sum of all values of frequency_2_grams
    int sum_frequency_2_grams = 0;
    for (auto token: frequency_2_grams){
        sum_frequency_2_grams += token.second;
    }

    // sum of all values of frequency_3_grams
    int sum_frequency_3_grams = 0;
    for (auto token: frequency_3_grams){
        sum_frequency_3_grams += token.second;
    }


    int* adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G);
    int vocab_index[unique_tokens_train.size() + 1];
    int vocab_index_size  = unique_tokens_train.size() + 1;
    int adjacency_matrix_size = 9; // 3 by 3 matrix
    
    // initialize with 0
    for (int i = 0; i < vocab_index_size; i++){
        vocab_index[i] = 0;
    }


    auto it_0 = find(unique_tokens_train.begin(), unique_tokens_train.end(), object_dict.at(subgraph_nodes[0]));
    auto it_1 = find(unique_tokens_train.begin(), unique_tokens_train.end(), object_dict.at(subgraph_nodes[1]));
    auto it_2 = find(unique_tokens_train.begin(), unique_tokens_train.end(), object_dict.at(subgraph_nodes[2]));

    if (it_0 != unique_tokens_train.end()){ // found
        vocab_index[it_0 - unique_tokens_train.begin()] += 1;
    }
    else{
        vocab_index[vocab_index_size - 1] += 1;
    }

    if (it_1 != unique_tokens_train.end()){ // found
        vocab_index[it_1 - unique_tokens_train.begin()] += 1;
    }
    else{
        vocab_index[vocab_index_size - 1] += 1;
    }

    if (it_2 != unique_tokens_train.end()){ // found
        vocab_index[it_2 - unique_tokens_train.begin()] += 1;
    }
    else{
        vocab_index[vocab_index_size - 1] += 1;
    }

    // concatenate adjacency matrix and vocab_index
    int initial_key[vocab_index_size + adjacency_matrix_size];
    

    for (int i = 0; i < vocab_index_size; i++){
        initial_key[i] = vocab_index[i];
    }
    
    for (int i = 0; i < adjacency_matrix_size; i++){
        initial_key[vocab_index_size + i] = adjacency_matrix[i];
    }

    // convert this array to string
    string key = "";
    int initial_key_size = vocab_index_size + adjacency_matrix_size;
    for (int i = 0; i < initial_key_size; i++){
        key += to_string(initial_key[i]);
    }

    // now calculate sha256 of this key
    string key_sha256 = sha256(key);

    delete[] adjacency_matrix;


    // if key_sha256 is in frequency_3_grams,then return it's probability
    if (frequency_3_grams.find(key_sha256) != frequency_3_grams.end()){ // found
        return ((frequency_3_grams.at(key_sha256) * 1.0)/(sum_frequency_3_grams * 1.0));
    }
    else{
        score = 1.0 * discount_factor;
        vector<vector<string> > edges_of_G = G.get_edges();

        for(auto edge: edges_of_G){
            vector<string> edge_vector = edge;
            sort(edge_vector.begin(), edge_vector.end(), [&object_dict](const string& a, const string& b) {
                return comparator(a, b, object_dict);
            });

            int* adjacency_matrix_2_gram = create_two_node_adjacency_matrix(edge_vector[0], edge_vector[1], G);

            int vocab_index_2_gram[unique_tokens_train.size() + 1];
            int vocab_index_2_gram_size  = unique_tokens_train.size() + 1;
            int adjacency_matrix_2_gram_size = 4; // 2 by 2 matrix
            
            // initialize with 0
            for (int i = 0; i < vocab_index_2_gram_size; i++){
                vocab_index_2_gram[i] = 0;
            }


            auto it_0_2_gram = find(unique_tokens_train.begin(), unique_tokens_train.end(), object_dict.at(edge_vector[0]));
            auto it_1_2_gram = find(unique_tokens_train.begin(), unique_tokens_train.end(), object_dict.at(edge_vector[1]));
            

            if (it_0_2_gram != unique_tokens_train.end()){ // found
                vocab_index_2_gram[it_0_2_gram - unique_tokens_train.begin()] += 1;
            }
            else{
                vocab_index_2_gram[vocab_index_2_gram_size - 1] += 1;
            }

            if (it_1_2_gram != unique_tokens_train.end()){ // found
                vocab_index_2_gram[it_1_2_gram - unique_tokens_train.begin()] += 1;
            }
            else{
                vocab_index_2_gram[vocab_index_2_gram_size - 1] += 1;
            }

           
            // concatenate adjacency matrix and vocab_index
            int initial_key_2_gram[vocab_index_2_gram_size + adjacency_matrix_2_gram_size];
            

            for (int i = 0; i < vocab_index_2_gram_size; i++){
                initial_key_2_gram[i] = vocab_index_2_gram[i];
            }
            
            for (int i = 0; i < adjacency_matrix_2_gram_size; i++){
                initial_key_2_gram[vocab_index_2_gram_size + i] = adjacency_matrix_2_gram[i];
            }

            // convert this array to string
            string key_2_gram = "";
            int initial_key_2_gram_size = vocab_index_2_gram_size + adjacency_matrix_2_gram_size;
            for (int i = 0; i < initial_key_2_gram_size; i++){
                key_2_gram += to_string(initial_key_2_gram[i]);
            }

            // now calculate sha256 of this key
            string key_sha256_2_gram = sha256(key_2_gram);

            // if key_sha256 is in frequency_2_grams,then return it's probability
            if (frequency_2_grams.find(key_sha256_2_gram) != frequency_2_grams.end()){ // found
                score *= ((frequency_2_grams.at(key_sha256_2_gram) * 1.0)/(sum_frequency_2_grams * 1.0));
            }
            else{
                score *= discount_factor; // missed two grams

                
                if (frequency_1_gram.find(object_dict.at(edge_vector[0])) != frequency_1_gram.end()){ // found
                    score *= ((frequency_1_gram.at(object_dict.at(edge_vector[0])) * 1.0)/(sum_frequency_1_gram * 1.0));
                }
                else{
                    score *= (0.5 * (1.0/(sum_frequency_1_gram * 1.0)));
                }

                
                if (frequency_1_gram.find(object_dict.at(edge_vector[1])) != frequency_1_gram.end()){ // found
                    score *= ((frequency_1_gram.at(object_dict.at(edge_vector[1])) * 1.0)/(sum_frequency_1_gram * 1.0));
                }
                else{
                    score *= (0.5 * (1.0/(sum_frequency_1_gram * 1.0)));
                }
            }

            delete[] adjacency_matrix_2_gram;
            
        }
    }

    return score;
}

float predict(vector<string> subgraph, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, Graph G, string blank_node){
    int next_token_correctly_predicted = 0;
    
   
    string node_to_remove;
    string true_token;


    if (subgraph[0] == blank_node){
        node_to_remove = subgraph[0];
        true_token = object_dict.at(subgraph[0]);
    }
    else if (subgraph[1] == blank_node){
        node_to_remove = subgraph[1];
        true_token = object_dict.at(subgraph[1]);
    }
    else{
        node_to_remove = subgraph[2];
        true_token = object_dict.at(subgraph[2]);
    }

    vector<string> subgraph_nodes = subgraph;
    vector<vector<string> > subgraph_edges;
    vector<vector<string> > edges_of_original_graph = G.get_edges();


    for (auto edge: edges_of_original_graph){
        if (edge[0] == subgraph_nodes[0] && ( edge[1] == subgraph_nodes[1] || edge[1] == subgraph_nodes[2])){
            subgraph_edges.push_back(edge);
        }
        else if (edge[0] == subgraph_nodes[1] && ( edge[1] == subgraph_nodes[0] || edge[1] == subgraph_nodes[2])){
            subgraph_edges.push_back(edge);
        }
        else if (edge[0] == subgraph_nodes[2] && ( edge[1] == subgraph_nodes[0] || edge[1] == subgraph_nodes[1])){
            subgraph_edges.push_back(edge);
        }
    }

    Graph G_test(subgraph_nodes, subgraph_edges); // this is my initial subgraph

    
    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > heap;
    make_heap(heap.begin(), heap.end(), cmp); 
    int max_heap_size = 5;

    // iterate through the vocabulary to find the token that generates the highest score
    for (auto vocab: unique_tokens_train){
        
        string node_to_add = vocab;

        vector<string> subgraph_nodes_test;
        for (string node: subgraph_nodes){
            if (node != node_to_remove){
                subgraph_nodes_test.push_back(node);
            }
        }
        subgraph_nodes_test.push_back(node_to_add);

        // for all the edges in subgraph_edges, if the edge is connected to node_to_remove, 
        // then add an edge to node_to_add, and add the remaining edges as it is
        vector<vector<string> > subgraph_edges_test;
        for (auto edge: subgraph_edges){
            if (edge[0] == node_to_remove && edge[1] == node_to_remove){
                subgraph_edges_test.push_back({node_to_add, node_to_add});
            }
            else if (edge[0] == node_to_remove){
                subgraph_edges_test.push_back({node_to_add, edge[1]});
            }
            else if (edge[1] == node_to_remove){
                subgraph_edges_test.push_back({edge[0], node_to_add});
            }
            else{
                subgraph_edges_test.push_back(edge);
            }
        }

        Graph G_test_new(subgraph_nodes_test, subgraph_edges_test);

        // code modified from https://www.geeksforgeeks.org/how-to-sort-vector-using-custom-comparator-in-cpp/
        sort(subgraph_nodes_test.begin(), subgraph_nodes_test.end(), [&object_dict](const string& a, const string& b) {
            return comparator(a, b, object_dict);
        });

        float score = 0.0;
        try{
            score = get_score(subgraph_nodes_test, object_dict, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test_new);
            float negative_probability_score = -1 * log( score ); // biggest is smallest, following Liveguess MITLM
            pair<string, float> p(vocab, negative_probability_score);
            
            
            if (heap.size() < max_heap_size){
                heap.push_back(p);

                if (heap.size() == max_heap_size){
                    make_heap(heap.begin(), heap.end(), cmp);
                }
            }
            
            else{
                // this is okay as we are using negative probability, if the heap one is larger than the new one, 
                // then replace it as smaller negative probability means bigger real probability
                if (heap.front().second > p.second){ 
                    pop_heap(heap.begin(), heap.end(), cmp);
                    heap.pop_back();
                    
                    heap.push_back(p);
                    push_heap(heap.begin(), heap.end(), cmp);

                    
                }
                
            }



        }
        catch(...){
            cout << "Exception occured while calculating score, assigning 0.0 to score" << endl;
            score = 0.0;
        }

        
    

    }

    sort_heap(heap.begin(), heap.end(), cmp); // sorts the elements in ascending order, that means highest real probability will be at the first
    
    // find the index of the true token in the heap first items
    int index = 0;
    for (auto token: heap){
        if (token.first == true_token){
            break;
        }
        index += 1;
    }


    float mean_reciprocal_rank = 0.0;
    if (index != max_heap_size){
        mean_reciprocal_rank = 1.0/(index+1); // if I find the true token at the first place, then it's reciprocal rank is 1; if I find it at the second place, then it's reciprocal rank is 0.5
    }
    else{
        mean_reciprocal_rank = 0.0;
    }

    return mean_reciprocal_rank;
}


int main(){
    ifstream myfile;
    myfile.open("../sample_jsons/sample2.json");
    string content( (istreambuf_iterator<char>(myfile) ),
                       (istreambuf_iterator<char>()    ) );
   
    json data = json::parse(content);

    map<string, int> frequency_1_gram;
    map<string, int> frequency_2_grams;
    map<string, int> frequency_3_grams;
    vector<string> unique_tokens_train;

    auto connections = data["connections"];
    
    
    vector<string> sources;
    vector<string> destinations;
    vector<vector<string> > edges;
    vector<vector<string> > undirected_edges;

    for (auto connection: connections){
        string source = connection["patchline"]["source"][0];
        string destination = connection["patchline"]["destination"][0];
        // unidirectional edge
        vector<string> edge = {source, destination};
        edges.push_back(edge);
        
        sources.push_back(source);
        destinations.push_back(destination);

        vector<string> edge_2 = {destination, source};
        undirected_edges.push_back(edge);
        undirected_edges.push_back(edge_2);
        
    }

    // if I have no sources or destinations, then I have no connections

    if (sources.size() == 0 || destinations.size() == 0){
        exit(0); // switch to continue when in db
    }

    set<string> nodes_set;
    for (auto source: sources){
        nodes_set.insert(source);
    }
    for (auto destination: destinations){
        nodes_set.insert(destination);
    }

    vector<string> nodes(nodes_set.begin(), nodes_set.end());

    // create a map of string to string
    map<string, string> object_dict = create_object_dict(data);
    // how to access
    // cout << object_dict.at("PD-ROOT_obj-1") << endl;

    set<string> unique_tokens = get_unique_tokens(nodes, object_dict);
    frequency_1_gram = get_frequency_1_gram(unique_tokens, object_dict, nodes);

    // print unique tokens
    // cout << "unique_tokens: \n";
    // for (auto token: unique_tokens){
    //     cout << token << endl;
    // }

    //sort it before passing below
    vector<string> unique_tokens_train_vector(unique_tokens.begin(), unique_tokens.end());
    sort(unique_tokens_train_vector.begin(), unique_tokens_train_vector.end());

    for(auto token: unique_tokens_train_vector){
        unique_tokens_train.push_back(token);
    }


    // step 1 done


    // step 2: Extract 2-gram frequencies
    // create a Graph
    Graph G_directed(nodes, edges);

    frequency_2_grams = get_frequency_2_grams(edges, object_dict, unique_tokens_train, G_directed);

    // cout << "frequency 2 gram size: " << frequency_2_grams.size() << endl;

    // // print frequency_2_grams
    // cout << "frequency_2_grams: \n";
    // for (auto token: frequency_2_grams){
    //     cout << token.first << " " << token.second << endl;
    // }


    // step 2 done

    // step 3: extract 3-gram frequencies

    Graph G_undirected(nodes, undirected_edges);

    vector<vector<string> > three_node_subgraphs = get_three_node_subgraphs(nodes, G_undirected);

    // cout << "G_undirected all paths size: " << G_undirected.all_paths.size() << endl; // output 0 as I did not pass a pointer to this graph

    
    frequency_3_grams = get_frequency_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G_directed);


    
    // cout << "frequency 3 gram size: " << frequency_3_grams.size() << endl;

    // // print frequency_3_grams
    // cout << "frequency_3_grams: \n";
    // for (auto token: frequency_3_grams){
    //     cout << token.first << " " << token.second << endl;
    // }

    // step 3 done

    
    myfile.close();


    // testing

    ifstream myfile_test;
    myfile_test.open("../sample_jsons/sample2.json");
    string content_test( (istreambuf_iterator<char>(myfile_test) ),
                       (istreambuf_iterator<char>()    ) );
   
    json data_test = json::parse(content_test);

    map<string, string> object_dict_test = create_object_dict(data_test);
    int total_predictions_for_this_graph = 0;

    clock_t t;
    t = clock();
    
    try{
        
        auto connections_test = data_test["connections"];

        vector<string> sources_test;
        vector<string> destinations_test;
        vector<vector<string> > edges_test;
        vector<vector<string> > undirected_edges_test;

        for (auto connection: connections_test){
            string source = connection["patchline"]["source"][0];
            string destination = connection["patchline"]["destination"][0];
            // unidirectional edge
            vector<string> edge = {source, destination};
            edges_test.push_back(edge);
            
            sources_test.push_back(source);
            destinations_test.push_back(destination);

            vector<string> edge_2 = {destination, source};
            undirected_edges_test.push_back(edge);
            undirected_edges_test.push_back(edge_2);
            
        }

        

        // if I have no sources or destinations, then I have no connections

        if (sources_test.size() == 0 || destinations_test.size() == 0){
            exit(0); // switch to continue when in db
        }

        set<string> nodes_set_test;
        for (auto source: sources_test){
            nodes_set_test.insert(source);
        }
        for (auto destination: destinations_test){
            nodes_set_test.insert(destination);
        }

        vector<string> nodes_test(nodes_set_test.begin(), nodes_set_test.end());

        Graph G_directed_test(nodes_test, edges_test);
        Graph G_undirected_test(nodes_test, undirected_edges_test);

        // update object_dict with more keys, for each key in unique_tokens_train, add it to object_dict
        for (auto token: unique_tokens_train){
            object_dict_test[token] = token;
        }

        vector<vector<string> > three_node_subgraphs_test = get_three_node_subgraphs(nodes_test, G_undirected_test);
        
 
        
        // for each subgraph, I am going to calculae the mrr score
        float mrr_score = 0.0;

        for (auto node: nodes_test){
            // get all three_node_subgraphs that contain this node
            vector<vector<string> > three_node_subgraphs_containing_this_node;
            for (auto subgraph: three_node_subgraphs_test){
                if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                    three_node_subgraphs_containing_this_node.push_back(subgraph);
                }
            }

            total_predictions_for_this_graph += three_node_subgraphs_containing_this_node.size();
            
            for (auto subgraph: three_node_subgraphs_containing_this_node){
                mrr_score += predict(subgraph, object_dict_test, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_directed_test, node);
            }

        }
        

        mrr_score = mrr_score/total_predictions_for_this_graph;
        cout << "MRR score for this program: " << mrr_score << endl;


    }
    catch(...){
        cout << "Exception occured while testing" << endl;
    }

    t = clock() - t;
    cout << "Time taken: " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    myfile_test.close();




    return 0;

}