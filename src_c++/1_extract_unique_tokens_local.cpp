# include <bits/stdc++.h>
// # include <sqlite3.h>
# include "graph.h"
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
            string object_text_string =  tokens[0] + "_" + tokens[1];
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


int main(){
    ifstream myfile;
    myfile.open("../sample_jsons/sample2.json");
    string content( (istreambuf_iterator<char>(myfile) ),
                       (std::istreambuf_iterator<char>()    ) );
   
    json data = json::parse(content);

    try{
        auto connections = data["connections"];
        
        
        vector<string> sources;
        vector<string> destinations;
        vector<vector<string> > edges;
        for (auto connection: connections){
            string source = connection["patchline"]["source"][0];
            string destination = connection["patchline"]["destination"][0];
            vector<string> edge = {source, destination};
            edges.push_back(edge);
            
            sources.push_back(source);
            destinations.push_back(destination);
            
        }

        // if I have no sources or destinations, then I have no connections

        if (sources.size() == 0 || destinations.size() == 0){
            exit(0); // switch to continue when in db
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
        // how to access
        // cout << object_dict.at("PD-ROOT_obj-1") << endl;


        // create a Graph
        Graph G_directed(nodes, edges);

        set<string> unique_tokens = get_unique_tokens(nodes, object_dict);
        map<string, int> frequency_1_gram = get_frequency_1_gram(unique_tokens, object_dict, nodes);

        cout << "frequency_1_gram: \n";
        for (auto token: frequency_1_gram){
            cout << token.first << " " << token.second << endl;
        }




        

        
    }
    catch(...){
        cout << "Exception occured" << endl;
    }

    
    myfile.close();

    return 0;

}