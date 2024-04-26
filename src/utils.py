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