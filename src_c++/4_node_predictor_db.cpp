# include "utils.h"

int main(int argc, char* argv[]){

    char* test_hashes_file = argv[1];
    const char* dbPath = argv[2];
   
    map<string, int> frequency_1_gram = load_frequency_1_gram();
    map<string, int> frequency_2_grams = load_frequency_2_grams();
    map<string, int> frequency_3_grams = load_frequency_3_grams();
    vector<string> unique_tokens_train = load_unique_tokens();
    map <string, unordered_set<string> > two_grams_to_connections = get_map_two_grams_to_connections();
    
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
    myfile_test.open(test_hashes_file);
    
    sqlite3* db;

    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    int processing = 0;
    int rank;

    // ofstream exception_file;
    // exception_file.open("/media/crouton/aislam4/Probability-Estimator-For-Visual-Code/src_c++/exception_1.txt", ios::app);

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

        // cout << "Processing: " << processing << endl;
        // processing += 1;

        json data_test = json::parse(content);

        // create new object dict for all graphs
        map<string, string> object_dict_test = create_object_dict(data_test);
        for (auto token: unique_tokens_train){
            object_dict_test[token] = token;
        }

        
        // write to a file line.txt
        // ofstream myfile_output;
        // myfile_output.open("/media/crouton/aislam4/Probability-Estimator-For-Visual-Code/src_c++/node_rank/" + line + ".txt", ios::app);


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
                //exception_file << line << ": 0 connections" << endl;
                continue;
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

            for (auto node: nodes_test){
                // This node is in the graph (source or destination)
                // and there is going to be a three node subgraph that contains this node 
                
                // if this is empty then, all the ranks will be -1 for this node
                vector<vector<string> > three_node_subgraphs_containing_this_node;
                for (auto subgraph: three_node_subgraphs_test){
                    if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                        three_node_subgraphs_containing_this_node.push_back(subgraph);
                    }
                }

                rank = predict(three_node_subgraphs_containing_this_node, two_grams_to_connections, object_dict_test, frequency_1_gram, frequency_2_grams, frequency_3_grams, node, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams, unique_tokens_train_map, unique_tokens_train, G_directed_test);
                cout << "Rank for this node: "  << node << " " << rank << endl; 
                //myfile_output << node << " " << three_node_subgraphs_containing_this_node.size() << " " << object_dict_test.at(node) <<  " " << rank << endl;
                
            }

            


        }
        catch(const exception& e){
            //exception_file << line << ": Exception: " << e.what() << endl;
        }

       // myfile_output.close();
    }

    myfile_test.close();
    //exception_file.close();
    sqlite3_close(db);
    
    return 0;

}