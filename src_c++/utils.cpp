# include "utils.h"

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


vector<int> create_two_node_adjacency_matrix(const string &node_0, const string &node_1, Graph G){

    // we are considering only 1 edge, so entries of adjacency matrix will be 0 or 1

    vector<int> adjacency_matrix_2_gram(4, 0);
    
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

vector<int> create_three_node_adjacency_matrix(const string &node_0, const string &node_1, const string &node_2, Graph G){


    vector<int> adjacency_matrix_3_gram(9, 0);
    
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


map<string, unordered_set<string> > get_map_two_grams_to_connections(){

    map<string, unordered_set<string> > two_grams_to_connections;
    vector<string> observed_three_grams = load_observed_three_grams();
    for (auto three_grams: observed_three_grams){
        vector<string> tokens;
        string token;
        istringstream tokenStream(three_grams);
        while (getline(tokenStream, token, ',')){
            tokens.push_back(token);
        }
        string first_token = tokens[0];
        string second_token = tokens[1];
        string third_token = tokens[2];

        string key_1 = first_token + "," + second_token;
        string key_2 = second_token + "," + third_token;
        string key_3 = first_token + "," + third_token;

        if (two_grams_to_connections.find(key_1) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_1] = {third_token};
        }
        else{
            two_grams_to_connections[key_1].insert(third_token);
        }

        if (two_grams_to_connections.find(key_2) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_2] = {first_token};
        }
        else{
            two_grams_to_connections[key_2].insert(first_token);
        }

        if (two_grams_to_connections.find(key_3) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_3] = {second_token};
        }
        else{
            two_grams_to_connections[key_3].insert(second_token);
        }

        
    }

    return two_grams_to_connections;


}

float get_score(const vector<string> &subgraph_nodes, const map<string, string> &object_dict, const map<string, int> &unique_tokens_train_map, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, Graph G, const int sum_frequency_1_gram, const int sum_frequency_2_grams, const int sum_frequency_3_grams){
    float score;
    float discount_factor = 0.05;

    vector<int> adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G);
   
    // subgraph nodes are already sorted
    auto it_0 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[0]));
    auto it_1 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[1]));
    auto it_2 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[2]));

    auto it_3_gram = frequency_3_grams.end();

    // if I have found all the nodes in unique trains before, only then calculate the hash
    if(it_0 != unique_tokens_train_map.end() && it_1 != unique_tokens_train_map.end() && it_2 != unique_tokens_train_map.end()){
        string key = get_key({it_0->second, it_1->second, it_2->second}, adjacency_matrix);
        it_3_gram = frequency_3_grams.find(key);
    }

    
    // if key is in frequency_3_grams,then return it's probability
    if (it_3_gram != frequency_3_grams.end()){ // found
        return ((it_3_gram->second * 1.0)/(sum_frequency_3_grams * 1.0));
    }
    else{
        score = 1.0 * discount_factor;
        vector<vector<string> > edges_of_G = G.get_edges();

        set<vector<string> > connections_set;

        for (auto connection: edges_of_G){
            sort(connection.begin(), connection.end(), [&object_dict](const string& a, const string& b) {
                return comparator(a, b, object_dict);
            });

            // the connection is now sorted by object dict
            connections_set.insert(connection);
        }
        // the edge_vectors are already sorted
        for(auto edge_vector: connections_set){
           
            vector<int> adjacency_matrix_2_gram = create_two_node_adjacency_matrix(edge_vector[0], edge_vector[1], G);
           
            string first_edge_item = object_dict.at(edge_vector[0]);
            string second_edge_item = object_dict.at(edge_vector[1]);

            auto it_0_2_gram = unique_tokens_train_map.find(first_edge_item);
            auto it_1_2_gram = unique_tokens_train_map.find(second_edge_item);

            auto it_2_gram = frequency_2_grams.end();

            if(it_0_2_gram != unique_tokens_train_map.end() && it_1_2_gram != unique_tokens_train_map.end()){               
                string key_2_gram = get_key({it_0_2_gram->second, it_1_2_gram->second}, adjacency_matrix_2_gram);
                it_2_gram = frequency_2_grams.find(key_2_gram);
            }

            if (it_2_gram != frequency_2_grams.end()){ // found
                score *= ((it_2_gram->second * 1.0)/(sum_frequency_2_grams * 1.0));
            }
            else{
                score *= discount_factor; // missed two grams

                auto it_1_gram = frequency_1_gram.find(first_edge_item);
                if (it_1_gram != frequency_1_gram.end()){ // found
                    score *= ((it_1_gram->second * 1.0)/(sum_frequency_1_gram * 1.0));
                }
                else{
                    score *= (0.5 * (1.0/(sum_frequency_1_gram * 1.0)));
                    score *= discount_factor; // missed one grams
                }

                it_1_gram = frequency_1_gram.find(second_edge_item);
                if (it_1_gram != frequency_1_gram.end()){ // found
                    score *= ((it_1_gram->second * 1.0)/(sum_frequency_1_gram * 1.0));
                }
                else{
                    score *= (0.5 * (1.0/(sum_frequency_1_gram * 1.0)));
                    score *= discount_factor; // missed one grams
                }
            }            
        }
    }

    return score;
}

float score_of_a_subgraph_with_a_word_from_vocab(const string &node_to_add, const string &node_to_remove, const vector<string> &subgraph, const vector<vector<string> > &subgraph_edges, const map<string, string> &object_dict, const map<string, int> &unique_tokens_train_map, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, const int sum_frequency_1_gram, const int sum_frequency_2_grams, const int sum_frequency_3_grams){

    vector<string> subgraph_nodes_test;
    for (string node: subgraph){
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
    }
    catch(...){
        cout << "Exception occured while calculating score, assigning 0.0 to score" << endl;
        score = 0.0;
    }

    return score;

}

unordered_set<string> get_node_to_add_list_for_a_subgraph(const vector<string> &subgraph, const string &node_to_remove, const map<string, unordered_set<string> >& two_grams_to_connections, const map<string, string> &object_dict, const map<string, int> &unique_tokens_train_map){
    // create node to add list for this subgraph

    vector<string> two_nodes;
    for (auto item: subgraph){
        if (item != node_to_remove){ // take the ones except for this one
            two_nodes.push_back(object_dict.at(item));
        }
    }

    auto it_0 = unique_tokens_train_map.find(two_nodes[0]);
    auto it_1 = unique_tokens_train_map.find(two_nodes[1]);

    if (it_0 == unique_tokens_train_map.end() || it_1 == unique_tokens_train_map.end()){
        return {};
    }

    // find the third node in three_node_subgraphs_sorted_by_object_dict which has these two nodes
    unordered_set<string> node_to_add_list;
    
    // what is the unique_tokens_train_map value for this two nodes
    int value_0 = it_0->second;
    int value_1 = it_1->second;

    vector<int> two_nodes_int = {value_0, value_1};
    // sort the two nodes
    sort(two_nodes_int.begin(), two_nodes_int.end());

    string key = to_string(two_nodes_int[0]) + "," + to_string(two_nodes_int[1]);


    if (two_grams_to_connections.find(key) != two_grams_to_connections.end()){ // found
        node_to_add_list = two_grams_to_connections.at(key);
    }

    return node_to_add_list;
}

int predict(const vector<vector <string> > &three_node_subgraphs_containing_this_node, const map<string, unordered_set<string> >& two_grams_to_connections, const map<string, string> &object_dict, const map<string, int> &frequency_1_gram, const map<string, int> &frequency_2_grams, const map<string, int> &frequency_3_grams, const string &node_to_remove, const int sum_frequency_1_gram, const int sum_frequency_2_grams, const int sum_frequency_3_grams, const map<string, int> &unique_tokens_train_map, const vector<string>& unique_tokens_train, Graph G_directed_test){

    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > heap;
    make_heap(heap.begin(), heap.end(), cmp); 
    int max_heap_size = 10;
    float score;

    string true_token = object_dict.at(node_to_remove);
    vector<vector<string> > edges_of_original_graph = G_directed_test.get_edges();

    // I am calling the predict function for each three_node_subgraph that contains this node
    for (auto subgraph: three_node_subgraphs_containing_this_node){

        vector<vector<string> > subgraph_edges;
        
        // this is okay, if edge is (0,0) and nodes set is (0,1,2) -> add this edge
        for (const auto& edge : edges_of_original_graph) {
            if (count(subgraph.begin(), subgraph.end(), edge[0]) > 0 && count(subgraph.begin(), subgraph.end(), edge[1]) > 0) {
                subgraph_edges.push_back(edge);
            }
        }

        // create a list of vocabulary to iterate through for this subgraph
        unordered_set<string> node_to_add_list_int = get_node_to_add_list_for_a_subgraph(subgraph, node_to_remove, two_grams_to_connections, object_dict, unique_tokens_train_map);

        unordered_set<string> node_to_add_list;
        for (auto item: node_to_add_list_int){
            node_to_add_list.insert(unique_tokens_train[stoi(item)]);
        }

        // now node_to_add list could be empty since this is a test graph, and I might not have seen this node before during train
        if (node_to_add_list.size() == 0){
            for (auto token: unique_tokens_train_map){
                node_to_add_list.insert(token.first);
            }
        }
        // iterate through the vocabulary to find the token that generates the highest score

        for (auto item: node_to_add_list){

            if (item.empty()){
                continue;
            }
            
            score = score_of_a_subgraph_with_a_word_from_vocab(item, node_to_remove, subgraph, subgraph_edges, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);
            float negative_probability_score = -1 * log( score ); // biggest is smallest, following Liveguess MITLM
            pair<string, float> p(item, negative_probability_score);
            
            // if heap has that item already, then no need to insert it again, check if the current probability is less than the one in the heap, if yes, then update it
            bool found = false;
            for (auto &token: heap){
                if (token.first == item){
                    found = true;
                    if (token.second > p.second){
                        token.second = p.second;
                    }
                    break;
                }
            }
            if (found){
                make_heap(heap.begin(), heap.end(), cmp);
            }
            else{
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
                        pop_heap(heap.begin(), heap.end(), cmp); //  to move the top element of the heap to the end of the container
                        heap.pop_back(); // actually remove that element from the container

                        heap.push_back(p); // add the new element to the end of the container
                        push_heap(heap.begin(), heap.end(), cmp); // rearranges elements to make sure heap property is maintained
                    }
                    
                }
            }
            

        }
    }
   

    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp); // sorts the elements in ascending order, that means highest real probability will be at the first
    // cout << "Heap: \n";
    // for (auto token: heap){
    //     cout << token.first << " " << token.second << endl;
    // }
    // find the index of the true token in the heap first items
    int index = -1;
    for (int i = 0; i < heap.size(); i++){
        if (heap[i].first == true_token){
            index = i+1;
            break;
        }
    }

    //cout << "True token: " << true_token << " Predicted token: " << heap[0].first << endl;
    return index;
}


vector<pair<string, float> > create_heap(const vector<pair<string, float> >& heap_to_insert){
    
    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<pair<string, float> > heap;
    make_heap(heap.begin(), heap.end(), cmp); 
    int max_heap_size = 10;
    
    for (auto p: heap_to_insert){
        bool found = false;
        for (auto &token: heap){
            if (token.first == p.first){
                found = true;
                if (token.second > p.second){
                    token.second = p.second;
                }
                break;
            }
        }
        if (found){
            make_heap(heap.begin(), heap.end(), cmp);
        }
        else{
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
                    pop_heap(heap.begin(), heap.end(), cmp); //  to move the top element of the heap to the end of the container
                    heap.pop_back(); // actually remove that element from the container

                    heap.push_back(p); // add the new element to the end of the container
                    push_heap(heap.begin(), heap.end(), cmp); // rearranges elements to make sure heap property is maintained
                }
                
            }
        }
        
    }

    return heap;
}


map<string, vector<pair<string, float> > > get_frequency_2_gram_map(const map<string, int>& frequency_2_grams){
     // create a map from frequency_2_gram where key is the first two tokens of current key separated by delimeters and value is a vector of map of <string, int> where string is the third token of the current key
    map<string, vector<pair<string, float> > > frequency_2_grams_map;
    for (auto two_gram: frequency_2_grams){
        vector<string> tokens;
        string token;
        istringstream tokenStream(two_gram.first);
        while (getline(tokenStream, token, ',')){
            tokens.push_back(token);
        }
        string adj_mat = tokens[2];
        pair<string, float> p = make_pair(adj_mat, two_gram.second*1.0);
        vector<pair<string, float> > v;
        string key = tokens[0] + "," + tokens[1];

        if (frequency_2_grams_map.find(key) == frequency_2_grams_map.end()){ // could not find the key
            v.push_back(p);
            frequency_2_grams_map[key] = v;
        }
        else{
            frequency_2_grams_map[key].push_back(p);
        }

    }
    return frequency_2_grams_map;
}

map<string, vector<pair<string, float> > > get_frequency_3_gram_map(const map<string, int>& frequency_3_grams, const int sum_frequency_3_grams){
     // create a map from frequency_2_gram where key is the first two tokens of current key separated by delimeters and value is a vector of map of <string, int> where string is the third token of the current key
    map<string, vector<pair<string, float> > > frequency_3_grams_map;
    for (auto three_gram: frequency_3_grams){
        vector<string> tokens;
        string token;
        istringstream tokenStream(three_gram.first);
        while (getline(tokenStream, token, ',')){
            tokens.push_back(token);
        }
        string adj_mat = tokens[3];
        float negative_probability_score = -1 * log( (three_gram.second * 1.0)/(sum_frequency_3_grams * 1.0) );
        pair<string, float> p = make_pair(adj_mat, negative_probability_score);
        vector<pair<string, float> > v;
        string key = tokens[0] + "," + tokens[1] + "," + tokens[2];

        if (frequency_3_grams_map.find(key) == frequency_3_grams_map.end()){ // could not find the key
            v.push_back(p);
            frequency_3_grams_map[key] = v;
        }
        else{
            frequency_3_grams_map[key].push_back(p);
        }

    }
    return frequency_3_grams_map;
}

pair<string, float> get_two_gram_pair_for_heap(const auto& two_gram_0_1, const auto& two_gram_1_2, const auto& two_gram_0_2, const int sum_frequency_2_grams){
    string ind_0, ind_1, ind_2, ind_3, ind_4, ind_5, ind_6, ind_7, ind_8;
    ind_0 = two_gram_0_1.first[0];
    ind_1 = two_gram_0_1.first[1];
    ind_3 = two_gram_0_1.first[2];
    ind_4 = two_gram_0_1.first[3];


    // if ind_4 is not 1, then it is currently 0 and it could be 0 or 1 based on the next part
    if (ind_4 != "1"){
        ind_4 = two_gram_1_2.first[0];
    }
    

    ind_5 = two_gram_1_2.first[1];
    ind_7 = two_gram_1_2.first[2];
    ind_8 = two_gram_1_2.first[3];
    
    if(ind_0 != "1"){
        ind_0 = two_gram_0_2.first[0];
    }
    
    ind_2 = two_gram_0_2.first[1];
    ind_6 = two_gram_0_2.first[2];
    
    if(ind_8 != "1"){
        ind_8 = two_gram_0_2.first[3];
    }

    string adj_mat = ind_0 + ind_1 + ind_2 + ind_3 + ind_4 + ind_5 + ind_6 + ind_7 + ind_8;
    float negative_probability_score = -1 * log( (two_gram_0_1.second * two_gram_1_2.second * two_gram_0_2.second)/(sum_frequency_2_grams * sum_frequency_2_grams * sum_frequency_2_grams * 1.0) ); // biggest is smallest, following Liveguess MITLM
    pair<string, float> p(adj_mat, negative_probability_score);

    return p;
}

int predict_edges(const vector<string>& subgraph, const map<string, string>& object_dict, const map<string, vector<pair<string, float> > >& frequency_2_grams_map, const map<string, vector<pair<string, float> > >& frequency_3_grams_map, const int sum_frequency_2_grams, const int sum_frequency_3_grams, const map<string, int>& unique_tokens_train_map, Graph G_directed_test){
    
   
    vector<pair<string, float> > heap;
    int max_heap_size = 10;
    auto cmp = [](const pair<string, float>& left, const pair<string, float>& right) {
        return left.second < right.second;
    };

    vector<string> subgraph_nodes;
    for (auto item: subgraph){
        subgraph_nodes.push_back(item);
    }

    // sort subgraph nodes based on object_dict
    sort(subgraph_nodes.begin(), subgraph_nodes.end(), [&object_dict](const string& a, const string& b) {
        return comparator(a, b, object_dict);
    });
    

    vector<int> adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G_directed_test);

    string true_adjacency_matrix = "";
    for (auto item: adjacency_matrix){
        true_adjacency_matrix += to_string(item);
    }

    // given three nodes, I need to find in the frequency three gram keys, if the key contains these three nodes

    auto it_0 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[0]));
    auto it_1 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[1]));
    auto it_2 = unique_tokens_train_map.find(object_dict.at(subgraph_nodes[2]));

    vector<pair<string, float> > heap_3_gram;
    
    
    // if I have found all the nodes in unique trains, only then check the frequency_3_grams
    if(it_0 != unique_tokens_train_map.end() && it_1 != unique_tokens_train_map.end() && it_2 != unique_tokens_train_map.end()){
        
        string key = to_string(it_0->second) + "," + to_string(it_1->second) + "," + to_string(it_2->second);
        auto it_3_gram = frequency_3_grams_map.find(key);

        if (it_3_gram != frequency_3_grams_map.end()){
            heap_3_gram = it_3_gram->second;
        }

        
    }
    
    
    
    // if heap_3_gram is empty then I couldn't find any three gram in our frequency_3_grams that matches our nodes
    // in this case, I will have to move to 2 grams
    
    //in the case where I have unknown tokens, I wouldn't have gone inside the if above and heap_3_gram would be empty
    
    // in either cases, when heap_3_grams is empty, move to 2 grams, if not, then insert in heap
    
    
    if (heap_3_gram.size() > 0){
        heap = create_heap(heap_3_gram);
    }
    else{
        // move to 2 grams
        vector<pair<string, float> > node_0_node_1;
        vector<pair<string, float> > node_1_node_2;
        vector<pair<string, float> > node_0_node_2;

        // node_0 and node_1

        if (it_0 != unique_tokens_train_map.end() && it_1 != unique_tokens_train_map.end()){
            string key = to_string(it_0->second) + "," + to_string(it_1->second);
            auto it_2_gram = frequency_2_grams_map.find(key);
            if (it_2_gram != frequency_2_grams_map.end()){
                node_0_node_1 = it_2_gram->second;
            }
            
            
        }

        // node_1 and node_2

        if (it_1 != unique_tokens_train_map.end() && it_2 != unique_tokens_train_map.end()){
            string key = to_string(it_1->second) + "," + to_string(it_2->second);
            auto it_2_gram = frequency_2_grams_map.find(key);
            if (it_2_gram != frequency_2_grams_map.end()){
                node_1_node_2 = it_2_gram->second;
            }

            
        }

        // node_0 and node_2

        if (it_0 != unique_tokens_train_map.end() && it_2 != unique_tokens_train_map.end()){
            string key = to_string(it_0->second) + "," + to_string(it_2->second);
            auto it_2_gram = frequency_2_grams_map.find(key);
            if (it_2_gram != frequency_2_grams_map.end()){
                node_0_node_2 = it_2_gram->second;
            }

            
        }

        // if node_0_node_1 is empty, then I couldn't find any two gram in our frequency_2_grams that matches our nodes
        // in this case, there are no connections between node_0 and node_1
        // also if node_0 or node_1 is unknown, then I wouldn't have gone inside the if above and node_0_node_1 would be empty
        // in either cases, when node_0_node_1 is empty, there are no connections between node_0 and node_1

        // same for node_1_node_2 and node_0_node_2

        // give a small probability to no connections and ensure that there's at least one element in the heap
        node_0_node_1.push_back({"0000", 1});
        node_1_node_2.push_back({"0000", 1});
        node_0_node_2.push_back({"0000", 1});
        

        vector<pair<string, float> > heap_2_gram;


        for (const auto two_gram_0_1: node_0_node_1){
            for (const auto two_gram_1_2: node_1_node_2){
                for (const auto two_gram_0_2: node_0_node_2){
                    pair<string, float> p = get_two_gram_pair_for_heap(two_gram_0_1, two_gram_1_2, two_gram_0_2, sum_frequency_2_grams);
                    heap_2_gram.push_back(p);
                }
            }
        }


        // there should be at least one element in heap as we have added a default one in the beginning
        // this covers the scenario when all nodes are unknown and we could not connect them

        heap = create_heap(heap_2_gram);

    }

    make_heap(heap.begin(), heap.end(), cmp);
    sort_heap(heap.begin(), heap.end(), cmp); // sorts the elements in ascending order, that means highest real probability will be at the first

    // // print heap
    // cout << "Heap: " << endl;
    // for (auto token: heap){
    //     cout << token.first << " " << token.second << endl;
    // }

    // cout << "True adjacency matrix: " << true_adjacency_matrix << endl;

    // find the index of the true token in the heap first items
    int index = -1;
    for (int i = 0; i < heap.size(); i++){
        if (heap[i].first == true_adjacency_matrix){
            index = i+1;
            break;
        }
    }

    return index;

}

vector<string> load_observed_three_grams(){
    ifstream myfile_observed_three_grams;
    myfile_observed_three_grams.open("vocabulary_frequencies/observed_3_grams_train.txt");
    string token;
    vector<string> observed_three_grams;
    while (myfile_observed_three_grams >> token){
        observed_three_grams.push_back(token);
    }


    myfile_observed_three_grams.close();
    sort(observed_three_grams.begin(), observed_three_grams.end());
    return observed_three_grams;


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
    myfile_frequency_3_gram_train.open("vocabulary_frequencies/frequency_3_grams_train.txt");
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


string get_key(const vector<int>& nodes_indices, const vector<int>& adjacency_matrix){
    string key = "";
    for (int node: nodes_indices){
        key += to_string(node);
        key += ",";
    }
    for (int entry: adjacency_matrix){
        key += to_string(entry);
    }
    return key;
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

    set<vector<string> > connections_set;

    for (auto connection: connections){
        sort(connection.begin(), connection.end(), [&object_dict](const string& a, const string& b) {
            return comparator(a, b, object_dict);
        });

        // the connection is now sorted by object dict
        connections_set.insert(connection);
    }

    // connections set contains all unique two node connections regardless of loop
    for (auto connection: connections_set){

        // connection is already sorted by object_dict
        vector<string> nodes = {object_dict.at(connection[0]), object_dict.at(connection[1])};

        vector<int> adjacency_matrix = create_two_node_adjacency_matrix(connection[0], connection[1], G);

        int node_0_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), nodes[0]) - unique_tokens_train.begin(); // msg is where in unique_tokens
        int node_1_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), nodes[1]) - unique_tokens_train.begin();
        
        string key = get_key({node_0_index, node_1_index}, adjacency_matrix);


        // update the frequency_2_grams
        if (frequncy_2_grams.find(key) == frequncy_2_grams.end()){ // could not find it in our map
            frequncy_2_grams[key] = 1;
        }
        else{
            frequncy_2_grams[key] += 1;
        }
        
    }
    return frequncy_2_grams;
   
}



map<string, int> get_frequency_3_grams(const vector<vector<string> >& three_node_subgraphs, const map<string, string> &object_dict, const vector<string> &unique_tokens_train, Graph G){

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
        

        vector<int> adjacency_matrix = create_three_node_adjacency_matrix(subgraph_nodes[0], subgraph_nodes[1], subgraph_nodes[2], G);
    
        int node_0_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[0]) - unique_tokens_train.begin(); // msg is where in unique_tokens
        int node_1_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[1]) - unique_tokens_train.begin();
        int node_2_index = find(unique_tokens_train.begin(), unique_tokens_train.end(), subgraph_nodes_object_dict[2]) - unique_tokens_train.begin();

        string key = get_key({node_0_index, node_1_index, node_2_index}, adjacency_matrix);
        
        // update the frequency_3_grams
        if (frequncy_3_grams.find(key) == frequncy_3_grams.end()){ // could not find it in our map
            frequncy_3_grams[key] = 1;
        }
        else{
            frequncy_3_grams[key] += 1;
        }
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

    unordered_map<string, int> count_map;
    
    // Count occurrences in subgraph
    for (const auto &item : subgraph) {
        count_map[item]++;
    }

    // Subtract counts for two_nodes
    for (const auto &item : two_nodes) {
        count_map[item]--;
    }

    vector<string> result;
    for (const auto &pair : count_map) {
        if (pair.second > 0) {
            result.push_back(pair.first);
        }
    }
    if (result.size() > 1) {
        cout << "Result size: " << result.size() << endl;
        cout << "Set 1: " << endl;
        for (const auto &item : subgraph) {
            cout << item << " ";
        }
        cout << endl;
        cout << "Set 2: " << endl;
        for (const auto &item : two_nodes) {
            cout << item << " ";
        }
        cout << endl;
    }

    return result;
}