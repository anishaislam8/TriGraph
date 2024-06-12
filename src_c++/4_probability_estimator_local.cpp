# include "utils.h"

int main(){
   
    map<string, int> frequency_1_gram = load_frequency_1_gram();
    map<string, int> frequency_2_grams = load_frequency_2_grams();
    map<string, int> frequency_3_grams = load_frequency_3_grams();
    vector<string> unique_tokens_train = load_unique_tokens();
    set<vector<string> > three_node_subgraphs_sorted_by_object_dict = get_three_node_subgraphs_sorted_by_object_dict();

    // testing

    ifstream myfile_test;
    myfile_test.open("../sample_jsons/sample2.json");
    string content_test( (istreambuf_iterator<char>(myfile_test) ),
                       (istreambuf_iterator<char>()    ) );
   
    json data_test = json::parse(content_test);

    map<string, string> object_dict_test = create_object_dict(data_test);
    
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
            continue; // switch to continue when in db
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
        clock_t start, end;
        double elapsed_time;


        for (auto node: nodes_test){
            // This node is in the graph (source or destination)
            // and there is going to be a three node subgraph that contains this node 
            
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

                // now node_to_add list could be empty since this is a test graph, and I might not have seen this node before during train
                
                if (node_to_add_list.size() == 0){
                    // iterate through the entire vocabulary, so node to add list is unique_tokens_train
                    for (auto token: unique_tokens_train){
                        node_to_add_list.insert(token);
                    }
                }

                
                start = clock();
                rank = predict(object_dict_test, frequency_1_gram, frequency_2_grams, frequency_3_grams, node_to_add_list, node, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams, unique_tokens_train_map, subgraph_nodes, subgraph_edges, true_token);
                end = clock();
                elapsed_time = double(end - start) / CLOCKS_PER_SEC;
                cout << "Time to iterate: " << elapsed_time << endl;
                cout << "Rank: " << rank << endl;
            }


        }
        


    }
    catch(...){
        cout << "Exception occured while testing" << endl;
    }


    myfile_test.close();

    
    return 0;

}