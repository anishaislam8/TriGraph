# Probability Estimator for Visual Code
We implement a probability estimator for Pure Data(PD) subgraphs in this project. Our goal is to output a probability value of a subgraph given a corpus of subgraphs extracted from the initial input parsed PD program. 

## Methodology
The methodology is as follows:
1. We build a graph from the list of connections in a parsed Pure Data file.
2. After that, we extract the unique nodes (object_types such as "msg", "tgl" etc), count the frequency of each token, and save it in a dictionary called `frequency_1_gram`.
3. For each unique connection, we create adjacency matrices, calculate the frequencies of the unique connections, and save it in a dictionary called `frequency_2_grams`.
4. Then, we find the three-node subgraphs from the input graph, create adjacency matrices, calculate the frequencies of the 3-node subgraphs, and store them in a dictionary called `frequency_3_grams`.
5. Finally, we answer the following questions.
    - Given a bag of subgraphs and their probabilities, how probable is a sequence?
    - Given a subgraph with a blank token, what is the most likely node that should be in the BLANK position?


## Score functionality

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


## Predict functionality
To predict the next token, we follow the same methodology mentioned above. However, we find the next token by iterating through our entire vocabulary and finding the token that maximizes the score of the subgraph.


## Limitations:
1. The current code only works on three-node subgraphs.
