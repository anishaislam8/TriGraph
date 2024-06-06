
# include <iostream>
# include <fstream>
# include <time.h>
# include <set>
# include <algorithm>
# include <utility>
# include <openssl/sha.h>
# include <sqlite3.h>
# include "graph.h"
# include "json.hpp"
using json = nlohmann::json;


string sha256(const string&);
map<string, string> create_object_dict(const json&);
int* create_two_node_adjacency_matrix(const string&, const string&, Graph);
vector<vector<string> > get_three_node_subgraphs(const vector<string>&, Graph);
int* create_three_node_adjacency_matrix(const string&, const string&, const string&, Graph);
bool comparator(const string&, const string&, const map<string, string>&);
float get_score(const vector<string>&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, Graph, const int, const int, const int);
float predict(const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const set<string>&, const string&, const int, const int, const int, const map<string, int>&, const vector<string>&, const vector<vector<string> >&, const string&);
vector<string> load_unique_tokens();
map<string, int> load_frequency_1_gram();
map<string, int> load_frequency_2_grams();
map<string, int> load_frequency_3_grams();
set<vector<string> > get_three_node_subgraphs_sorted_by_object_dict();
set<string> get_unique_tokens(const vector<string>&, const map<string, string>&);
map<string, int> get_frequency_1_gram(const set<string>&, const map<string, string>&, const vector<string>&);
map<string, int> get_frequency_2_grams(const vector<vector<string> >&, const map<string, string>&, const vector<string>&, Graph);
map<string, int> get_frequency_3_grams(const vector<vector<string> >&, const map<string, string>&, const vector<string>&, Graph);
string get_content_from_db(string, sqlite3*);
vector<string> find_the_set_difference(const vector<string>&, const vector<string>&);


