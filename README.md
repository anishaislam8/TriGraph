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

To get the score of a given test subgraph, at first, for the three node subgraphs of the given test subgraph, we calculate the sha256 hash of the adjacency matrix and the concatenation of the names of the three sorted nodes.
This is the key we will use to find the frequency of the 3-gram in the frequency_3_grams dictionary.

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


## Predict functionality
To predict the next token, we follow the same methodology mentioned above. However, we find the next token by iterating through our entire vocabulary and finding the token that maximizes the score of the subgraph.


## Limitation:
1. The current code only works on three-node subgraphs.
2. Our vocabulary is limited to the subgraphs of the parsed source code of one file only.