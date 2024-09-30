
# include "utils.h"
# include <cassert>



int main(){

    // *************************************test score function*****************************************

    // sample corpus
    map<string, int> frequency_3_grams;
    frequency_3_grams["0,1,2,010000010"] = 1;
    frequency_3_grams["0,1,2,010010010"] = 1;
    frequency_3_grams["1,2,3,010000100"] = 2;
    frequency_3_grams["0,2,3,010000100"] = 1;

    map<string, int> frequency_2_grams;
    frequency_2_grams["0,1,0100"] = 1;
    frequency_2_grams["1,2,0100"] = 2;
    frequency_2_grams["0,2,0100"] = 7;
    frequency_2_grams["2,3,0100"] = 2;
    frequency_2_grams["2,1,0100"] = 1;

    map<string, int> frequency_1_gram;
    frequency_1_gram["msg"] = 1;
    frequency_1_gram["floatatom"] = 2;
    frequency_1_gram["tgl"] = 3;
    frequency_1_gram["r"] = 4;

    int sum_frequency_1_gram = 10;
    int sum_frequency_2_grams = 6;
    int sum_frequency_3_grams = 5;

    map<string, int> unique_tokens_train_map;
    unique_tokens_train_map["floatatom"] = 0;
    unique_tokens_train_map["msg"] = 1;
    unique_tokens_train_map["r"] = 2;
    unique_tokens_train_map["tgl"] = 3;

    cout << "Known tokens: \n";
    for (auto token: unique_tokens_train_map){
        cout << token.first << " ";
    }
    cout << endl;
    
    // case 1
    vector<string> nodes = {"a", "b", "c"};
    vector<vector<string> > edges = {{"a", "b"}, {"b", "c"}, {"a", "c"}};

    Graph G(nodes, edges);
   
    map<string, string> object_dict;
    object_dict["a"] = "msg";
    object_dict["b"] = "r";
    object_dict["c"] = "tgl";

    
    vector<string> subgraph = nodes;

    float score_1 = get_score(subgraph, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);

    cout << "Score_1: Score for msg, r, tgl (3 gram exists in corpus): " << score_1 << endl;

    // case 2

    object_dict.clear();
    object_dict["a"] = "floatatom";
    object_dict["b"] = "msg";
    object_dict["c"] = "tgl";

    float score_2 = get_score(subgraph, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);

    cout << "Score_2: Score for floatatom, msg, tgl (3 gram does not exist in corpus): " << score_2 << endl;

    // case 3

    object_dict.clear();
    object_dict["a"] = "floatatom";
    object_dict["b"] = "unknown";
    object_dict["c"] = "tgl";

    float score_3 = get_score(subgraph, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);

    cout << "Score_3: Score for floatatom, unknown, tgl (1 unknown token): " << score_3 << endl;



    // case 4
    object_dict.clear();
    object_dict["a"] = "unknown";
    object_dict["b"] = "unknown";
    object_dict["c"] = "tgl";
 
    float score_4 = get_score(subgraph, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);

    cout << "Score_4: Score for unknown, unknown, tgl (2 unknown tokens): " << score_4 << endl;


    // case 5
    object_dict.clear();
    object_dict["a"] = "unknown";
    object_dict["b"] = "unknown";
    object_dict["c"] = "unknown";

    float score_5 = get_score(subgraph, object_dict, unique_tokens_train_map, frequency_1_gram, frequency_2_grams, frequency_3_grams, G, sum_frequency_1_gram, sum_frequency_2_grams, sum_frequency_3_grams);

    cout << "Score_5: Score for unknown, unknown, unknown (all unknown tokens): " << score_5 << endl;

    assert(score_1 > score_2 > score_3 > score_4 > score_5);

    cout << "All tests passed for score_1 > score_2 > score_3 > score_4 > score_5 " << endl;


    return 0;


}