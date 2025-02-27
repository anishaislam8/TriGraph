
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
unordered_set<string> get_node_to_add_list_for_a_subgraph(const vector<string>&, const string&, const map<string, unordered_set<string> >&, const map<string, string>&, const map<string, int> &);
float score_of_a_subgraph_with_a_word_from_vocab(const string&, const string&, const vector<string>&, const vector<vector<string> >&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const int, const int, const int);
int predict(const vector<vector <string> >&, const map<string, unordered_set<string> >&, const map<string, string>&, const map<string, int>&, const map<string, int>&, const map<string, int>&, const string&, const int, const int, const int, const map<string, int>&, const vector<string>&, Graph, int);
int predict_edges(const vector<string>&, const map<string, string>&, const map<string, vector<pair<string, float> > >&, const map<string, vector<pair<string, float> > >&, const int, const int, const map<string, int>&, Graph, int);
int predict_edges_baseline(const vector<string>&, const map<string, string>&, const map<string, vector<pair<string, float> > >&, const int, const map<string, int>&, Graph, int);
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
vector<pair<string, float> > create_heap(const vector<pair<string, float> >&);
pair<string, float> get_two_gram_pair_for_heap(const auto&,const auto&, const auto&, const int);
map<string, vector<pair<string, float> > > get_frequency_2_gram_map(const map<string, int>&);
map<string, vector<pair<string, float> > > get_frequency_3_gram_map(const map<string, int>&, const int);
vector<string> load_observed_three_grams();
map<string, unordered_set<string> > get_map_two_grams_to_connections();