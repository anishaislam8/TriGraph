# include "utils.h"


int main(){
    ifstream myfile;
    myfile.open("../sample_jsons/example_basic.json");
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

    //sort it before passing below (in db code we are sorting it when we load unique tokens)
    vector<string> unique_tokens_train_vector(unique_tokens.begin(), unique_tokens.end());
    sort(unique_tokens_train_vector.begin(), unique_tokens_train_vector.end());

    for(auto token: unique_tokens_train_vector){
        unique_tokens_train.push_back(token);
    }

   

    assert(unique_tokens_train.size() == 4);
    assert(find(unique_tokens_train.begin(), unique_tokens_train.end(), "*~") != unique_tokens_train.end());
    assert(find(unique_tokens_train.begin(), unique_tokens_train.end(), "floatatom") != unique_tokens_train.end());
    assert(find(unique_tokens_train.begin(), unique_tokens_train.end(), "dac~") != unique_tokens_train.end());
    assert(find(unique_tokens_train.begin(), unique_tokens_train.end(), "osc~") != unique_tokens_train.end());
    assert(frequency_1_gram["*~"] == 1);
    assert(frequency_1_gram["floatatom"] == 2);
    assert(frequency_1_gram["dac~"] == 1);
    assert(frequency_1_gram["osc~"] == 1);
    assert(unique_tokens_train[0] == "*~");
    assert(unique_tokens_train[1] == "dac~");
    assert(unique_tokens_train[2] == "floatatom");
    assert(unique_tokens_train[3] == "osc~");

    cout << "All tests passed for creating unique tokens corpus and frequency 1 gram!" << endl;
    // step 1 done


    
    // step 2: Extract 2-gram frequencies
    // create a Graph
    Graph G_directed(nodes, edges);

    frequency_2_grams = get_frequency_2_grams(edges, object_dict, unique_tokens_train, G_directed);

    // // print frequency_2_grams
    // cout << "frequency_2_grams: \n";
    // for (auto token: frequency_2_grams){
    //     cout << token.first << " " << token.second << endl;
    // }

    assert(frequency_2_grams["0,1,0100"] == 1);
    assert(frequency_2_grams["0,2,0010"] == 1);
    assert(frequency_2_grams["0,3,0010"] == 1);
    assert(frequency_2_grams["2,3,0100"] == 1);

    cout << "All tests passed for creating frequency 2 grams!" << endl;


    // step 2 done

    
    // step 3: extract 3-gram frequencies

    Graph G_undirected(nodes, undirected_edges);

    vector<vector<string> > three_node_subgraphs = get_three_node_subgraphs(nodes, G_undirected);

    // cout << "G_undirected all paths size: " << G_undirected.all_paths.size() << endl; // output 0 as I did not pass a pointer to this graph
    
    frequency_3_grams = get_frequency_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G_directed);


    
    assert(frequency_3_grams.size() == 4);
    assert(frequency_3_grams["0,1,2,010000100"] == 1);
    assert(frequency_3_grams["0,1,3,010000100"] == 1);
    assert(frequency_3_grams["0,2,3,000001100"] == 1);
    assert(frequency_3_grams["0,2,3,000100100"] == 1);

    cout << "All tests passed for creating frequency 3 grams!" << endl;

    // // print frequency_3_grams
    // cout << "frequency_3_grams: \n";
    // for (auto token: frequency_3_grams){
    //     cout << token.first << " " << token.second << endl;
    // }

    // step 3 done

    
    
    myfile.close();

    
    return 0;

}