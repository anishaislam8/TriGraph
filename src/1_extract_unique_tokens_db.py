import networkx as nx
import json
import sqlite3
import pickle

from utils import *
from probability_estimator import *


conn = sqlite3.connect("/media/crouton/aislam4/database.db")
cursor = conn.cursor()


def get_content_from_db(hash):
    cursor.execute("SELECT Content FROM Contents WHERE hash = ?", (hash,))
    content = cursor.fetchall()[0][0]
    return content

processed = 0
unique_tokens_train = set()
frequency_1_gram_train = {}

with open("/media/baguette/aislam4/paths/train_test_split/train_hashes.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        
        print("Processed :", processed)
        processed += 1
        line = line.strip()
        content = get_content_from_db(line)
        data = json.loads(content)
        # Step 0: Get the connections, for now just using an example graph, build the graph

        try:
            connections = data["connections"]
            all_objects = data["all_objects"]
        except:
            connections = []
            all_objects = []


        if len(connections) > 0:
            
            object_dict = create_a_dictionary_of_object_id_to_type(all_objects)

            sources = [connection["patchline"]["source"][0] for connection in connections]
            destinations = [connection["patchline"]["destination"][0] for connection in connections]
            nodes = set(sources + destinations)

            G = nx.DiGraph()
            G.add_nodes_from(nodes)
            G.add_edges_from([(connection["patchline"]["source"][0], connection["patchline"]["destination"][0]) for connection in connections])

            # Step 1: 1-grams: Get the unique tokens and count the frequency of each token

            unique_tokens, frequency_1_gram = get_unique_tokens_and_counts(nodes, object_dict)
            unique_tokens_train.update(unique_tokens)
            
            # update the main frequency_1_gram_train dictionary
            for key, value in frequency_1_gram.items():
                if key in frequency_1_gram_train:
                    frequency_1_gram_train[key] += value
                else:
                    frequency_1_gram_train[key] = value
            
            

        else:
            print("No connections found in the parsed JSON file")


# save the unique tokens and frequency_1_gram_train to a pkl file
with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/unique_tokens_train.pkl", "wb") as f:
    pickle.dump(unique_tokens_train, f)

with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_1_gram_train.pkl", "wb") as f:
    pickle.dump(frequency_1_gram_train, f)