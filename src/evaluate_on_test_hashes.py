import networkx as nx
import json
import sqlite3
import pickle

from utils import *
from probability_estimator_test_hashes import *


# load everything

conn = sqlite3.connect("/media/crouton/aislam4/database.db")
cursor = conn.cursor()


def get_content_from_db(hash):
    cursor.execute("SELECT Content FROM Contents WHERE hash = ?", (hash,))
    content = cursor.fetchall()[0][0]
    return content

processed = 0


with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/unique_tokens_train.pkl", "rb") as f:
    unique_tokens_train_set = pickle.load(f)

unique_tokens_train = list(unique_tokens_train_set)
unique_tokens_train.sort()

with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_1_gram_train.pkl", "rb") as f:
    frequency_1_gram = pickle.load(f)

with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_2_grams_train.pkl", "rb") as f:
    frequency_2_grams = pickle.load(f)

with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_3_grams_train.pkl", "rb") as f:
    frequency_3_grams = pickle.load(f)

correct_predictions = 0
total_predictions = 0
with open("/media/baguette/aislam4/paths/train_test_split/test_hashes_filtered.txt", "r") as f:
    lines = f.readlines()
    
    for line in lines:
        
        print("Processed :", processed)
        processed += 1
        line = line.strip()
        content = get_content_from_db(line)
        data = json.loads(content)

        try:
            connections = data["connections"]
            all_objects = data["all_objects"]
        except:
            connections = []
            all_objects = []


        if len(connections) > 0:
            
            object_dict = create_a_dictionary_of_object_id_to_type(all_objects)

            for word in frequency_1_gram.keys():
                object_dict[word] = word

            sources = [connection["patchline"]["source"][0] for connection in connections]
            destinations = [connection["patchline"]["destination"][0] for connection in connections]
            nodes = set(sources + destinations)
        
            G = create_directed_graph(connections, all_objects)
            
            # Step 3: 3-grams: We have to find three-node subgraphs in the graph, create adjacency matrix and their frequencies

            G_undirected = create_undirected_graph(connections, all_objects)
            
            three_node_subgraphs = get_3_node_subgraphs_v2(nodes, G_undirected)

            total_predictions += len(three_node_subgraphs)

            for subgraph in three_node_subgraphs:
                next_token_correctly_predicted = predict(subgraph, object_dict, unique_tokens_train, G, frequency_1_gram, frequency_2_grams, frequency_3_grams)
                correct_predictions += next_token_correctly_predicted
        else:
            print("No connections found in the parsed file")


print("Total predictions: ", total_predictions)
print("Correct predictions: ", correct_predictions)
print("Accuracy: ", correct_predictions / total_predictions)



