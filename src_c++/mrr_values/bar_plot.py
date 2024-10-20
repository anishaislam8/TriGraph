import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

model_1_mrr = []
with open('model_1/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr.append(float(line.strip().split(" ")[1]))


model_2_mrr = []
with open('model_2/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr.append(float(line.strip().split(" ")[1]))


model_3_mrr = []
with open('model_3/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_3_mrr.append(float(line.strip().split(" ")[1]))


model_4_mrr = []
with open('model_4/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_4_mrr.append(float(line.strip().split(" ")[1]))


model_5_mrr = []
with open('model_5/edge_mrr.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_5_mrr.append(float(line.strip().split(" ")[1]))


mrr_values = []
mrr_values.extend(model_1_mrr)
mrr_values.extend(model_2_mrr)
mrr_values.extend(model_3_mrr)
mrr_values.extend(model_4_mrr)
mrr_values.extend(model_5_mrr)

model_names = ['Model 1']*len(model_1_mrr) + ['Model 2']*len(model_2_mrr) + ['Model 3']*len(model_3_mrr) + ['Model 4']*len(model_4_mrr) + ['Model 5']*len(model_5_mrr)

df = pd.DataFrame({'MRR': mrr_values, 'Model Name': model_names})

# sns.set_theme(style="ticks")

# diamonds = sns.load_dataset("diamonds")

# f, ax = plt.subplots(figsize=(7, 5))

sns.histplot(
    df,
    x="MRR", hue="Model Name",
    multiple="stack",
    palette="Set2",
    edgecolor=".3",
    linewidth=.5,
    bins=20
)
# sns.color_palette("Set2")
plt.ylabel('Number of Test Graphs')
plt.title('Edge Prediction (with 2 and 3 Node Subgraphs) MRR Distribution')
plt.show()