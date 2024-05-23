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


int* create_three_node_adjacency_matrix(string node_0, string node_1, Graph G){

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

map<string, int> get_frequency_3_grams(vector<vector<string> > connections, map<string, string> object_dict, vector<string> unique_tokens_train, Graph G){

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


        cout << node_0_index << " " << node_1_index << endl;
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

        // update the frequency_3_grams
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
    myfile.open("/media/baguette/aislam4/paths/train_test_split/train_hashes.txt");
    sqlite3* db;
    const char* dbPath = "/media/crouton/aislam4/database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    map<string, int> frequency_3_grams_train;
    vector<string> unique_tokens_train = load_unique_tokens();


    while(!myfile.eof()){
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

                // bidirectional edge
                vector<string> edge2 = {destination, source};
                undirected_edges.push_back(edge);
                undirected_edges.push_back(edge2);

                
                sources.push_back(source);
                destinations.push_back(destination);
                
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

            // create a directed and undirected graph
            Graph G_directed(nodes, edges);
            Graph G_undirected(nodes, undirected_edges);

            map<string, int> frequency_3_grams = get_frequency_3_grams(edges, object_dict, unique_tokens_train, G_directed);

            // update the final freuqency 2 grams after each hash content
            for (auto token: frequency_3_grams){
                if (frequency_3_grams_train.find(token.first) == frequency_3_grams_train.end()){
                    frequency_3_grams_train[token.first] = token.second;
                }
                else{
                    frequency_3_grams_train[token.first] += token.second;
                }
            }

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }


    // save frequency_3_grams_train to a file
    ofstream myfile_frequency_3_grams_train;
    myfile_frequency_3_grams_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/frequency_3_grams_train.txt");
    for (auto token: frequency_3_grams_train){
        myfile_frequency_3_grams_train << token.first << " " << token.second << endl;
    }

    myfile_frequency_3_grams_train.close();
    myfile.close();
    sqlite3_close(db);


    return 0;

}