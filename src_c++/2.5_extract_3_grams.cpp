# include "utils.h"


int main(){
    ifstream myfile;
    myfile.open("/media/baguette/aislam4/paths/train_test_split/parallel/train_hashes_1.txt");
    sqlite3* db;
    const char* dbPath = "/media/crouton/aislam4/database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    ofstream myfile_3_grams;
    myfile_3_grams.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/3_grams_train_1.txt", ios::app);

    vector<string> unique_tokens_train = load_unique_tokens();

    int processing = 0;
    while(!myfile.eof()){

        
        cout << "Processing: " << processing << endl;
        processing += 1;
        

        string line;
        getline(myfile, line);

        // for each of the hash ids access the sqlite db and get content
        if (line.empty()) {
            continue;
        }
        
        string content = get_content_from_db(line, db);
        if(content.empty()) {
            continue;
        }
        json data = json::parse(content);

        try{
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

            // if I have no sources or destinations, then I have no connections, so no need to process this hash

            if (sources.size() == 0 || destinations.size() == 0){
                continue;
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

            Graph G_directed(nodes, edges);
            Graph G_undirected(nodes, undirected_edges);

            vector<vector<string> > three_node_subgraphs = get_three_node_subgraphs(nodes, G_undirected);    

            // sort the three node subgraphs based on the object_dict
                   
           
            for (auto subgraph: three_node_subgraphs){
               
                vector<string> object_dict_values;
                for (auto node: subgraph){
                    object_dict_values.push_back(object_dict[node]);
                }
                sort(object_dict_values.begin(), object_dict_values.end());

                myfile_3_grams << object_dict_values[0] << " " << object_dict_values[1] << " " << object_dict_values[2] << endl;
            }

            

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }

   
    myfile.close();
    myfile_3_grams.close();
    sqlite3_close(db);


    return 0;

}