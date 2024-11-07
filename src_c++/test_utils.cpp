
# include "utils.h"
# include <cassert>

void test_get_frequency_3_gram_map(){
    map<string, int> frequency_3_grams;
    frequency_3_grams["0,1,2,010000010"] = 1;
    frequency_3_grams["0,1,2,010010010"] = 1;
    frequency_3_grams["1,2,3,010000100"] = 2;
    frequency_3_grams["0,2,3,010000100"] = 1;

    int sum_frequency_3_grams = 4;

    map<string, vector<pair<string, float> > > frequency_3_gram_map = get_frequency_3_gram_map(frequency_3_grams, sum_frequency_3_grams); // works

    // for (auto item: frequency_3_gram_map){
    //     cout << item.first << ": ";
    //     for (auto token: item.second){
    //         cout << token.first << " " << token.second << " ";
    //     }
    //     cout << endl;
    // }

    assert(frequency_3_gram_map["0,1,2"].size() == 2);
    assert(frequency_3_gram_map["1,2,3"].size() == 1);
    assert(frequency_3_gram_map["0,2,3"].size() == 1);

    cout << "All tests passed for get_frequency_3_gram_map!" << endl;

}


void test_create_three_node_adjacency_matrix(){
    vector<string> nodes = {"0", "1", "2"};
    vector<vector<string> > edges = {{"0", "1"}, {"1", "2"}, {"0", "2"}, {"0", "2"}, {"2", "1"}};

    Graph G(nodes, edges);

    vector<int> adjacency_matrix = create_three_node_adjacency_matrix("0", "1", "2", G); // this function works
    string key = "";
    for (auto entry: adjacency_matrix){
        key += to_string(entry);
    }

    assert(key == "011001010");

    cout << "All tests passed for create_three_node_adjacency_matrix!" << endl;
}

void test_get_two_gram_pair_for_heap(){
    pair<string, float> p1("0011", 1); // 01
    pair<string, float> p2("0010", 1); // 12
    pair<string, float> p3("0100", 1); // 02

    pair<string, float> pout = get_two_gram_pair_for_heap(p1, p2, p3, 3); // works

    assert(pout.first == "001110010");

    cout << "All tests passed for get_two_gram_pair_for_heap!" << endl;
}


void test_get_map_two_grams_to_connections(){
    map<string, unordered_set<string> > two_grams_to_connections;
    vector<string> observed_three_grams = {"0,1,2", "1,2,3", "0,2,3", "0,1,4"};
    for (auto three_grams: observed_three_grams){
        vector<string> tokens;
        string token;
        istringstream tokenStream(three_grams);
        while (getline(tokenStream, token, ',')){
            tokens.push_back(token);
        }
        string first_token = tokens[0];
        string second_token = tokens[1];
        string third_token = tokens[2];

        string key_1 = first_token + "," + second_token;
        string key_2 = second_token + "," + third_token;
        string key_3 = first_token + "," + third_token;

        if (two_grams_to_connections.find(key_1) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_1] = {third_token};
        }
        else{
            two_grams_to_connections[key_1].insert(third_token);
        }

        if (two_grams_to_connections.find(key_2) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_2] = {first_token};
        }
        else{
            two_grams_to_connections[key_2].insert(first_token);
        }

        if (two_grams_to_connections.find(key_3) == two_grams_to_connections.end()){ // not found
            two_grams_to_connections[key_3] = {second_token};
        }
        else{
            two_grams_to_connections[key_3].insert(second_token);
        }

        
    }

    // for (auto item: two_grams_to_connections){
    //     cout << item.first << ": ";
    //     for (auto token: item.second){
    //         cout << token << " ";
    //     }
    //     cout << endl;
    // }

    assert(two_grams_to_connections["0,1"].size() == 2 && two_grams_to_connections["0,1"].find("2") != two_grams_to_connections["0,1"].end() && two_grams_to_connections["0,1"].find("4") != two_grams_to_connections["0,1"].end());
    assert(two_grams_to_connections["1,2"].size() == 2 && two_grams_to_connections["1,2"].find("0") != two_grams_to_connections["1,2"].end() && two_grams_to_connections["1,2"].find("3") != two_grams_to_connections["1,2"].end());
    assert(two_grams_to_connections["0,2"].size() == 2 && two_grams_to_connections["0,2"].find("1") != two_grams_to_connections["0,2"].end() && two_grams_to_connections["0,2"].find("3") != two_grams_to_connections["0,2"].end());
    assert(two_grams_to_connections["2,3"].size() == 2 && two_grams_to_connections["2,3"].find("1") != two_grams_to_connections["2,3"].end() && two_grams_to_connections["2,3"].find("0") != two_grams_to_connections["2,3"].end());
    assert(two_grams_to_connections["0,3"].size() == 1 && two_grams_to_connections["0,3"].find("2") != two_grams_to_connections["0,3"].end());
    assert(two_grams_to_connections["1,3"].size() == 1 && two_grams_to_connections["1,3"].find("2") != two_grams_to_connections["1,3"].end());
    assert(two_grams_to_connections["0,4"].size() == 1 && two_grams_to_connections["0,4"].find("1") != two_grams_to_connections["0,4"].end());
    assert(two_grams_to_connections["1,4"].size() == 1 && two_grams_to_connections["1,4"].find("0") != two_grams_to_connections["1,4"].end());

    cout << "All tests passed for get_map_two_grams_to_connections!" << endl;
}

// void test_load_frequency_maps(){
//     map<string, int> frequency_1_gram = load_frequency_1_gram();
//     map<string, int> frequency_2_grams = load_frequency_2_grams();
//     map<string, int> frequency_3_grams = load_frequency_3_grams();

//     // cout << frequency_3_grams["8137,27519,31595,010000010"] << endl;
//     // cout << frequency_3_grams["11077,11871,28319,010000100"] << endl;
//     // cout << frequency_2_grams["5172,30115,0100"] << endl;
//     // cout << frequency_1_gram["msg"] << endl;

//     assert(frequency_1_gram["msg"] == 2880151);
//     assert(frequency_2_grams["5172,30115,0100"] == 25);
//     assert(frequency_3_grams["8137,27519,31595,010000010"] == 22);
//     assert(frequency_3_grams["11077,11871,28319,010000100"] == 1);

//     cout << "All tests passed for loading frequency maps 1 gram 2 gram and 3 grams!" << endl;
// }

void test_sort_vector_based_on_object_type(){
    
    /***********SORT VECTOR BASED ON OBJECT DICT MAP VALUE***********/
    // test sorting vector

    vector<string> x = {"PD_ROOT_obj_0", "PD_ROOT_obj_1", "PD_ROOT_obj_2", "PD_ROOT_obj_3", "PD_ROOT_obj_4"};

    // Example map that maps strings to other strings
    map<string, string> y = {
       {"PD_ROOT_obj_0", "msg"},
       {"PD_ROOT_obj_1", "msg"},
       {"PD_ROOT_obj_2", "floatatom"},
       {"PD_ROOT_obj_3", "tgl"},
       {"PD_ROOT_obj_4", "r"}
    };

    sort(x.begin(), x.end(), [&y](const string& a, const string& b) {
        return comparator(a, b, y);
    });

    assert(y.at(x[0]) == "floatatom");
    assert(y.at(x[1]) == "msg");
    assert(y.at(x[2]) == "msg");
    assert(y.at(x[3]) == "r");
    assert(y.at(x[4]) == "tgl");

    cout << "All tests passed for sorting vector based on object dict map!" << endl;
}


void test_get_three_node_subgraphs(){
    vector<string> nodes = {"a", "b" , "c", "d","e", "f", "g", "h"};
    vector<vector<string> > edges = {{"a", "b"}, {"a", "c"}, {"a", "d"}, {"b", "e"}, {"b", "f"}, {"c", "g"}, {"c", "h"}, {"b", "a"}, {"c", "a"}, {"d", "a"}, {"e", "b"}, {"f", "b"}, {"g", "c"}, {"h", "c"}};

    Graph graph(nodes, edges);

    vector<vector<string> > three_node_subgraphs = get_three_node_subgraphs(nodes, graph);

    // cout << "Three node subgraphs: \n";
    // for (auto subgraph: three_node_subgraphs){
    //     for (auto node: subgraph){
    //         cout << node << " ";
    //     }
    //     cout << endl;
    // }


    assert(three_node_subgraphs.size() == 9);

    cout << "All tests passed for get_three_node_subgraphs!" << endl;
}

void test_get_node_to_add_list_for_a_subgraph(){
    
    // fake subgraph
    vector<string> subgraph = {"a", "b", "c"};

    // fake node to remove
    string node_to_remove = "b";

    // fake object dict
    map<string, string> object_dict;
    object_dict["a"] = "msg";
    object_dict["b"] = "floatatom";
    object_dict["c"] = "tgl";
    object_dict["d"] = "r";
    object_dict["e"] = "send";
    object_dict["f"] = "receive";
    object_dict["g"] = "print";

    // fake unique tokens train map
    map<string, int> unique_tokens_train_map;
    unique_tokens_train_map["floatatom"] = 0;
    unique_tokens_train_map["msg"] = 1;
    unique_tokens_train_map["tgl"] = 2;
    unique_tokens_train_map["r"] = 3;
    unique_tokens_train_map["send"] = 4;
    unique_tokens_train_map["receive"] = 5;
    unique_tokens_train_map["print"] = 6;


    // fake two grams to connections map
    map<string, unordered_set<string> > two_grams_to_connections;
    two_grams_to_connections["0,1"] = {"2", "3"};
    two_grams_to_connections["0,2"] = {"2", "4", "5"};
    two_grams_to_connections["1,2"] = {"0", "6"};

    unordered_set<string> node_to_add_list = get_node_to_add_list_for_a_subgraph(subgraph, node_to_remove, two_grams_to_connections, object_dict, unique_tokens_train_map);
    
    assert(node_to_add_list.size() == 2);
    assert(node_to_add_list.find("0") != node_to_add_list.end());
    assert(node_to_add_list.find("6") != node_to_add_list.end());

    cout << "All tests passed for get_node_to_add_list_for_a_subgraph!" << endl;


}

int main(){


    // test get_three_node_subgraphs
    test_get_three_node_subgraphs();

    // test sort_vector_based_on_object_type
    test_sort_vector_based_on_object_type();

    // test get_map_two_grams_to_connections
    test_get_map_two_grams_to_connections();

    // test get_two_gram_pair_for_heap
    test_get_two_gram_pair_for_heap();

    // test create_three_node_adjacency_matrix
    test_create_three_node_adjacency_matrix();
    
    // test get_frequency_3_gram_map
    test_get_frequency_3_gram_map();

    // test get_node_to_add_list_for_a_subgraph
    test_get_node_to_add_list_for_a_subgraph();

    return 0;

}