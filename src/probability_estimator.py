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

    # Step 1: We should have a bag of 1-grams with their probabilities. Get the unique tokens and count the frequency of each token

    unique_tokens, token_counts_1_gram = get_unique_tokens_and_counts(nodes, object_dict)

    # Step 2: Then we create adjacency matrices for 2-grams

    print("Unique tokens: ", unique_tokens)

    # Step 3: Same for three-grams
    # Step 4: Now, that we have a bag of graphs and their probabilities -> how probable is a sequence?

else:
    print("No connections found in the parsed JSON file")