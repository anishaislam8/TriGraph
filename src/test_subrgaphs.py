import networkx as nx
import json
import itertools

from utils import create_a_dictionary_of_object_id_to_type, get_3_node_subgraphs


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

    # sources = [connection["patchline"]["source"][0] for connection in connections]
    # destinations = [connection["patchline"]["destination"][0] for connection in connections]
    # nodes = set(sources + destinations)\

    nodes = ['a', 'b', 'c', 'd', 'e', 'f']
    edges = [('a', 'b'), ('c', 'b'), ('b', 'd'), ('e', 'd'), ('d', 'f')]

    G = nx.DiGraph()
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)

    # get combination of three nodes
    three_nodes = list(itertools.combinations(G.nodes, 3))
    three_node_subgraphs = get_3_node_subgraphs(G)
    print(three_node_subgraphs)

