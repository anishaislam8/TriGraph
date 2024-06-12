# include "utils.h"

int main(){
   
    map<string, int> frequency_1_gram = load_frequency_1_gram();
    map<string, int> frequency_2_grams = load_frequency_2_grams();
    map<string, int> frequency_3_grams = load_frequency_3_grams();
    vector<string> unique_tokens_train = load_unique_tokens();
    set<vector<string> > three_node_subgraphs_sorted_by_object_dict = get_three_node_subgraphs_sorted_by_object_dict();

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

    map<string, int> unique_tokens_train_map;
    for (int i = 0; i < unique_tokens_train.size(); i++) {
        unique_tokens_train_map[unique_tokens_train[i]] = i;
    }
    

    ifstream myfile_test;
    myfile_test.open("/media/baguette/aislam4/paths/train_test_split/train_hashes.txt");
    
    sqlite3* db;
    const char* dbPath = "/media/crouton/aislam4/database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    int processing = 0;
    int rank;
    clock_t start, end;
    double elapsed_time;

    ofstream elapsed_time_file;
    elapsed_time_file.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/elapsed_time.txt", ios::app);

    ofstream exception_file;
    exception_file.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/exception.txt", ios::app);

    while(!myfile_test.eof()){
        string line;
        getline(myfile_test, line);

        // for each of the hash ids access the sqlite db and get content
        if (line.empty()) {
            continue;
        }
        
        string content = get_content_from_db(line, db);
        if(content.empty()) {
            continue;
        }

        cout << "Processing: " << processing << endl;
        processing += 1;

        json data_test = json::parse(content);

        // create new object dict for all graphs
        map<string, string> object_dict_test = create_object_dict(data_test);
        for (auto token: unique_tokens_train){
            object_dict_test[token] = token;
        }

        
        // write to a file line.txt
        ofstream myfile_output;
        myfile_output.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/rank/" + line + ".txt", ios::app);


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

            

            vector<vector<string> > three_node_subgraphs_test = get_three_node_subgraphs(nodes_test, G_undirected_test);
            
            // for each subgraph, I am going to calculate the rank

            start = clock();
            for (auto node: nodes_test){
                // get all three_node_subgraphs that contain this node
                vector<vector<string> > three_node_subgraphs_containing_this_node;
                for (auto subgraph: three_node_subgraphs_test){
                    if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                        three_node_subgraphs_containing_this_node.push_back(subgraph);
                    }
                }

                string true_token = object_dict_test.at(node);
                
                // I am calling the predict function for each three_node_subgraph that contains this node
                for (auto subgraph: three_node_subgraphs_containing_this_node){
                    
                    vector<string> subgraph_nodes = subgraph;
                    set<string> subgraph_nodes_set(subgraph.begin(), subgraph.end());
                    vector<vector<string> > subgraph_edges;
                    vector<vector<string> > edges_of_original_graph = G_directed_test.get_edges();

                    // this is okay, if edge is (0,0) and nodes set is (0,1,2) -> add this edge
                    for (const auto& edge : edges_of_original_graph) {
                        if (subgraph_nodes_set.count(edge[0]) > 0 && subgraph_nodes_set.count(edge[1]) > 0) {
                            subgraph_edges.push_back(edge);
                        }
                    }


                    // create node to add list for this subgraph

                    vector<string> two_nodes;
                    for (auto item: subgraph){
                        if (item != node){ // take the ones except for this one
                            two_nodes.push_back(object_dict_test.at(item));
                        }
                    }


                    // find the third node in three_node_subgraphs_sorted_by_object_dict which has these two nodes
                    set<string> node_to_add_list;

                    for (auto subgraph_inner: three_node_subgraphs_sorted_by_object_dict){
                        vector<string> result;
                        if (two_nodes[0] == two_nodes[1]){
                            if (count(subgraph_inner.begin(), subgraph_inner.end(), two_nodes[0]) >= 2){
                                result = find_the_set_difference(subgraph_inner, two_nodes);
                                node_to_add_list.insert(result[0]);
                            }
                        }
                        else{
                            if (count(subgraph_inner.begin(), subgraph_inner.end(), two_nodes[0]) >= 1 && count(subgraph_inner.begin(), subgraph_inner.end(), two_nodes[1]) >= 1){
                                result = find_the_set_difference(subgraph_inner, two_nodes);
                                node_to_add_list.insert(result[0]);
                            }

                        }
                    } 
                    
                    if (node_to_add_list.size() == 0){
                        // iterate through the entire vocabulary, so node to add list is unique_tokens_train
                        for (auto token: unique_tokens_train){
                            node_to_add_list.insert(token);
                        }
                    }
                    
                    rank = predict(object_dict_test, frequency_1_gram, frequency_2_grams, frequency_3_grams, node_to_add_list, node, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams, unique_tokens_train_map, subgraph_nodes, subgraph_edges, true_token);

                    // write to the file the subgraph nodes, blank nodes, rank
                    for (auto sub_node: subgraph_nodes){
                        myfile_output << object_dict_test.at(sub_node) << " ";
                    }

                    myfile_output << object_dict_test.at(node) << " " << true_token << " " << rank << endl;
                    
                }

            }

            end = clock();
            elapsed_time = double(end - start) / CLOCKS_PER_SEC;
            elapsed_time_file << line << " " << elapsed_time << endl;
            


        }
        catch(const exception& e){
            exception_file << line << ": Exception: " << e.what() << endl;
        }

        myfile_output.close();
    }

    myfile_test.close();
    elapsed_time_file.close();
    exception_file.close();
    sqlite3_close(db);
    
    return 0;

}