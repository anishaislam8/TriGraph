import hashlib
import networkx as nx
import itertools
import copy

def create_a_dictionary_of_object_id_to_type(all_objects):
    
    '''
    Create a dictionary of object_id to object_type
    For example, if the object_id is PD-ROOT_obj-0 and the object_type is "list_append", then the dictionary entry will be {"PD-ROOT_obj-0": "list_append"}
    '''

    object_dict = {}
    for obj in all_objects:
        if obj["box"]["object_type"] in ["list"]: # because for list we need the second argument
            obj_text = obj["box"]["text"].split(" ")[:2]
            obj_text_str = "_".join(obj_text)
            object_dict[obj["box"]["id"]] = obj_text_str
        else:
            object_dict[obj["box"]["id"]] = obj["box"]["object_type"]
    return object_dict


def get_unique_tokens_and_counts(nodes, object_dict):

    '''
    Initially, the number of unique tokens should be the number of unique object_dict[nodes[i]] values and we also want the count
    of each of these unique tokens. We can then calculate the probability of each of these tokens by dividing the count of the token
    by the total number of tokens. In this case, total number of tokens is the total number of nodes in the graph.

    probabilities = count_of_token/total_number_of_tokens
    '''

    # count the number of unique tokens
    unique_tokens = set(object_dict[node] for node in nodes)

    # count the number of each token
    token_counts_1_gram = {token: 0 for token in unique_tokens}
    for node in nodes:
        token_counts_1_gram[object_dict[node]] += 1

    return unique_tokens, token_counts_1_gram


# calculate_sha256() collected from https://unogeeks.com/python-sha256/
def calculate_sha256(data):

    '''
    Get the hash of a string using the SHA-256 algorithm.
    '''

    # Convert data to bytes if it’s not already
    if isinstance(data, str):
        data = data.encode()

    # Calculate SHA-256 hash
    sha256_hash = hashlib.sha256(data).hexdigest()

    return sha256_hash


def create_two_node_adjacency_matrix(first_node, second_node, G):
    
    '''
    Create a 2 by 2 matrix for the two nodes
    '''
    
    entry00 = 1 if G.has_edge(first_node, first_node) else 0
    entry01 = 1 if G.has_edge(first_node, second_node) else 0
    entry10 = 1 if G.has_edge(second_node, first_node) else 0
    entry11 = 1 if G.has_edge(second_node, second_node) else 0

    adjacency_matrix = [entry00, entry01, entry10, entry11]
    return adjacency_matrix


def create_two_node_adjacency_matrix_v2(first_node, second_node, G):
    
    '''
    Create a 2 by 2 matrix for the two nodes
    '''
    
    entry00 = 1 if first_node in G[first_node] else 0
    entry01 = 1 if second_node in G[first_node] else 0
    entry10 = 1 if first_node in G[second_node] else 0
    entry11 = 1 if second_node in G[second_node] else 0

    adjacency_matrix = [entry00, entry01, entry10, entry11]
    return adjacency_matrix

def get_adjacency_matrices_2_grams(connections, object_dict, unique_nodes, G):
    
    '''
    If we want to build adjacency matrices for 2-grams, there are some scenarios we need to consider:
    1. There could be multiple connections between two nodes. For generating an adjacency matrix, we skipped duplicate connections.
    2. But for counting the frequency of 2-grams, we need to consider all connections. So, for duplicate connections, we increment the frequency of the 2-gram.
    3. If there is a connection from a to b, we create one key for it, and if there is a connection from b to a, we create another key for it. As these are different subgraphs.
    4. If there is a similar subgraph detected later in the graph, we keep the adjacency matrix the same but increment the frequency of the 2-gram.
    '''
    
    adjacency_matrices_2_grams = {}
    frequency_2_grams = {}


    for connection in connections:
        source = object_dict[connection["patchline"]["source"][0]]
        destination = object_dict[connection["patchline"]["destination"][0]]

        nodes = [source, destination]
        nodes.sort()

        if nodes[0] == source:
            node_0 = connection["patchline"]["source"][0]
            node_1 = connection["patchline"]["destination"][0]
        else:
            node_0 = connection["patchline"]["destination"][0]
            node_1 = connection["patchline"]["source"][0]

        # create adjacency matrix for the two sorted nodes
        # vocab_index for connections msg -> tgl and tgl -> msg will be same, for example [0, 1, 0, 1] depending on the length of the vocabulary and the position of the sorted tokens
        # if the connection is msg -> tgl, then the adjacency matrix will be [0, 1, 0, 0]
        # if the connection is tgl -> msg, then the adjacency matrix will be [0, 0, 1, 0]
        
        adjacency_matrix = create_two_node_adjacency_matrix(node_0, node_1, G)
        vocab_index = [0]* (len(unique_nodes) + 1)
        vocab_index[unique_nodes.index(nodes[0])] += 1
        vocab_index[unique_nodes.index(nodes[1])] += 1
        initial_key = str(vocab_index + adjacency_matrix)
        key = calculate_sha256(initial_key)

        if not key in adjacency_matrices_2_grams:                     
            adjacency_matrices_2_grams[key] = adjacency_matrix
            frequency_2_grams[key] = 1
        else:
            frequency_2_grams[key] += 1
    
    
    return adjacency_matrices_2_grams, frequency_2_grams

        
# not needed for the new implementation
def central_node(node_0, node_1, node_2, G):
    '''
    For three nodes, we are checking if there is a connectivity between them.
    We are checking if node_0 is a central node, that means if this connects the other two nodes by any undirected edge.
    If the edge list is like this: [(node_1, node_0), (node_2, node_0)], then node_0 is a central node which connects node_1 and node_2.

    So, what we are trying to answer is : Is node_0 connected to the node_1 and node_2 by edges regardless of direction?
    '''

    is_connected_to_node_1 = G.has_edge(node_0, node_1) or G.has_edge(node_1, node_0)
    is_connected_to_node_2 = G.has_edge(node_0, node_2) or G.has_edge(node_2, node_0)

    return is_connected_to_node_1 and is_connected_to_node_2


def get_3_node_subgraphs(G):
        
    '''
    Get all the 3-node subgraphs from the graph G
    by checking if there is a path between the nodes less than equal to length 2 using networkx all_simple_paths
    '''

    all_paths = []
    
    # for all nodes, find paths less than equal length 3
    for node  in G.nodes:
        # targets are all nodes in the list except this one
        targets = [x for x in G.nodes if x != node]
        paths = nx.all_simple_paths(G, source=node, target=targets, cutoff=2)
        all_paths.extend(paths)

    test_three_node_subgraphs = set()
    
    for path in all_paths:
        path = list(set(path))
        # sort the path
        path.sort()
        if len(path) == 3:
            test_three_node_subgraphs.add(tuple(path))

    return test_three_node_subgraphs



def get_3_node_subgraphs_v2(nodes, G_undirected):
    
    all_three_node_subgraphs = []
    for node in nodes:
        # for each node, restart the algorithm from scratch
        visited = {node: False for node in nodes}
        current_path_for_this_node = []
        two_length_dfs(node, G_undirected, visited, current_path_for_this_node, all_three_node_subgraphs)


    # get the three_node_subgraphs
    three_node_subgraphs = set()
    
    for subgraph in all_three_node_subgraphs:
        path = list(set(subgraph))
        # sort the path
        path.sort()
        if len(path) == 3:
            three_node_subgraphs.add(tuple(path))

    return three_node_subgraphs

def create_three_node_adjacency_matrix(node_0, node_1, node_2, G):

    '''
    Create a 3 by 3 matrix for the three nodes
    '''
    
    # 0 if there is no connection between node_0 and node_1, 1 if there is a connection
    entry00 = 1 if G.has_edge(node_0, node_0) else 0
    entry01 = 1 if G.has_edge(node_0, node_1) else 0
    entry02 = 1 if G.has_edge(node_0, node_2) else 0
    entry10 = 1 if G.has_edge(node_1, node_0) else 0
    entry11 = 1 if G.has_edge(node_1, node_1) else 0
    entry12 = 1 if G.has_edge(node_1, node_2) else 0
    entry20 = 1 if G.has_edge(node_2, node_0) else 0
    entry21 = 1 if G.has_edge(node_2, node_1) else 0
    entry22 = 1 if G.has_edge(node_2, node_2) else 0

    adjacency_matrix = [entry00, entry01, entry02, entry10, entry11, entry12, entry20, entry21, entry22]

    return adjacency_matrix


def create_three_node_adjacency_matrix_v2(node_0, node_1, node_2, G):

    '''
    Create a 3 by 3 matrix for the three nodes
    '''
    
    # 0 if there is no connection between node_0 and node_1, 1 if there is a connection
    entry00 = 1 if node_0 in G[node_0] else 0
    entry01 = 1 if node_1 in G[node_0] else 0
    entry02 = 1 if node_2 in G[node_0] else 0
    entry10 = 1 if node_0 in G[node_1] else 0
    entry11 = 1 if node_1 in G[node_1] else 0
    entry12 = 1 if node_2 in G[node_1] else 0
    entry20 = 1 if node_0 in G[node_2] else 0
    entry21 = 1 if node_1 in G[node_2] else 0
    entry22 = 1 if node_2 in G[node_2] else 0

    adjacency_matrix = [entry00, entry01, entry02, entry10, entry11, entry12, entry20, entry21, entry22]

    return adjacency_matrix


def get_adjacency_matrices_3_grams(three_node_subgraphs, object_dict, unique_nodes, G):
    
    '''
    If we want to build adjacency matrices for 3-grams, there are some scenarios we need to consider:
    1. When we get three node subgraphs, we get it in no specific order, it could be ("PD-ROOT_obj-0", "PD-ROOT_obj-1", "PD-ROOT_obj-2") or ("PD-ROOT_obj-1", "PD-ROOT_obj-0", "PD-ROOT_obj-2")
    2. Now, suppose PD-ROOT_obj-0 is a msg, PD-ROOT_obj-1 is a tgl, and PD-ROOT_obj-2 is a floatatom. Then, to uniquely identify this subgraph, we do two things:
        a. we sort the object types and get a sorted tuple, which could be ("floatatom", "msg", "tgl"). 
        Note that, it's important to sort the object types not id of an object, because id is of no significance to us when finding subgraphs.
        b. for this particular order of nodes, we create adjacency matrix
    4. Finally, we can uniquely identify this subgraph by using the sorted tuple and the adjacency matrix to create a unique key.
    5. For this particular order of nodes, if we get the same adjacency matrix later, then we have found a duplicate subgraph.
    6. If we find the same subgraph later in the graph, we keep the adjacency matrix the same but increment the frequency of the 3-gram.
    7. If for the same serial of nodes, we have a different adjacency matrix (suppose the direction of edges is different), we create a different key for it.
    '''
    
    adjacency_matrices_3_grams = {}
    frequency_3_grams = {}


    for subgraph in three_node_subgraphs:

        # sort the object_type version of these three nodes and get a sorted tuple
        subgraph_items = [object_dict[node] for node in subgraph]
        sorted_tuple = sorted(subgraph_items)
        sorted_indices = sorted(range(len(subgraph_items)), key=lambda x: subgraph_items[x])

        # assign first second and third node according to the sorted indices
        node_0 = subgraph[sorted_indices[0]]
        node_1 = subgraph[sorted_indices[1]]
        node_2 = subgraph[sorted_indices[2]]

        adjacency_matrix = create_three_node_adjacency_matrix(node_0, node_1, node_2, G)
        vocab_index = [0]* (len(unique_nodes) + 1)
        vocab_index[unique_nodes.index(sorted_tuple[0])] += 1
        vocab_index[unique_nodes.index(sorted_tuple[1])] += 1
        vocab_index[unique_nodes.index(sorted_tuple[2])] += 1
        initial_key = str(vocab_index + adjacency_matrix)
        key = calculate_sha256(initial_key)

        
        if not key in adjacency_matrices_3_grams:            
            
            adjacency_matrices_3_grams[key] = adjacency_matrix
            frequency_3_grams[key] = 1
        else:
            frequency_3_grams[key] += 1
    
    return adjacency_matrices_3_grams, frequency_3_grams


def create_a_copy_of_sample_subgraph(sample_subgraph):
    '''
    Create a copy of the sample subgraph
    '''

    return copy.deepcopy(sample_subgraph)


def create_a_test_graph(node_0, node_1, node_2, G):

    '''
    Create a test graph with three nodes and the edges between them.
    '''

    G_test = nx.DiGraph()
    G_test.add_nodes_from([node_0, node_1, node_2])

    edge_01 = []
    edge_12 = []
    edge_02 = []

    # 01
    if G.has_edge(node_0, node_1):
        edge_01.append([node_0, node_1])
    if G.has_edge(node_1, node_0):
        edge_01.append([node_1, node_0])
    # 12 
    if G.has_edge(node_1, node_2):
        edge_12.append([node_1, node_2])
    if G.has_edge(node_2, node_1):
        edge_12.append([node_2, node_1])
    # 02
    if G.has_edge(node_0, node_2):
        edge_02.append([node_0, node_2])
    if G.has_edge(node_2, node_0):
        edge_02.append([node_2, node_0])

    G_test.add_edges_from(edge_01)
    G_test.add_edges_from(edge_12)
    G_test.add_edges_from(edge_02)

    return G_test


def create_a_test_graph_v2(node_0, node_1, node_2, G):
    nodes = [node_0, node_1, node_2]

    G_test = {node:[] for node in nodes}

    # node_0: check if it has a connection with node_1 and node_2
    if node_2 in G[node_0]:
        G_test[node_0].append(node_2)
    if node_1 in G[node_0]:
        G_test[node_0].append(node_1)


    # node_1: check if it has a connection with node_0 and node_2
    if node_0 in G[node_1]:
        G_test[node_1].append(node_0)
    if node_2 in G[node_1]:
        G_test[node_1].append(node_2)

    # node_2: check if it has a connection with node_0 and node_1
    if node_0 in G[node_2]:
        G_test[node_2].append(node_0)
    if node_1 in G[node_2]:
        G_test[node_2].append(node_1)

    return G_test

def two_length_dfs(node, G, visited, path, paths):

    '''
    Ref: code collected from internet
    Get all the paths of length 2 from the node
    '''

    visited[node] = True
    path.append(node)

    if len(path) == 3:
        paths.append(path.copy())
    else:
        for neighbor in G[node]:
            if not visited[neighbor]:
                two_length_dfs(neighbor, G, visited, path, paths)

    path.pop()
    visited[node] = False


def create_undirected_graph(connections, all_objects):
    
    graph = {obj["box"]["id"]: [] for obj in all_objects}
    
    for connection in connections:

        source = connection["patchline"]["source"][0]
        destination = connection["patchline"]["destination"][0]

        # make it bidirectional
        graph[source].append(destination)
        graph[destination].append(source)

    return graph


def create_directed_graph(connections, all_objects):
    
    graph = {obj["box"]["id"]: [] for obj in all_objects}
    
    for connection in connections:

        source = connection["patchline"]["source"][0]
        destination = connection["patchline"]["destination"][0]

        graph[source].append(destination)

    return graph


def get_edges(G_test):

    edges = []
    for node in G_test.keys():
        for edge in G_test[node]:
            edges.append((node, edge))

    return edges
