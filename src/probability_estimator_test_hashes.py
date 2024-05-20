import networkx as nx
import random

from utils import *

def get_score(node_0, node_1, node_2, object_dict, unique_tokens_train, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test):
    
    '''
    @param node_0: string (object_type), the first node of the subgraph
    @param node_1: string (object_type), the second node of the subgraph
    @param node_2: string (object_type), the third node of the subgraph
    @param object_dict: dictionary, key is the object_id and value is the object_type
    @param unique_tokens_train: list of strings, the unique tokens in the graph
    @param frequency_1_gram: dictionary, key is the object_type and value is the frequency of the object_type
    @param frequency_2_grams: dictionary, key is the sha256 hash of the adjacency matrix of the two nodes and value is the frequency of the 2-gram
    @param frequency_3_grams: dictionary, key is the sha256 hash of the adjacency matrix of the three nodes and value is the frequency of the 3-gram
    @param G_test: networkx graph, the graph of the subgraph

    @return: float, the score of the given subgraph

    This function calculates the score of a given subgraph considering the frequency of the 1-gram, 2-gram, and 3-gram. At first we create an adjacency matrix for the three nodes.
    After that, we are calculating the sha256 hash of the adjacency matrix and the word vector for these three nodes.
    This is the key that we are going to use to find the frequency of the 3-gram in the frequency_3_grams dictionary.
    
    The algorithm is as follows:
    1. If the key is present in our corpus, the score is assigned as the probability of the 3-node subgraph. This probability is calculated as the frequency of the 3-node
    subgraph divided by the sum of the frequencies of all 3-node subgraphs.
    2. In cases where the key is absent from our corpus, we generate keys for the 2-node subgraphs using the same procedure. For each 2-node subgraph in our input graph, if its key exists in the corpus, we multiply the final
    probability score by the probability of the 2-node subgraph. The probability of a 2-node subgraph is calculated as the frequency of the current 2-node subgraph divided by the sum of the frequencies of all 2-node subgraphs in our corpus.
    3. If the 2-node subgraphs are not present in our corpus, we adjust the score by the probability of the nodes within the subgraph. The probability of a 1-node subgraph is determined by the frequency of that node divided by the
    sum of the frequencies of all unique nodes. Additionally, if the node is not present in our corpus, the score is multiplied by a very small value, such as 0.5 × (1/sum of the frequencies of all unique nodes).
    4. For each missed 3-node and 2-node subgraph, the score is multiplied by a small discount factor to indicate that these subgraphs are unseen in our corpus, thus their probabilities should be lower compared to those observed in our corpus. We have used a discount factor of 0.05.
    5. The final score is returned as the probability of the given subgraph.

    '''
    
    discount_factor = 0.05

    # 3-grams: calculate the sha256 hash of the adjacency matrix of the three nodes

    # ekhane je node_0,1,2 ashbe and G_test ashbe, era real name
    adjacency_matrix_3_gram = create_three_node_adjacency_matrix_v2(node_0, node_1, node_2, G_test)
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
    initial_key = str(vocab_index + adjacency_matrix_3_gram)
    key = calculate_sha256(initial_key)


    
    if key in frequency_3_grams:
        return frequency_3_grams[key] / sum(frequency_3_grams.values())
    else:
        
        edges = get_edges(G_test)
        score = 1.0 * discount_factor # for the missed three grams
        
        
        for edge in edges:
            
            subgraph_items = [object_dict[item] for item in edge]
            sorted_tuple = sorted(subgraph_items)
            sorted_indices = sorted(range(len(subgraph_items)), key=lambda x: subgraph_items[x])

            # assign first second and third node according to the sorted indices
            node_0 = edge[sorted_indices[0]]
            node_1 = edge[sorted_indices[1]]

            adjacency_matrix_2_gram = create_two_node_adjacency_matrix_v2(node_0, node_1, G_test)
            vocab_index_2_grams = [0]* (len(unique_tokens_train) + 1)
            try:
                vocab_index_2_grams[unique_tokens_train.index(object_dict[node_0])] += 1
            except:
                vocab_index_2_grams[-1] += 1
            try:
                vocab_index_2_grams[unique_tokens_train.index(object_dict[node_1])] += 1
            except:
                vocab_index_2_grams[-1] += 1
            initial_key_2_grams = str(vocab_index_2_grams + adjacency_matrix_2_gram)
            key_2_grams = calculate_sha256(initial_key_2_grams)

            if key_2_grams in frequency_2_grams:
                score *= (frequency_2_grams[key_2_grams] / sum(frequency_2_grams.values())) # do not multiply by discount factor
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
    G_test = create_a_test_graph_v2(subgraph[0], subgraph[1], subgraph[2], G)
   
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
        # add the new key in the dictionary
        G_test_new[node_to_add] = []
        
        # what are the edges that are connected to the node that was removed
        edges_to_add = G_test_new[node_to_remove] # if i want to remove c, I have to replicate c's edges
        G_test_new[node_to_add] = edges_to_add
        # remove the node to remove first
        del G_test_new[node_to_remove]

        # remove all references of the node that was removed from the new dictionary
        for key in G_test_new.keys():
            for i in range(len(G_test_new[key])):
                if G_test_new[key][i] == node_to_remove:
                    G_test_new[key][i] = node_to_add
            
        
        
        

        # now G_test_new is my new graph
        G_test_nodes_new = list(G_test_new.keys())
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

    print("True token :", true_token, "Predicted token :", predicted_token, "probability :", max_score)
    if predicted_token == true_token:
        return 1
    else:
        return 0
        

    
        