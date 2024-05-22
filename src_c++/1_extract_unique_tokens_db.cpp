# include <bits/stdc++.h>
# include <sqlite3.h>
# include "json.hpp"
using json = nlohmann::json;
using namespace std;


map<string, string> create_object_dict(json data){
    map<string, string> object_dict;
    auto all_objects = data["all_objects"];

    for (auto object: all_objects){
        if(object["box"]["object_type"] == "list"){
            string object_text = object["box"]["text"];
            vector<string> tokens;
            string token;
            istringstream tokenStream(object_text);
            while (getline(tokenStream, token, ' ')){
                tokens.push_back(token);
            }
            string object_text_string;
            if (tokens.size() > 1){
                object_text_string =  tokens[0] + "_" + tokens[1];
            }
            else{
                object_text_string = tokens[0];
            }
            object_dict[object["box"]["id"]] = object_text_string;


        }
        else{
            object_dict[object["box"]["id"]] = object["box"]["object_type"];
        }
    }

    return object_dict;
}


set<string> get_unique_tokens(vector<string> nodes, map<string, string> object_dict){
    set<string> unique_tokens;
    for (string node: nodes){
        unique_tokens.insert(object_dict.at(node));
    }

    return unique_tokens;
}

map<string, int> get_frequency_1_gram(set<string> unique_tokens, map<string, string> object_dict, vector<string> nodes){
    map<string, int> frequency_1_gram;
    for (string token: unique_tokens){
        frequency_1_gram[token] = 0;
    }
    for (string node: nodes){
        frequency_1_gram[object_dict.at(node)] += 1;
    }
    

    return frequency_1_gram;
}


string get_content_from_db(string line, sqlite3* db){
    
    sqlite3_stmt* stmt;
    string hash_id = line;
    const char* sql = "SELECT Content FROM Contents WHERE Hash = ?;";
    string content;

    

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Bind the hash_id to the parameter in the SQL statement
    rc = sqlite3_bind_text(stmt, 1, hash_id.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Retrieve the first column (content)
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            content = reinterpret_cast<const char*>(text);
        }
    } else {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);


    // Output the retrieved content
    if (content.empty()) {
        cout << "No content found for hash id: " << hash_id << endl;
        return "";
    }

    return content;
}

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

            cout << "getting segmentation fault here" << endl;

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
    myfile_unique_tokens_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/unique_tokens_train.txt");
    for (auto token: unique_tokens_train){
        myfile_unique_tokens_train << token << endl;
    }

    myfile_unique_tokens_train.close();

    // save frequency_1_gram_train to a file
    ofstream myfile_frequency_1_gram_train;
    myfile_frequency_1_gram_train.open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src_c++/vocabulary_frequencies/frequency_1_gram_train.txt");
    for (auto token: frequency_1_gram_train){
        myfile_frequency_1_gram_train << token.first << " " << token.second << endl;
    }

    myfile_frequency_1_gram_train.close();
    myfile.close();
    sqlite3_close(db);


    return 0;

}