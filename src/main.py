import networkx as nx
import json

from utils import *


# Step 0: Get the connections, for now just using an example graph, build the graph


f = open("../sample_jsons/sample2.json", "r")
data = json.loads(f.read())

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

    unique_tokens, token_counts_1_gram = get_unique_tokens_and_counts(nodes, object_dict)

    # Step 2: 2-grams: Then we create adjacency matrices for 2-grams

    adjaceny_matrices_2_grams, frequency_2_grams = get_adjacency_matrices_2_grams(connections, object_dict)

    # Step 3: 3-grams: We have to find three-node connected components in the graph, create adjacency matrices for 3-grams and count the frequency of each 3-gram


    # Step 4: Now, that we have a bag of graphs and their probabilities -> how probable is a sequence?

else:
    print("No connections found in the parsed JSON file")