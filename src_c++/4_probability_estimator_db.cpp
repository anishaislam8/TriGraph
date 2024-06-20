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
    myfile_test.open("/media/baguette/aislam4/paths/train_test_split/parallel/test_hashes_1.txt");
    
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
    elapsed_time_file.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/elapsed_time_1.txt", ios::app);

    ofstream exception_file;
    exception_file.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/exception_1.txt", ios::app);

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
                exception_file << line << ": 0 connections" << endl;
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

            start = clock();
            for (auto node: nodes_test){
                // This node is in the graph (source or destination)
                // and there is going to be a three node subgraph that contains this node 
                
                vector<vector<string> > three_node_subgraphs_containing_this_node;
                for (auto subgraph: three_node_subgraphs_test){
                    if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                        three_node_subgraphs_containing_this_node.push_back(subgraph);
                    }
                }

                rank = predict(three_node_subgraphs_containing_this_node, three_node_subgraphs_sorted_by_object_dict, object_dict_test, frequency_1_gram, frequency_2_grams, frequency_3_grams, node, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams, unique_tokens_train_map, G_directed_test);
                myfile_output << node << " " << three_node_subgraphs_containing_this_node.size() << " " << object_dict_test.at(node) <<  " " << rank << endl;
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