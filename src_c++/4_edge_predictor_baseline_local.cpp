# include "utils.h"

int main(int argc, char* argv[]){

    char* test_hashes_file = argv[1];

    map<string, int> frequency_3_grams = load_frequency_3_grams();
    vector<string> unique_tokens_train = load_unique_tokens();

    // sum of all values of frequency_3_grams
    int sum_frequency_3_grams = 0;
    for (auto token: frequency_3_grams){
        sum_frequency_3_grams += token.second;
    }

    // map<string, vector<pair<string, float> > > frequency_2_grams_map = get_frequency_2_gram_map(frequency_2_grams);
    map<string, vector<pair<string, float> > > frequency_3_grams_map = get_frequency_3_gram_map(frequency_3_grams, sum_frequency_3_grams);
    
    map<string, int> unique_tokens_train_map;
    for (int i = 0; i < unique_tokens_train.size(); i++) {
        unique_tokens_train_map[unique_tokens_train[i]] = i;
    }

    // testing

    ifstream myfile_test;
    myfile_test.open(test_hashes_file);
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
            exit(0); // continue in db
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
        
        
        // for each subgraph, I am going to calculae the mrr score
        int rank = -1;
        for (auto subgraph: three_node_subgraphs_test){
            cout << "Subgraph: " << subgraph[0] << ", " << subgraph[1] << ", " << subgraph[2] << endl;
            cout << "Object type of these nodes: " << object_dict_test.at(subgraph[0]) << ", " << object_dict_test.at(subgraph[1]) << ", " << object_dict_test.at(subgraph[2]) << endl;
            rank = predict_edges_baseline(subgraph, object_dict_test, frequency_3_grams_map, sum_frequency_3_grams, unique_tokens_train_map, G_directed_test);
            cout << "Rank for this subgraph adjacency matrix: " << rank << endl;
            cout << endl;
        }
        


    }
    catch(...){
        cout << "Exception occured while testing" << endl;
    }


    myfile_test.close();

    
    return 0;

}