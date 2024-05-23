# include <bits/stdc++.h>
# include <sqlite3.h>
# include <openssl/sha.h>
# include "json.hpp"
# include "graph.h"
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

string get_content_from_db(string line, sqlite3* db){
    
    sqlite3_stmt* stmt;
    string hash_id = line;
    const char* sql = "SELECT Content FROM Contents WHERE Hash = ?;";
    string content;

    

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Bind the hash_id to the parameter in the SQL statement
    rc = sqlite3_bind_text(stmt, 1, hash_id.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Retrieve the first column (content)
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            content = reinterpret_cast<const char*>(text);
        }
    } else {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);


    // Output the retrieved content
    if (content.empty()) {
        cout << "No content found for hash id: " << hash_id << endl;
        return "";
    }

    return content;
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

map<string, int> get_frequency_3_grams(vector<vector<string> > three_node_subgraphs, map<string, string> object_dict, vector<string> unique_tokens_train, Graph G){

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


vector<string> load_unique_tokens(){
    ifstream myfile_unique_tokens_train;
    myfile_unique_tokens_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/unique_tokens_train.txt");
    string token;
    vector<string> unique_tokens_train;
    while (myfile_unique_tokens_train >> token){
        unique_tokens_train.push_back(token);
    }


    myfile_unique_tokens_train.close();
    sort(unique_tokens_train.begin(), unique_tokens_train.end());
    return unique_tokens_train;


}


int main(){
    ifstream myfile;
    myfile.open("/media/baguette/aislam4/paths/train_test_split/parallel/train_hashes_1.txt");
    sqlite3* db;
    const char* dbPath = "/media/crouton/aislam4/database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    ofstream myfile_frequency_3_grams_train;
    myfile_frequency_3_grams_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/frequency_3_grams_train_1.txt", ios::app);

    vector<string> unique_tokens_train = load_unique_tokens();

    int processing = 0;
    while(!myfile.eof()){

        
        cout << "Processing: " << processing << endl;
        processing += 1;
        

        string line;
        getline(myfile, line);

        // for each of the hash ids access the sqlite db and get content
        if (line.empty()) {
            continue;
        }
        
        string content = get_content_from_db(line, db);
        if(content.empty()) {
            continue;
        }
        json data = json::parse(content);

        try{
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

            // if I have no sources or destinations, then I have no connections, so no need to process this hash

            if (sources.size() == 0 || destinations.size() == 0){
                continue;
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

            Graph G_directed(nodes, edges);
            Graph G_undirected(nodes, undirected_edges);

            vector<vector<string> > three_node_subgraphs = get_three_node_subgraphs(nodes, G_undirected);            
            map<string, int> frequency_3_grams = get_frequency_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G_directed);

            
            
            for (auto token: frequency_3_grams){
                myfile_frequency_3_grams_train << token.first << " " << token.second << endl;
            }

            

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }

   
    myfile.close();
    myfile_frequency_3_grams_train.close();
    sqlite3_close(db);


    return 0;

}