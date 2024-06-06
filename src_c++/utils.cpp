# include "utils.h"


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


int* create_two_node_adjacency_matrix(const string &node_0, const string &node_1, Graph G){

    // we are considering only 1 edge, so entries of adjacency matrix will be 0 or 1

    int* adjacency_matrix_2_gram = new int[4];
    
    unordered_set<string> neighbors_of_node_0 = G.get_neighbors_of_a_node(node_0); // where node_0 is source
    unordered_set<string> neighbors_of_node_1 = G.get_neighbors_of_a_node(node_1); // where node_1 is source
    
    int entry_00 = 0;
    int entry_01 = 0;
    int entry_10 = 0;
    int entry_11 = 0;

    // entry_00: if node_0 is in neighbirs_of_node_0, then entry_00 is 1 else 0
    if (neighbors_of_node_0.find(node_0) != neighbors_of_node_0.end()){ // found
        entry_00 = 1;
    }
    // entry_01
    if (neighbors_of_node_0.find(node_1) != neighbors_of_node_0.end()){ // found
        entry_01 = 1;
    }
    // entry_10
    if (neighbors_of_node_1.find(node_0) != neighbors_of_node_1.end()){ // found
        entry_10 = 1;
    }
    // entry_11
    if (neighbors_of_node_1.find(node_1) != neighbors_of_node_1.end()){ // found
        entry_11 = 1;
    }

    adjacency_matrix_2_gram[0] = entry_00;
    adjacency_matrix_2_gram[1] = entry_01;
    adjacency_matrix_2_gram[2] = entry_10;
    adjacency_matrix_2_gram[3] = entry_11;

    return adjacency_matrix_2_gram;
   

}

vector<vector<string> > get_three_node_subgraphs(const vector<string> &nodes, Graph G){
    
    set<vector<string> > three_node_subgraphs_set;
    
    for (string node: nodes){
        // clear visited and current_path for each node
        G.visited.clear();
        G.current_path.clear();
        G.dfs(node);
    }

    // G.all_paths has all the paths, but there might be duplicates
    for (auto subgraph: G.all_paths){

        set<string> path_set(subgraph.begin(), subgraph.end()); // in case I get loop, discard it
        vector<string> path(path_set.begin(), path_set.end());
        sort(path.begin(), path.end());

        if (path.size() == 3){
            three_node_subgraphs_set.insert(path);
        }

    }

    vector<vector<string> > three_node_subgraphs(three_node_subgraphs_set.begin(), three_node_subgraphs_set.end());
    return three_node_subgraphs;
}

int* create_three_node_adjacency_matrix(const string &node_0, const string &node_1, const string &node_2, Graph G){


    int* adjacency_matrix_3_gram = new int[9];
    
    unordered_set<string> neighbors_of_node_0 = G.get_neighbors_of_a_node(node_0); // where node_0 is source
    unordered_set<string> neighbors_of_node_1 = G.get_neighbors_of_a_node(node_1); // where node_1 is source
    unordered_set<string> neighbors_of_node_2 = G.get_neighbors_of_a_node(node_2); // where node_2 is source
    
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
    if (neighbors_of_node_0.find(node_0) != neighbors_of_node_0.end()){ // found
        entry_00 = 1;
    }
    // entry_01
    if (neighbors_of_node_0.find(node_1) != neighbors_of_node_0.end()){ // found
        entry_01 = 1;
    }

    // entry_02
    if (neighbors_of_node_0.find(node_2) != neighbors_of_node_0.end()){ // found
        entry_02 = 1;
    }

    // entry_10
    if (neighbors_of_node_1.find(node_0) != neighbors_of_node_1.end()){ // found
        entry_10 = 1;
    }
    // entry_11
    if (neighbors_of_node_1.find(node_1) != neighbors_of_node_1.end()){ // found
        entry_11 = 1;
    }

    // entry_12
    if (neighbors_of_node_1.find(node_2) != neighbors_of_node_1.end()){ // found
        entry_12 = 1;
    }

    // entry_20
    if (neighbors_of_node_2.find(node_0) != neighbors_of_node_2.end()){ // found
        entry_20 = 1;
    }

    // entry_21
    if (neighbors_of_node_2.find(node_1) != neighbors_of_node_2.end()){ // found
        entry_21 = 1;
    }

    // entry_22
    if (neighbors_of_node_2.find(node_2) != neighbors_of_node_2.end()){ // found
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

float get_score(const vector<string> &subgraph_nodes, const map<string, string> &object_dict, const map<string, int> &unique_tokens_train_map, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, Graph G, const int sum_frequency_1_gram, const int sum_frequency_2_grams, const int sum_frequency_3_grams){
    float score;
    float discount_factor = 0.05;

    
    int* adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G);
    int vocab_index[unique_tokens_train_map.size() + 1];
    int vocab_index_size  = unique_tokens_train_map.size() + 1;
    int adjacency_matrix_size = 9; // 3 by 3 matrix
    
    // initialize with 0
    for (int i = 0; i < vocab_index_size; i++){
        vocab_index[i] = 0;
    }


    auto it_0 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[0]));
    auto it_1 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[1]));
    auto it_2 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[2]));

    if (it_0 != unique_tokens_train_map.end()){ // found
        vocab_index[it_0->second] += 1;
    }
    else{
        vocab_index[vocab_index_size - 1] += 1;
    }

    if (it_1 != unique_tokens_train_map.end()){ // found
        vocab_index[it_1->second] += 1;
    }
    else{
        vocab_index[vocab_index_size - 1] += 1;
    }

    if (it_2 != unique_tokens_train_map.end()){ // found
        vocab_index[it_2->second] += 1;
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

    auto it_3_gram = frequency_3_grams.find(key_sha256);
    // if key_sha256 is in frequency_3_grams,then return it's probability
    if (it_3_gram != frequency_3_grams.end()){ // found
        return ((it_3_gram->second * 1.0)/(sum_frequency_3_grams * 1.0));
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
            int vocab_index_2_gram[unique_tokens_train_map.size() + 1];
            int vocab_index_2_gram_size  = unique_tokens_train_map.size() + 1;
            int adjacency_matrix_2_gram_size = 4; // 2 by 2 matrix
            
            // initialize with 0
            for (int i = 0; i < vocab_index_2_gram_size; i++){
                vocab_index_2_gram[i] = 0;
            }


            auto it_0_2_gram = unique_tokens_train_map.find(object_dict.at(edge_vector[0]));
            auto it_1_2_gram = unique_tokens_train_map.find(object_dict.at(edge_vector[0]));
            

            if (it_0_2_gram != unique_tokens_train_map.end()){ // found
                vocab_index_2_gram[it_0_2_gram->second] += 1;
            }
            else{
                vocab_index_2_gram[vocab_index_2_gram_size - 1] += 1;
            }

            if (it_1_2_gram != unique_tokens_train_map.end()){ // found
                vocab_index_2_gram[it_1_2_gram->second] += 1;
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
            auto it_2_gram = frequency_2_grams.find(key_sha256_2_gram);

            // if key_sha256 is in frequency_2_grams,then return it's probability
            if (it_2_gram != frequency_2_grams.end()){ // found
                score *= ((it_2_gram->second * 1.0)/(sum_frequency_2_grams * 1.0));
            }
            else{
                score *= discount_factor; // missed two grams

                auto it_1_gram = frequency_1_gram.find(object_dict.at(edge_vector[0]));
                if (it_1_gram != frequency_1_gram.end()){ // found
                    score *= ((it_1_gram->second * 1.0)/(sum_frequency_1_gram * 1.0));
                }
                else{
                    score *= (0.5 * (1.0/(sum_frequency_1_gram * 1.0)));
                }

                it_1_gram = frequency_1_gram.find(object_dict.at(edge_vector[1]));
                if (it_1_gram != frequency_1_gram.end()){ // found
                    score *= ((it_1_gram->second * 1.0)/(sum_frequency_1_gram * 1.0));
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

float predict(const map<string, string> &object_dict, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, const set<string> &node_to_add_list, const string &node_to_remove, const int sum_frequency_1_gram, const int sum_frequency_2_grams, const int sum_frequency_3_grams, const map<string, int> &unique_tokens_train_map, const vector<string> &subgraph_nodes, const vector<vector<string> > &subgraph_edges, const string &true_token){

    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > heap;
    make_heap(heap.begin(), heap.end(), cmp); 
    int max_heap_size = 5;
   
    // iterate through the vocabulary to find the token that generates the highest score
    for (auto item: node_to_add_list){

        if (item.empty()){
            continue;
        }
        
        string node_to_add = item;

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
            score = get_score(subgraph_nodes_test, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test_new, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);
            float negative_probability_score = -1 * log( score ); // biggest is smallest, following Liveguess MITLM
            pair<string, float> p(node_to_add, negative_probability_score);
            
            
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
    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp); // sorts the elements in ascending order, that means highest real probability will be at the first
    // cout << "Heap: \n";
    // for (auto token: heap){
    //     cout << token.first << " " << token.second << endl;
    // }
    // find the index of the true token in the heap first items
    int index = 0;
    for (auto token: heap){
        if (token.first == true_token){
            break;
        }
        index += 1;
    }

    // cout << "True token: " << true_token << " Predicted token: " << heap[0].first << endl;
   

    // float mean_reciprocal_rank = 0.0;
    // if (index != max_heap_size){
    //     mean_reciprocal_rank = 1.0/(index+1); // if I find the true token at the first place, then it's reciprocal rank is 1; if I find it at the second place, then it's reciprocal rank is 0.5
    // }
    // else{
    //     mean_reciprocal_rank = 0.0;
    // }

    return index+1 > max_heap_size ? -1 : index+1;
}

vector<string> load_unique_tokens(){
    ifstream myfile_unique_tokens_train;
    myfile_unique_tokens_train.open("vocabulary_frequencies/unique_tokens_train.txt");
    string token;
    vector<string> unique_tokens_train;
    while (myfile_unique_tokens_train >> token){
        unique_tokens_train.push_back(token);
    }


    myfile_unique_tokens_train.close();
    sort(unique_tokens_train.begin(), unique_tokens_train.end());
    return unique_tokens_train;


}

map<string, int> load_frequency_1_gram(){
    ifstream myfile_frequency_1_gram_train;
    myfile_frequency_1_gram_train.open("vocabulary_frequencies/frequency_1_gram_train.txt");
    map<string, int> frequency_1_gram;
    string token;
    string frequency;
    string line;
    
    while (getline(myfile_frequency_1_gram_train, line)) { 
        // split line into token and frequency
        stringstream iss(line);
        iss >> token >> frequency;
        if (token == "" || frequency == ""){
            continue;
        }
        frequency_1_gram[token] = atoi(frequency.c_str());
        

    }
    

    myfile_frequency_1_gram_train.close();
    return frequency_1_gram;


}

map<string, int> load_frequency_2_grams(){
    ifstream myfile_frequency_2_gram_train;
    myfile_frequency_2_gram_train.open("vocabulary_frequencies/frequency_2_grams_train.txt");
    map<string, int> frequency_2_gram;
    string token;
    string frequency;
    string line;
    
    while (getline(myfile_frequency_2_gram_train, line)) { 
        // split line into token and frequency
        stringstream iss(line);
        iss >> token >> frequency;
        if (token == "" || frequency == ""){
            continue;
        }
        frequency_2_gram[token] = atoi(frequency.c_str());
        

    }
    

    myfile_frequency_2_gram_train.close();
    return frequency_2_gram;


}

map<string, int> load_frequency_3_grams(){
    ifstream myfile_frequency_3_gram_train;
    myfile_frequency_3_gram_train.open("vocabulary_frequencies/frequency_3_grams_train_merged.txt");
    map<string, int> frequency_3_gram;
    string token;
    string frequency;
    string line;
    
    while (getline(myfile_frequency_3_gram_train, line)) { 
        // split line into token and frequency
        stringstream iss(line);
        iss >> token >> frequency;
        if (token == "" || frequency == ""){
            continue;
        }
        frequency_3_gram[token] = atoi(frequency.c_str());
        

    }
    

    myfile_frequency_3_gram_train.close();
    return frequency_3_gram;


}

set<vector<string> > get_three_node_subgraphs_sorted_by_object_dict(){

    ifstream myfile_3_grams_train;
    myfile_3_grams_train.open("vocabulary_frequencies/3_grams_train_filtered_final.txt");
    set<vector<string> > three_node_subgraphs_sorted_by_object_dict;
    string token_0;
    string token_1;
    string token_2;

    string line;
    
    while (getline(myfile_3_grams_train, line)) { 
        // split line into token and frequency
        stringstream iss(line);
        iss >> token_0 >> token_1 >> token_2;
        if (token_0 == "" || token_1 == "" || token_2 == ""){
            continue;
        }
        three_node_subgraphs_sorted_by_object_dict.insert({token_0, token_1, token_2});
    }
    

    myfile_3_grams_train.close();
    return three_node_subgraphs_sorted_by_object_dict;
} 

set<string> get_unique_tokens(const vector<string> &nodes, const map<string, string> &object_dict){
    set<string> unique_tokens;
    for (string node: nodes){
        unique_tokens.insert(object_dict.at(node));
    }

    return unique_tokens;
}

map<string, int> get_frequency_1_gram(const set<string> &unique_tokens, const map<string, string> &object_dict, const vector<string> &nodes){
    map<string, int> frequency_1_gram;
    for (string token: unique_tokens){
        frequency_1_gram[token] = 0;
    }
    for (string node: nodes){
        frequency_1_gram[object_dict.at(node)] += 1;
    }
    

    return frequency_1_gram;
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

map<string, int> get_frequency_3_grams(const vector<vector<string> > &three_node_subgraphs, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, Graph G){

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

vector<string> find_the_set_difference(const vector<string> &subgraph, const vector<string> &two_nodes){
    multiset<string> set1(subgraph.begin(), subgraph.end());
    multiset<string> set2(two_nodes.begin(), two_nodes.end());
    vector<string> result;

    // Find the difference between the two sets
    set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(result));
    if (result.size() > 1){
        cout << "Result size: " << result.size() << endl;
        cout << "Set 1: " << endl;
        for (auto item: set1){
            cout << item << " ";
        }
        cout << endl;
        cout << "Set 2: " << endl;
        for (auto item: set2){
            cout << item << " ";
        }
        cout << endl;
    }

    return result;
}