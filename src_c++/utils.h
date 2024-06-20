
# include <iostream>
# include <fstream>
# include <time.h>
# include <set>
# include <algorithm>
# include <utility>
# include <sqlite3.h>
# include "graph.h"
# include "json.hpp"
using json = nlohmann::json;


map<string, string> create_object_dict(const json&);
vector<int> create_two_node_adjacency_matrix(const string&, const string&, Graph);
vector<vector<string> > get_three_node_subgraphs(const vector<string>&, Graph);
vector<int> create_three_node_adjacency_matrix(const string&, const string&, const string&, Graph);
bool comparator(const string&, const string&, const map<string, string>&);
float get_score(const vector<string>&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, Graph, const int, const int, const int);
set<string> get_node_to_add_list_for_a_subgraph(const vector<string>&, const string&, const set<vector<string> >&, const map<string, string>&);
float score_of_a_subgraph_with_a_word_from_vocab(const string&, const string&, const vector<string>&, const vector<vector<string> >&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const int, const int, const int);
float predict(const vector<vector <string> >&, const set<vector<string> >&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const string&, const int, const int, const int, const map<string, int>&, Graph);
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
string get_key(const vector<int>&, const vector<int>&);


