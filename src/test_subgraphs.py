import networkx as nx
import itertools

from utils import get_3_node_subgraphs



nodes = ['a', 'b', 'c', 'd', 'e', 'f']
edges = [('a', 'b'), ('c', 'b'), ('b', 'd'), ('e', 'd'), ('d', 'f')]

G = nx.DiGraph()
G.add_nodes_from(nodes)
G.add_edges_from(edges)

# get combination of three nodes
three_nodes = list(itertools.combinations(G.nodes, 3))
three_node_subgraphs = get_3_node_subgraphs(G)
print(three_node_subgraphs)

