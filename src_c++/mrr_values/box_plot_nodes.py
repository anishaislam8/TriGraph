import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

model_1_mrr = []
with open('model_1/kenlm_mrr_model1.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr.append(float(line.strip().split(" ")[1]))


model_2_mrr = []
with open('model_2/kenlm_mrr_model2.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr.append(float(line.strip().split(" ")[1]))


model_3_mrr = []
with open('model_3/kenlm_mrr_model3.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_3_mrr.append(float(line.strip().split(" ")[1]))


model_4_mrr = []
with open('model_4/kenlm_mrr_model4.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_4_mrr.append(float(line.strip().split(" ")[1]))


model_5_mrr = []
with open('model_5/kenlm_mrr_model5.txt', 'r') as f:
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
with open('model_1/node_mrr_v2.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr_main.append(float(line.strip().split(" ")[1]))

model_2_mrr_main = []
with open('model_2/node_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr_main.append(float(line.strip().split(" ")[1]))


model_3_mrr_main = []
with open('model_3/node_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_3_mrr_main.append(float(line.strip().split(" ")[1]))


model_4_mrr_main = []
with open('model_4/node_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_4_mrr_main.append(float(line.strip().split(" ")[1]))


model_5_mrr_main = []
with open('model_5/node_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_5_mrr_main.append(float(line.strip().split(" ")[1]))


# print("Model 1:")
# print("Kenlm MRR: ", len(model_1_mrr))
# print("Graph MRR: ", len(model_1_mrr_main))

# print("Model 2:")
# print("Kenlm MRR: ", len(model_2_mrr))
# print("Graph MRR: ", len(model_2_mrr_main))


# print("Model 3:")
# print("Kenlm MRR: ", len(model_3_mrr))
# print("Graph MRR: ", len(model_3_mrr_main))


# print("Model 4:")
# print("Kenlm MRR: ", len(model_4_mrr))
# print("Graph MRR: ", len(model_4_mrr_main))


# print("Model 5:")
# print("Kenlm MRR: ", len(model_5_mrr))
# print("Graph MRR: ", len(model_5_mrr_main))


mrr_values = []
mrr_values.extend(model_1_mrr)
mrr_values.extend(model_1_mrr_main)
mrr_values.extend(model_2_mrr)
mrr_values.extend(model_2_mrr_main)
mrr_values.extend(model_3_mrr)
mrr_values.extend(model_3_mrr_main)
mrr_values.extend(model_4_mrr)
mrr_values.extend(model_4_mrr_main)
mrr_values.extend(model_5_mrr)
mrr_values.extend(model_5_mrr_main)

model_names = []
model_names.extend(['Model 1']*len(model_1_mrr))
model_names.extend(['Model 1']*len(model_1_mrr_main))
model_names.extend(['Model 2']*len(model_2_mrr))
model_names.extend(['Model 2']*len(model_2_mrr_main))
model_names.extend(['Model 3']*len(model_3_mrr))
model_names.extend(['Model 3']*len(model_3_mrr_main))
model_names.extend(['Model 4']*len(model_4_mrr))
model_names.extend(['Model 4']*len(model_4_mrr_main))
model_names.extend(['Model 5']*len(model_5_mrr))
model_names.extend(['Model 5']*len(model_5_mrr_main))


kenlm_vs_graph = []
kenlm_vs_graph.extend(["KenLM"]*len(model_1_mrr))
kenlm_vs_graph.extend(["Graph Model"]*len(model_1_mrr_main))
kenlm_vs_graph.extend(["KenLM"]*len(model_2_mrr))
kenlm_vs_graph.extend(["Graph Model"]*len(model_2_mrr_main))
kenlm_vs_graph.extend(["KenLM"]*len(model_3_mrr))
kenlm_vs_graph.extend(["Graph Model"]*len(model_3_mrr_main))
kenlm_vs_graph.extend(["KenLM"]*len(model_4_mrr))
kenlm_vs_graph.extend(["Graph Model"]*len(model_4_mrr_main))
kenlm_vs_graph.extend(["KenLM"]*len(model_5_mrr))
kenlm_vs_graph.extend(["Graph Model"]*len(model_5_mrr_main))




# Example data
data_dict = {
    "Model Name": model_names,
    "Node Prediction MRR": mrr_values,
    "Language Model": kenlm_vs_graph
}

# Create DataFrame
data = pd.DataFrame(data_dict)

# print(data.head(5))


# create grouped boxplot  
sns.boxplot(x = data['Model Name'], 
            y = data['Node Prediction MRR'], 
            hue = data['Language Model'],
            palette="Set2") 


# Move the legend to the upper right corner, outside the plot
plt.legend( bbox_to_anchor=(0.88, 1), title='Language Model')

plt.title('Node Prediction MRR for Models 1, 2, 3, 4, and 5')
plt.show()


