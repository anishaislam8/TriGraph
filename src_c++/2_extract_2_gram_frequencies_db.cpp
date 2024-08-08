# include "utils.h"

int main(){
    ifstream myfile;
    myfile.open("/media/baguette/aislam4/paths/train_test_split/train_hashes.txt");
    sqlite3* db;
    const char* dbPath = "/media/crouton/aislam4/database.db";
    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    map<string, int> frequency_2_grams_train;
    vector<string> unique_tokens_train = load_unique_tokens(); // read from file and sorted


    while(!myfile.eof()){
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

            for (auto connection: connections){
                string source = connection["patchline"]["source"][0];
                string destination = connection["patchline"]["destination"][0];
                
                // unidirectional edge
                vector<string> edge = {source, destination};
                edges.push_back(edge);
                
                sources.push_back(source);
                destinations.push_back(destination);
                
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


            // step 2: Extract 2-gram frequencies
            // create a Graph
            Graph G_directed(nodes, edges);

            map<string, int> frequency_2_grams = get_frequency_2_grams(edges, object_dict, unique_tokens_train, G_directed);

            // update the final freuqency 2 grams after each hash content
            for (auto token: frequency_2_grams){
                if (frequency_2_grams_train.find(token.first) == frequency_2_grams_train.end()){
                    frequency_2_grams_train[token.first] = token.second;
                }
                else{
                    frequency_2_grams_train[token.first] += token.second;
                }
            }

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }


    // save frequency_2_grams_train to a file
    ofstream myfile_frequency_2_grams_train;
    myfile_frequency_2_grams_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/frequency_2_grams_train.txt");
    for (auto token: frequency_2_grams_train){
        myfile_frequency_2_grams_train << token.first << " " << token.second << endl;
    }

    myfile_frequency_2_grams_train.close();
    myfile.close();
    sqlite3_close(db);


    return 0;

}