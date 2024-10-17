#include "utils.h"

int main(int argc, char* argv[]){

    char* train_hashes_file = argv[1];
    const char* dbPath = argv[2];

    ifstream myfile;
    myfile.open(train_hashes_file);
    sqlite3* db;

    // Open the database
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    map<string, int> frequency_1_gram_train;
    set<string> unique_tokens_train_set;
    vector<string> unique_tokens_train;

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

            for (auto connection: connections){
                string source = connection["patchline"]["source"][0];
                string destination = connection["patchline"]["destination"][0];

                
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

            set<string> unique_tokens = get_unique_tokens(nodes, object_dict);
            // update unique tokens after each hash content
            unique_tokens_train_set.insert(unique_tokens.begin(), unique_tokens.end());

            map<string, int> frequency_1_gram = get_frequency_1_gram(unique_tokens, object_dict, nodes);
            
            // update the final freuqency 1 gram after each hash content
            for (auto token: frequency_1_gram){
                if (frequency_1_gram_train.find(token.first) == frequency_1_gram_train.end()){
                    frequency_1_gram_train[token.first] = token.second;
                }
                else{
                    frequency_1_gram_train[token.first] += token.second;
                }
            }

        
        }
        catch(...){
            cout << "Exception occured" << endl;
        }

        


    }
    unique_tokens_train = vector<string>(unique_tokens_train_set.begin(), unique_tokens_train_set.end());

    // save unique_tokens_train to a file
    ofstream myfile_unique_tokens_train;
    myfile_unique_tokens_train.open("vocabulary_frequencies/unique_tokens_train.txt");
    for (auto token: unique_tokens_train){
        myfile_unique_tokens_train << token << endl;
    }

    myfile_unique_tokens_train.close();

    // save frequency_1_gram_train to a file
    ofstream myfile_frequency_1_gram_train;
    myfile_frequency_1_gram_train.open("vocabulary_frequencies/frequency_1_gram_train.txt");
    for (auto token: frequency_1_gram_train){
        myfile_frequency_1_gram_train << token.first << " " << token.second << endl;
    }

    myfile_frequency_1_gram_train.close();
    myfile.close();
    sqlite3_close(db);


    return 0;

}