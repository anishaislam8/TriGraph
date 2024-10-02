#include "utils.h"

int main(){
    ifstream myfile;
    myfile.open("train_hashes.txt");
    sqlite3* db;
    const char* dbPath = "database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    ofstream myfile_frequency_3_grams_train;
    myfile_frequency_3_grams_train.open("vocabulary_frequencies/frequency_3_grams_train_all.txt", ios::app);

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
            map<string, int> frequency_3_grams = get_frequency_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G_directed);

            
            
            for (auto token: frequency_3_grams){
                myfile_frequency_3_grams_train << token.first << " " << token.second << endl;
            }

            

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }

   
    myfile.close();
    myfile_frequency_3_grams_train.close();
    sqlite3_close(db);


    return 0;

}