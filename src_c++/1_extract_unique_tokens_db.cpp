# include <bits/stdc++.h>
# include <sqlite3.h>
# include "utils.h"
# include "json.hpp"
using json = nlohmann::json;
using namespace std;



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

    while(!myfile.eof()){
        string line;
        getline(myfile, line);

        // for each of the hash ids access the sqlite db and get content
        if (line.empty()) {
            continue;
        }
        
        sqlite3_stmt* stmt;
        string hash_id = line;
        const char* sql = "SELECT Content FROM Contents WHERE Hash = ?;";
        string content;

        

        // Prepare the SQL statement
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return rc;
        }

        // Bind the hash_id to the parameter in the SQL statement
        rc = sqlite3_bind_text(stmt, 1, hash_id.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return rc;
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
        }

        // Finalize the statement to release resources
        sqlite3_finalize(stmt);


        // Output the retrieved content
        if (!content.empty()) {
            cout << "Retrieved content: " << content << endl;
        } else {
            cout << "No content found for hash id: " << hash_id << endl;
        }
        json data = json::parse(content);
        cout << data["nodes"] << endl;


    }
    myfile.close();
    sqlite3_close(db);
    return 0;

}