import hashlib
import networkx as nx
import itertools

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


def get_adjacency_matrices_2_grams(connections, object_dict):
    
    '''
    If we want to build adjacency matrices for 2-grams, there are some scenarios we need to consider:
    1. there could be multiple connections between two nodes. For generating an adjacency matrix, we skipped duplicate connections.
    2. But for counting the frequency of 2-grams, we need to consider all connections. So, for duplicate connections, we increment the frequency of the 2-gram.
    3. If there is a connection from a to b, we create one key for it, and if there is a connection from b to a, we create another key for it. As these are different subgraphs.
    4. If there is a similar subgraph detected later in the graph, we keep the adjacency matrix the same but increment the frequency of the 2-gram.
    '''
    
    adjacency_matrices_2_grams = {}
    frequency_2_grams = {}


    for connection in connections:
        source = object_dict[connection["patchline"]["source"][0]]
        destination = object_dict[connection["patchline"]["destination"][0]]

        key =  + source + "_CONNECTS_" + destination
        
        if not key in adjacency_matrices_2_grams:            
            entry00 = 0
            entry01 = 1
            entry10 = 0
            entry11 = 0
            adjacency_matrices_2_grams[key] = [[entry00, entry01], [entry10, entry11]]
            frequency_2_grams[key] = 1
        else:
            frequency_2_grams[key] += 1
    
    return adjacency_matrices_2_grams, frequency_2_grams

        
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
        by checking if there is a central node that connects the other two nodes in a list of three nodes.
        '''

        # get all combination of 3 nodes
        three_nodes = list(itertools.combinations(G.nodes, 3))

        # not all of them are connected, so we need to only select the ones that have an edge between them
        three_node_subgraphs = []
        
        for three_node in three_nodes:
            is_connected = central_node(three_node[0], three_node[1], three_node[2], G) or central_node(three_node[1], three_node[0], three_node[2], G) or central_node(three_node[2], three_node[0], three_node[1], G)
            if is_connected:
                three_node_subgraphs.append(three_node)

        return three_node_subgraphs
   



