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


frequency_3_grams_all = []

with open("/media/baguette/aislam4/paths/train_test_split/parallel/train_hashes_1.txt", "r") as f:
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

            # Step 3: 3-grams: We have to find three-node subgraphs in the graph, create adjacency matrix and their frequencies

            G_undirected = nx.Graph()
            G_undirected.add_nodes_from(nodes)
            G_undirected.add_edges_from([(connection["patchline"]["source"][0], connection["patchline"]["destination"][0]) for connection in connections])

            all_paths = []
            # for all nodes, find paths less than equal length 3
            for node  in nodes:
                # targets are all nodes in the list except this one
                targets = [x for x in nodes if x != node]
                paths = nx.all_simple_paths(G_undirected, source=node, target=targets, cutoff=2)
                all_paths.extend(paths)

            three_node_subgraphs = set()
            for path in all_paths:
                path = list(set(path))
                # sort the path
                path.sort()
                if len(path) == 3:
                    three_node_subgraphs.add(tuple(path))
            # get adjacency matrices for 3-grams and the frequency of each 3-gram
            
            adjacency_matrices_3_grams, frequency_3_grams = get_adjacency_matrices_3_grams(three_node_subgraphs, object_dict, unique_tokens_train, G)
            frequency_3_grams_all.append(frequency_3_grams)
            # later
            # # update the main frequency_2_grams_train dictionary
            # for key, value in frequency_3_grams.items():
            #     if key in frequency_3_grams_train:
            #         frequency_3_grams_train[key] += value
            #     else:
            #         frequency_3_grams_train[key] = value
            
            

        else:
            print("No connections found in the parsed JSON file")




with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_3_grams_train_1.pkl", "wb") as f:
    pickle.dump(frequency_3_grams_all, f)