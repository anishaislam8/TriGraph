import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# model_1_mrr = []
# with open('model_1/node_mrr_v2.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_1_mrr.append(float(line.strip().split(" ")[1]))


# model_2_mrr = []
# with open('model_2/node_mrr.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_2_mrr.append(float(line.strip().split(" ")[1]))


# model_3_mrr = []
# with open('model_3/node_mrr.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_3_mrr.append(float(line.strip().split(" ")[1]))


# model_4_mrr = []
# with open('model_4/node_mrr.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_4_mrr.append(float(line.strip().split(" ")[1]))


# model_5_mrr = []
# with open('model_5/node_mrr.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         model_5_mrr.append(float(line.strip().split(" ")[1]))



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


mrr_values = []
mrr_values.extend(model_1_mrr)
mrr_values.extend(model_2_mrr)
mrr_values.extend(model_3_mrr)
mrr_values.extend(model_4_mrr)
mrr_values.extend(model_5_mrr)

model_names = ['Model 1']*len(model_1_mrr) + ['Model 2']*len(model_2_mrr) + ['Model 3']*len(model_3_mrr) + ['Model 4']*len(model_4_mrr) + ['Model 5']*len(model_5_mrr)
#model_names = ['Model 1']*len(model_1_mrr) + ['Model 3']*len(model_3_mrr) + ['Model 4']*len(model_4_mrr) + ['Model 5']*len(model_5_mrr)

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
plt.title('Node Prediction (KenLM) MRR Distribution')
plt.show()