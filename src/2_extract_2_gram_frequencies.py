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


with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/unique_tokens_train.pkl", "rb") as f:
    unique_tokens_train_set = pickle.load(f)

unique_tokens_train = list(unique_tokens_train_set)
unique_tokens_train.sort()


frequency_2_grams_train = {}

with open("/media/baguette/aislam4/paths/train_test_split/train_hashes.txt", "r") as f:
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

            sources = [connection["patchline"]["source"][0] for connection in connections]
            destinations = [connection["patchline"]["destination"][0] for connection in connections]
            nodes = set(sources + destinations)

            G = nx.DiGraph()
            G.add_nodes_from(nodes)
            G.add_edges_from([(connection["patchline"]["source"][0], connection["patchline"]["destination"][0]) for connection in connections])

            adjaceny_matrices_2_grams, frequency_2_grams = get_adjacency_matrices_2_grams(connections, object_dict, unique_tokens_train, G)
            
            # update the main frequency_2_grams_train dictionary
            for key, value in frequency_2_grams.items():
                if key in frequency_2_grams_train:
                    frequency_2_grams_train[key] += value
                else:
                    frequency_2_grams_train[key] = value
            
            

        else:
            print("No connections found in the parsed JSON file")




with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_2_grams_train.pkl", "wb") as f:
    pickle.dump(frequency_2_grams_train, f)