# include <bits/stdc++.h>
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



map<string, string> create_object_dict(json data){
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


set<string> get_unique_tokens(vector<string> nodes, map<string, string> object_dict){
    set<string> unique_tokens;
    for (string node: nodes){
        unique_tokens.insert(object_dict.at(node));
    }

    return unique_tokens;
}

map<string, int> get_frequency_1_gram(set<string> unique_tokens, map<string, string> object_dict, vector<string> nodes){
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


map<string, int> get_frequency_2_grams(vector<vector<string> > connections, map<string, string> object_dict, vector<string> unique_tokens_train, Graph G){

    map<string, int> frequncy_2_grams;

    for (auto connection: connections){
        string source = object_dict.at(connection[0]);
        string destination = object_dict.at(connection[1]);
        vector<string> nodes;
        nodes.push_back(source);
        nodes.push_back(destination);
        sort(nodes.begin(), nodes.end());

        string node_0;
        string node_1;

        if (nodes[0] == source){
            node_0 = source;
            node_1 = destination;
        }
        else{
            node_0 = destination;
            node_1 = source;
        }

        int* adjacency_matrix = create_two_node_adjacency_matrix(node_0, node_1, G);
        int vocab_index[unique_tokens_train.size() + 1];

        // initialize with 0
        for (int i = 0; i < unique_tokens_train.size(); i++){
            vocab_index[i] = 0;
        }

        // get index of node_0 and node_1 in unique_tokens_train, not checking the case where it is not found as the nodes in connections while training should be in unique_tokens
        int node_0_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), node_0) - unique_tokens_train.begin();
        int node_1_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), node_1) - unique_tokens_train.begin();

        vocab_index[node_0_index] += 1;
        vocab_index[node_1_index] += 1;

        // concatenate adjacency matrix and vocab_index
        int vocab_index_size  = unique_tokens_train.size() + 1;
        int adjacency_matrix_size = 4; // 2 by 2 matrix
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


int main(){
    ifstream myfile;
    myfile.open("../sample_jsons/sample2.json");
    string content( (istreambuf_iterator<char>(myfile) ),
                       (std::istreambuf_iterator<char>()    ) );
   
    json data = json::parse(content);

    try{
        auto connections = data["connections"];
        
        
        vector<string> sources;
        vector<string> destinations;
        vector<vector<string> > edges;
        for (auto connection: connections){
            string source = connection["patchline"]["source"][0];
            string destination = connection["patchline"]["destination"][0];
            // unidirectional edge
            vector<string> edge = {source, destination};
            edges.push_back(edge);
            
            sources.push_back(source);
            destinations.push_back(destination);
            
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
        map<string, int> frequency_1_gram = get_frequency_1_gram(unique_tokens, object_dict, nodes);

        // print unique tokens
        cout << "unique_tokens: \n";
        for (auto token: unique_tokens){
            cout << token << endl;
        }

        //sort it before passing below
        vector<string> unique_tokens_train(unique_tokens.begin(), unique_tokens.end());
        sort(unique_tokens_train.begin(), unique_tokens_train.end());


        // step 1 done


        // step 2: Extract 2-gram frequencies
        // create a Graph
        Graph G_directed(nodes, edges);

        map<string, int> frequency_2_grams = get_frequency_2_grams(edges, object_dict, unique_tokens_train, G_directed);

        // print frequency_2_grams
        cout << "frequency_2_grams: \n";
        for (auto token: frequency_2_grams){
            cout << token.first << " " << token.second << endl;
        }
        
    }
    catch(...){
        cout << "Exception occured" << endl;
    }

    
    myfile.close();

    return 0;

}