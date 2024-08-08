import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

model_1_mrr = []
with open('model_1/edge_mrr_baseline.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr.append(float(line.strip().split(" ")[1]))


model_2_mrr = []
with open('model_2/edge_mrr_baseline.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr.append(float(line.strip().split(" ")[1]))


# model_3_mrr = []
# with open('model_3/edge_mrr_baseline.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_3_mrr.append(float(line.strip().split(" ")[1]))


model_4_mrr = []
with open('model_4/edge_mrr_baseline.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_4_mrr.append(float(line.strip().split(" ")[1]))


model_5_mrr = []
with open('model_5/edge_mrr_baseline.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_5_mrr.append(float(line.strip().split(" ")[1]))


# print(len(model_1_mrr))
# print(len(model_2_mrr))
# print(len(model_4_mrr))
# print(len(model_5_mrr))

# data = [model_1_mrr, model_2_mrr, model_4_mrr, model_5_mrr]


# # Creating boxplot
# plt.boxplot(data, labels=['Model 1', 'Model 2', 'Model 4', 'Model 5'])
# plt.title('Edge Prediction (with 3 Node Subgraphs) MRR Distribution')
# plt.xlabel('Models')
# plt.ylabel('MRR')
# plt.show()


model_1_mrr_main = []
with open('model_1/edge_mrr_completed.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr_main.append(float(line.strip().split(" ")[1]))

model_2_mrr_main = []
with open('model_2/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr_main.append(float(line.strip().split(" ")[1]))


# model_3_mrr_main = []
# with open('model_3/edge_mrr.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_3_mrr_main.append(float(line.strip().split(" ")[1]))


model_4_mrr_main = []
with open('model_4/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_4_mrr_main.append(float(line.strip().split(" ")[1]))


model_5_mrr_main = []
with open('model_5/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_5_mrr_main.append(float(line.strip().split(" ")[1]))



mrr_values = []
mrr_values.extend(model_1_mrr)
mrr_values.extend(model_1_mrr_main)
mrr_values.extend(model_2_mrr)
mrr_values.extend(model_2_mrr_main)
mrr_values.extend(model_4_mrr)
mrr_values.extend(model_4_mrr_main)
mrr_values.extend(model_5_mrr)
mrr_values.extend(model_5_mrr_main)

model_names = []
model_names.extend(['Model 1']*len(model_1_mrr))
model_names.extend(['Model 1']*len(model_1_mrr_main))
model_names.extend(['Model 2']*len(model_2_mrr))
model_names.extend(['Model 2']*len(model_2_mrr_main))
model_names.extend(['Model 4']*len(model_4_mrr))
model_names.extend(['Model 4']*len(model_4_mrr_main))
model_names.extend(['Model 5']*len(model_5_mrr))
model_names.extend(['Model 5']*len(model_5_mrr_main))


is_2_3_node_subgraphs = []
is_2_3_node_subgraphs.extend(["3 Node Subgraphs"]*len(model_1_mrr))
is_2_3_node_subgraphs.extend(["2 and 3 Node Subgraphs"]*len(model_1_mrr_main))
is_2_3_node_subgraphs.extend(["3 Node Subgraphs"]*len(model_2_mrr))
is_2_3_node_subgraphs.extend(["2 and 3 Node Subgraphs"]*len(model_2_mrr_main))
is_2_3_node_subgraphs.extend(["3 Node Subgraphs"]*len(model_4_mrr))
is_2_3_node_subgraphs.extend(["2 and 3 Node Subgraphs"]*len(model_4_mrr_main))
is_2_3_node_subgraphs.extend(["3 Node Subgraphs"]*len(model_5_mrr))
is_2_3_node_subgraphs.extend(["2 and 3 Node Subgraphs"]*len(model_5_mrr_main))




# Example data
data_dict = {
    "Model Name": model_names,
    "Edge Prediction MRR": mrr_values,
    "Subgraphs Considered": is_2_3_node_subgraphs
}

# Create DataFrame
data = pd.DataFrame(data_dict)

# print(data.head(5))


# create grouped boxplot  
sns.boxplot(x = data['Model Name'], 
            y = data['Edge Prediction MRR'], 
            hue = data['Subgraphs Considered'])


# Move the legend to the upper right corner, outside the plot
plt.legend( bbox_to_anchor=(0.81, 1), title='Subgraphs Considered')

plt.title('Edge Prediction MRR for Models 1, 2, 4, and 5')
plt.show()