# include "utils.h"

int main(){
   
    map<string, int> frequency_1_gram = load_frequency_1_gram();
    map<string, int> frequency_2_grams = load_frequency_2_grams();
    map<string, int> frequency_3_grams = load_frequency_3_grams();
    vector<string> unique_tokens_train = load_unique_tokens();
    
    
    // testing

    ifstream myfile_test;
    myfile_test.open("../sample_jsons/sample2.json");
    string content_test( (istreambuf_iterator<char>(myfile_test) ),
                       (istreambuf_iterator<char>()    ) );
   
    json data_test = json::parse(content_test);

    map<string, string> object_dict_test = create_object_dict(data_test);
    int total_predictions_for_this_graph = 0;
    
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
        
 
        
        // for each subgraph, I am going to calculae the mrr score
        float mrr_score = 0.0;
        int count = 0;
        cout << "starting prediction" << endl;
        for (auto node: nodes_test){
            // get all three_node_subgraphs that contain this node
            vector<vector<string> > three_node_subgraphs_containing_this_node;
            for (auto subgraph: three_node_subgraphs_test){
                if (find(subgraph.begin(), subgraph.end(), node) != subgraph.end()){
                    three_node_subgraphs_containing_this_node.push_back(subgraph);
                }
            }

            total_predictions_for_this_graph += three_node_subgraphs_containing_this_node.size();
            
            clock_t start, end;
            double elapsed_time;

            for (auto subgraph: three_node_subgraphs_containing_this_node){
                start = clock();
                mrr_score += predict(subgraph, object_dict_test, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_directed_test, node);
                end = clock();
                elapsed_time = double(end - start) / CLOCKS_PER_SEC;
                cout << "One iteration of the entire vocabulary takes: " << elapsed_time << " seconds" <<  endl;
                cout << "Predicted: " << count << endl;
                count +=1;
            }

        }
        

        mrr_score = mrr_score/total_predictions_for_this_graph;
        cout << "MRR score for this program: " << mrr_score << endl;


    }
    catch(...){
        cout << "Exception occured while testing" << endl;
    }


    myfile_test.close();


    return 0;

}