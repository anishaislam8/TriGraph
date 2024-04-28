import networkx as nx
import json

from utils import *
from probability_estimator import *


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

    unique_tokens, frequency_1_gram = get_unique_tokens_and_counts(nodes, object_dict)

    print("Unique tokens: ", unique_tokens)
    print("Frequency of each token: ", frequency_1_gram)
    print("\n")

    # Step 2: 2-grams: Then we create adjacency matrices for 2-grams

    adjaceny_matrices_2_grams, frequency_2_grams = get_adjacency_matrices_2_grams(connections, object_dict)

    # Step 3: 3-grams: We have to find three-node subgraphs in the graph, create adjacency matrix and their frequencies

    # get 3 node subgraphs
    three_node_subgraphs = get_3_node_subgraphs(G)

    # get adjacency matrices for 3-grams and the frequency of each 3-gram
    adjacency_matrices_3_grams, frequency_3_grams = get_adjacency_matrices_3_grams(three_node_subgraphs, object_dict, G)



    # Step 4: Now, that we have a bag of graphs and their probabilities -> how probable is a sequence?

    # for now we are using a hard_coded sequence of three nodes, need to make it more flexible
    sample_subgraphs = [
        [["list_append", "list_trim"], ["inlet", "list_append"]],
        [["list_append", "msg"], ["inlet", "list_append"]],
        [["list_append", "msg"], ["msg", "list_trim"]],
        [["msg", "tgl"], ["tgl", "floatatom"]]
    ]
    
    print("******Calculating the probability of a subgraph********\n")
    for sample_subgraph in sample_subgraphs:
        print("Sample subgraph: ", sample_subgraph)
        probability_of_this_subgraph = count_probability(sample_subgraph, frequency_1_gram, frequency_2_grams, frequency_3_grams)
        print("Probability of this subgraph given our corpus: ", probability_of_this_subgraph)
        print("\n")


    # Step 5: Given a subgraph, with a blank token, predict the token in the blank
    print("******Predicting the next token given a sample subgraph with a blank token********\n")

    sample_subgraphs = [
        [["list_append", "BLANK"], ["inlet", "list_append"]], # output should be "list_trim"
        [["list_prepend", "list_trim"],["list_trim", "BLANK"]], # output should be print
        [["list_prepend", "msg"],["BLANK", "msg"]], # output should be print
    ]

    for sample_subgraph in sample_subgraphs:
        next_token = predict_token(sample_subgraph, frequency_1_gram, frequency_2_grams, frequency_3_grams)
        print("Sample subgraph: ", sample_subgraph)
        print("Predicted token: ", next_token)
        print("\n")

    
    




else:
    print("No connections found in the parsed JSON file")