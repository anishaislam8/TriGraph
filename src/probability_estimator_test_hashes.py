import networkx as nx
import random

from utils import *

def get_score(node_0, node_1, node_2, object_dict, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test):
    
    '''
    @param node_0: string (object_type), the first node of the subgraph
    @param node_1: string (object_type), the second node of the subgraph
    @param node_2: string (object_type), the third node of the subgraph
    @param object_dict: dictionary, key is the object_id and value is the object_type
    @param unique_tokens: list of strings, the unique tokens in the graph
    @param frequency_1_gram: dictionary, key is the object_type and value is the frequency of the object_type
    @param frequency_2_grams: dictionary, key is the sha256 hash of the adjacency matrix of the two nodes and value is the frequency of the 2-gram
    @param frequency_3_grams: dictionary, key is the sha256 hash of the adjacency matrix of the three nodes and value is the frequency of the 3-gram
    @param G_test: networkx graph, the graph of the subgraph

    @return: float, the score of the given subgraph

    This function calculates the score of a given subgraph considering the frequency of the 1-gram, 2-gram, and 3-gram. At first we create an adjacency matrix for the three nodes.
    After that, we are calculating the sha256 hash of the adjacency matrix and the concatenation of the names of the three sorted nodes.
    This is the key that we are going to use to find the frequency of the 3-gram in the frequency_3_grams dictionary.
    
    The algorithm is as follows:
    1. If the key is in the frequency_3_grams dictionary, then the score is the probability of the 3-gram.
    The probability of the 3-gram is the frequency of the 3-gram divided by the sum of the frequencies of all the 3-grams.
    2. If the key is not in the frequency_3_grams dictionary, we need to find the 2-gram keys.
    We are finding the 2-gram keys by taking the edges and calculating the sha256 hash of the adjacency matrix of the two nodes.
    3. If the 2-gram key is in the frequency_2_grams dictionary, multiply the score by the probability of the 2-gram.
    The probability of the 2-gram is the frequency of the 2-gram divided by the sum of the frequencies of all the 2-grams.
    4. If the 2-gram key is not in the frequency_2_grams dictionary, multiply the score by the probability of the 1-gram.
    The probability of the 1-gram is the frequency of the 1-gram divided by the sum of the frequencies of all the 1-grams.
    5. Multiply the score with some discount factor raised to the power (number of unique missed two grams + number of missed unique one grams + number of found one grams).
    6. Additionally, for each missed unique one gram, multiply the score with 0.5 * (1 / sum of the frequencies of all the 1-grams).
    7. Return the score.
    '''
    
    discount_factor = 0.05

    # 3-grams: calculate the sha256 hash of the adjacency matrix of the three nodes

    # ekhane je node_0,1,2 ashbe and G_test ashbe, era real name like
    adjacency_matrix_3_gram = create_three_node_adjacency_matrix(node_0, node_1, node_2, G_test)
    vocab_index = [0]* (len(unique_tokens_train) + 1) # the last one is for unknowns
    try:
        vocab_index[unique_tokens_train.index(object_dict[node_0])] += 1
    except:
        vocab_index[-1] += 1
    try:
        vocab_index[unique_tokens_train.index(object_dict[node_1])] += 1
    except:
        vocab_index[-1] += 1
    try:
        vocab_index[unique_tokens_train.index(object_dict[node_2])] += 1
    except:
        vocab_index[-1] += 1
    key = str(vocab_index + adjacency_matrix_3_gram)


    
    if key in frequency_3_grams:
        return frequency_3_grams[key] / sum(frequency_3_grams.values())
    else:
        
        edges = list(G_test.edges)
        score = 1.0 * discount_factor # for the missed three grams
        
        
        for edge in edges:
            
            subgraph_items = [object_dict[item] for item in edge]
            sorted_tuple = sorted(subgraph_items)
            sorted_indices = sorted(range(len(subgraph_items)), key=lambda x: subgraph_items[x])

            # assign first second and third node according to the sorted indices
            node_0 = edge[sorted_indices[0]]
            node_1 = edge[sorted_indices[1]]

            adjacency_matrix_2_gram = create_two_node_adjacency_matrix(node_0, node_1, G_test)
            vocab_index_2_grams = [0]* (len(unique_tokens_train) + 1)
            try:
                vocab_index_2_grams[unique_tokens_train.index(object_dict[node_0])] += 1
            except:
                vocab_index_2_grams[-1] += 1
            try:
                vocab_index_2_grams[unique_tokens_train.index(object_dict[node_1])] += 1
            except:
                vocab_index_2_grams[-1] += 1
            key = str(vocab_index_2_grams + adjacency_matrix_2_gram)

            if key in frequency_2_grams:
                score *= (frequency_2_grams[key] / sum(frequency_2_grams.values())) # do not multiply by discount factor
            else:
                score *= discount_factor # for missed two grams
                
                if object_dict[edge[0]] in frequency_1_gram:
                    score *= (frequency_1_gram[object_dict[edge[0]]] / sum(frequency_1_gram.values()))
                else:
                    score *= 0.5 * (1 / sum(frequency_1_gram.values())) # assigning a very small value, half of the lowest probability
                
                if edge[1] in frequency_1_gram:
                    score *= (frequency_1_gram[object_dict[edge[1]]] / sum(frequency_1_gram.values()))
                else:
                    score *= 0.5 * (1 / sum(frequency_1_gram.values())) # assigning a very small value, half of the lowest probability
                
        
        
        return score


    

def predict(subgraph, object_dict, unique_tokens_train, G, frequency_1_gram, frequency_2_grams, frequency_3_grams):
    
    

    # this is my initial subgraph
    G_test = create_a_test_graph(subgraph[0], subgraph[1], subgraph[2], G)
   
    random_object_to_remove = random.randint(0, 2)
    node_to_remove = None
    true_token = None
    
    if random_object_to_remove == 0:
        node_to_remove = subgraph[0]
        true_token = object_dict[subgraph[0]]
    elif random_object_to_remove == 1:
        node_to_remove = subgraph[1]
        true_token = object_dict[subgraph[1]]
    else:
        node_to_remove = subgraph[2]
        true_token = object_dict[subgraph[2]]

    # add a new node in graph, add edges like the one that was removed, remove the node that was removed and all edges connected to it
    vocabulary = frequency_1_gram.keys()
    max_score = 0.0
    predicted_token = None

    for word in vocabulary:
        node_to_add = word
        
        # need new G_test every time
        G_test_new = copy.deepcopy(G_test)
        G_test_new.add_node(node_to_add)
        
        # what are the edges that are connected to the node that was removed
        edges_to_add = []
        for edge in G_test_new.edges:
            if node_to_remove in edge:
                edges_to_add.append(edge)
        
        for edge in edges_to_add:
            if edge[0] == node_to_remove:
                G_test_new.add_edge(node_to_add, edge[1])
            else:
                G_test_new.add_edge(edge[0], node_to_add)

        G_test_new.remove_node(node_to_remove)
        # remove the edges that are connected to the node that was removed
        G_test_new.remove_edges_from(edges_to_add)

        # now G_test_new is my new graph
        G_test_nodes_new = list(G_test_new.nodes)
        subgraph_items = [object_dict[node] for node in G_test_nodes_new]
        sorted_tuple = sorted(subgraph_items)
        sorted_indices = sorted(range(len(subgraph_items)), key=lambda x: subgraph_items[x])

        # assign first second and third node according to the sorted indices
        node_0 = G_test_nodes_new[sorted_indices[0]]
        node_1 = G_test_nodes_new[sorted_indices[1]]
        node_2 = G_test_nodes_new[sorted_indices[2]]
        
        try:
            score = get_score(node_0, node_1, node_2, object_dict, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test_new)
        except Exception as e:
            score =  0.0

        if score > max_score:
            max_score = score
            predicted_token = word

    
    if predicted_token == true_token:
        return 1
    else:
        return 0
        

    
        