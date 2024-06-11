# include "utils.h"


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

    // set<vector<string> > three_node_subgraphs_sorted_by_object_dict;
    // for (auto subgraph: three_node_subgraphs){
               
    //     vector<string> object_dict_values;
    //     for (auto node: subgraph){
    //         object_dict_values.push_back(object_dict[node]);
    //     }
    //     sort(object_dict_values.begin(), object_dict_values.end());
    //     three_node_subgraphs_sorted_by_object_dict.insert(object_dict_values);
    // }

    
    frequency_3_grams = get_frequency_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G_directed);


    
    // cout << "frequency 3 gram size: " << frequency_3_grams.size() << endl;

    // // print frequency_3_grams
    // cout << "frequency_3_grams: \n";
    // for (auto token: frequency_3_grams){
    //     cout << token.first << " " << token.second << endl;
    // }

    // step 3 done

    
    myfile.close();


    /*
    // testing

    ifstream myfile_test;
    myfile_test.open("../sample_jsons/sample2.json");
    string content_test( (istreambuf_iterator<char>(myfile_test) ),
                       (istreambuf_iterator<char>()    ) );
   
    json data_test = json::parse(content_test);

    map<string, string> object_dict_test = create_object_dict(data_test);
    int total_predictions_for_this_graph = 0;

    clock_t start, end;
    double elapsed_time;
    
    
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

        // for each subgraph, I am going to calculae the mrr score
        int rank = -1;

        for (auto node: nodes_test){
            // get all three_node_subgraphs that contain this node
            vector<vector<string> > three_node_subgraphs_containing_this_node;
            for (auto subgraph: three_node_subgraphs_test){
                if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                    three_node_subgraphs_containing_this_node.push_back(subgraph);
                }
            }

            string true_token = object_dict_test.at(node);
            

            for (auto subgraph: three_node_subgraphs_containing_this_node){
                
                vector<string> subgraph_nodes = subgraph;
                set<string> subgraph_nodes_set(subgraph.begin(), subgraph.end());
                vector<vector<string> > subgraph_edges;
                vector<vector<string> > edges_of_original_graph = G_directed_test.get_edges();

                for (const auto& edge : edges_of_original_graph) {
                    if (subgraph_nodes_set.count(edge[0]) > 0 && subgraph_nodes_set.count(edge[1]) > 0) {
                        subgraph_edges.push_back(edge);
                    }
                }

                vector<string> two_nodes;
                for (auto item: subgraph){
                    if (item != node){ // take the ones except for this one
                        two_nodes.push_back(object_dict_test.at(item));
                    }
                }

                // find the third node in three_node_subgraphs_sorted_by_object_dict which has these two nodes
                set<string> node_to_add_list;
                for (auto subgraph: three_node_subgraphs_sorted_by_object_dict){
                    vector<string> result;
                    if (two_nodes[0] == two_nodes[1]){
                        if (count(subgraph.begin(), subgraph.end(), two_nodes[0]) >= 2){
                            result = find_the_set_difference(subgraph, two_nodes);
                            node_to_add_list.insert(result[0]);
                        }
                    }
                    else{
                        if (count(subgraph.begin(), subgraph.end(), two_nodes[0]) >= 1 && count(subgraph.begin(), subgraph.end(), two_nodes[1]) >= 1){
                            result = find_the_set_difference(subgraph, two_nodes);
                            node_to_add_list.insert(result[0]);
                        }

                    }
                } 
                
                if (node_to_add_list.size() == 0){
                    continue;
                }
                
                start = clock();
                rank = predict(object_dict_test, frequency_1_gram, frequency_2_grams, frequency_3_grams, node_to_add_list, node, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams, unique_tokens_train_map, subgraph_nodes, subgraph_edges, true_token);
                end = clock();
                elapsed_time = double(end - start) / double(CLOCKS_PER_SEC);
                cout << "Time taken: " << elapsed_time << " seconds" << endl;

            }

        }
        

    }
    catch(const exception& e){
        cout << "Exception occured while testing: " << e.what() << endl;
    }

    

    myfile_test.close();

    */


    return 0;

}