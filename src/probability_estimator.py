import networkx as nx
import math

from utils import *

def get_score(node_0, node_1, node_2, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test):
    
    '''
    @param node_0: string (object_type), the first node of the subgraph
    @param node_1: string (object_type), the second node of the subgraph
    @param node_2: string (object_type), the third node of the subgraph
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
    2. If the key is not in the frequency_3_grams dictionary, then we need to find the 2-gram keys.
    We are finding the 2-gram keys by taking the first two nodes of the three nodes and calculating the sha256 hash of the adjacency matrix of the two nodes.
    3. If the 2-gram key is in the frequency_2_grams dictionary, then the score is the probability of the 2-gram.
    The probability of the 2-gram is the frequency of the 2-gram divided by the sum of the frequencies of all the 2-grams.
    4. If the 2-gram key is not in the frequency_2_grams dictionary, then the score is the probability of the 1-gram.
    The probability of the 1-gram is the frequency of the 1-gram divided by the sum of the frequencies of all the 1-grams.
    5. Finally, the score is returned.
    '''
    discount_factor = 0.05

    # 3-grams: calculate the sha256 hash of the adjacency matrix of the three nodes

    adjacency_matrix_3_gram = create_three_node_adjaency_matrix(node_0, node_1, node_2, G_test)
    key = calculate_sha256("_".join([node_0, node_1, node_2]) + "_" + str(adjacency_matrix_3_gram))

    
    if key in frequency_3_grams:
        return frequency_3_grams[key] / sum(frequency_3_grams.values())
    else:
        # I have to get the edges of the graph and then calculate the adjacency matrix because the key needs the source and destination of the edges
        edges = list(G_test.edges)
        adjacency_matrix_2_gram = create_two_node_adjaency_matrix()
        score = 1.0
        
        # for each edge, calculate the sha256 hash of the adjacency matrix of the two nodes
        one_gram_keys = set()
        number_of_missed_two_grams = set()
        number_of_missed_one_grams = set()
        
        for edge in edges:
            subgraph_name = "_".join([edge[0], edge[1]])
            key =  calculate_sha256(subgraph_name + "_" + str(adjacency_matrix_2_gram))
            if key in frequency_2_grams:
                score *= (frequency_2_grams[key] / sum(frequency_2_grams.values()))
            else:
                number_of_missed_two_grams.add(key)
                
                if edge[0] in frequency_1_gram:
                    one_gram_keys.add(edge[0])
                else:
                    number_of_missed_one_grams.add(edge[0])
                
                if edge[1] in frequency_1_gram:
                    one_gram_keys.add(edge[1])
                else:
                    number_of_missed_one_grams.add(edge[1])
        
        for key in one_gram_keys:
            score *= (frequency_1_gram[key] / sum(frequency_1_gram.values()))
        
        for key in number_of_missed_one_grams:
            score *= 0.5 * (1 / sum(frequency_1_gram.values())) # assigning a very small value, half of the lowest probability
        
        return score * math.pow(discount_factor, (len(number_of_missed_one_grams) + len(number_of_missed_two_grams) + len(one_gram_keys)))


       
        


def predict_token():
    pass


def count_probability(sample_subgraph, frequency_1_gram, frequency_2_grams, frequency_3_grams):

    '''

    @param sample_subgraph: list of lists, each list contains two strings representing the nodes of the subgraph, each list represents an edge
    @param frequency_1_gram: dictionary, key is the object_type and value is the frequency of the object_type
    @param frequency_2_grams: dictionary, key is the sha256 hash of the adjacency matrix of the two nodes and value is the frequency of the 2-gram
    @param frequency_3_grams: dictionary, key is the sha256 hash of the adjacency matrix of the three nodes and value is the frequency of the 3-gram
    
    @return: float, the probability of the given subgraph

    This method calls all the necessary functions to calculate and return the probability of a given subgraph. 
    At first we are creating a graph from the given subgraph. Then we are finding the three node subgraphs in the graph. 
    We are hard coding this for now as in we are only assuming one subgraph as for three nodes we get one three node subgraph.

    Then we are sorting the nodes by their object_type.
    
    Then we are calculating the score of the subgraph by calling the get_score() function. Finally, we are returning the score.
    
    '''
    
    nodes = set()
    for sample_edge in sample_subgraph:
        nodes.add(sample_edge[0])
        nodes.add(sample_edge[1])

    G_test = nx.DiGraph()
    G_test.add_nodes_from(nodes)
    G_test.add_edges_from([(sample_edge[0], sample_edge[1]) for sample_edge in sample_subgraph])

    test_three_node_subgraphs = get_3_node_subgraphs(G_test)[0] # hard coding this for testing purposes

    # in this case, we are not passing ids, we are directly passing the object types
    sorted_tuple = sorted(test_three_node_subgraphs)
    sorted_indices = sorted(range(len(test_three_node_subgraphs)), key=lambda x: test_three_node_subgraphs[x])

    # the followings are strings representing the object_types (e.g. "msg")
    node_0 = test_three_node_subgraphs[sorted_indices[0]] 
    node_1 = test_three_node_subgraphs[sorted_indices[1]]
    node_2 = test_three_node_subgraphs[sorted_indices[2]]

    score = get_score(node_0, node_1, node_2, frequency_1_gram, frequency_2_grams, frequency_3_grams, G_test)
    return score
        