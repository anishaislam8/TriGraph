import hashlib

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

        



