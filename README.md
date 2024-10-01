# Probability Estimator for Visual Code
We implement a code prediction model for Pure Data(PD) subgraphs in this project. Our goal is to predict the nodes and edges of a PD graph by utilizing the graph nature of the PD programs.

## Methodology
The methodology is as follows:
1. We build a graph from the list of connections in a parsed Pure Data file.
2. After that, we extract the unique nodes (object_types such as "msg", "tgl" etc), count the frequency of each token, and save it in a dictionary called `frequency_1_gram`.
3. For each unique connection, we create adjacency matrices, calculate the frequencies of the unique connections, and save it in a dictionary called `frequency_2_grams`.
4. Then, we find the three-node subgraphs from the input graph, create adjacency matrices, calculate the frequencies of the 3-node subgraphs, and store them in a dictionary called `frequency_3_grams`.
5. Finally, we answer the following questions.
    -  In the scenario of a PD graph featuring an empty node, how effectively can our model predict which node will fill the empty position?
    - Given three nodes in a PD graph that could potentially be interconnected, how effectively can our model identify the most probable edges connecting these 3-node combinations?


## How to run our code for predicting the nodes and edges of a PD graph

### Step 1: Parsing the PD file contents (Skip this step if you have the parsed content of a PD file or if you are using the PD database of parsed source files)

1. To parse a PD file into an Abstract Syntax Tree, at first move to the directory *parsing_the_PD_file_contents*

    - `cd parsing_the_PD_file_contents`

2. Then, provide the location of your PD file as an argument of the *parse.py* file and run it using Python

    - `python parse.py <location_of_PD_file>`

3. The parsed output will be saved in the *example_PD_file_and_parsed_output* directory as a file named *example.json*


### Step 2: Predict the nodes and edges of a PD graph

1. To predict the nodes and edges of the PD graph, at first move to the *src_c++* directory.

    - `cd src_c++`

2. At first parse the file using the procedure mentioned in Step 1 (or skip that step if you already have a parsed file). 

3. To predict the nodes:
    - Update line 41 of the *node_predictor_local.cpp* file to provide the parsed file path.  

    - Then run the file using the following commands:

        - `g++ -O3 -fconcepts -o node.exe node_predictor_local.cpp utils.cpp graph.cpp -lsqlite3`
        - `./node.exe`

    - Our node predictor model will replace the nodes in your PD file with a *BLANK*. For each node, based on its context, the model will predict the most likely node to fill that position. The output will include the rank of the actual node within the prediction heap and the contents of the heap after the prediction phase. Note that the heap contains the negative log probability scores of the tokens, meaning the highest real probability corresponds to the lowest negative log probability. As a result, the heap is sorted in ascending order, with the topmost entry being the most likely node to fill the *BLANK* position. A sample format of output is shown below:

    ![Sample Output](./src_c++/figures/sample_output.JPG)

4. To predict the edges:
    - Update line 39 of the *edge_predictor_local.cpp* file to provide the parsed file path.  

    - Then run the file using the following commands:

        - `g++ -O3 -fconcepts -o edge.exe edge_predictor_local.cpp utils.cpp graph.cpp -lsqlite3`
        - `./edge.exe`

    - Our edge predictor model will take every three node subgraph of your PD graph, and predict the most probable edges between these three nodes. Note that the heap contains the negative log probability scores of the tokens, meaning the highest real probability corresponds to the lowest negative log probability. As a result, the heap is sorted in ascending order, with the topmost entry being the most likely adjacency matrix representing the connections between the three nodes where the order of the nodes is shown in the output. A sample format of output is shown below:
    
    ![Sample Output](./src_c++/figures/sample_output_edges.JPG)




