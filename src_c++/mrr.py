# hashes = []

# with open('/media/baguette/aislam4/paths/train_test_split/test_hashes_filtered.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         hashes.append(line.strip())




# folder = "node_rank"

# # for each hash in hash, read the rank/hash file

# mrr = {}
# for hash in hashes:
#     try:
#         with open(f'{folder}/{hash}.txt', 'r') as f:
#             mrr_of_hash = 0.0
#             lines = f.readlines()
#             if len(lines) == 0: # no connections
#                 continue
#             for line in lines:
#                 line = line.strip()
#                 # PD-ROOT_obj-0 7 select 5
#                 rank = int(line.split(" ")[-1])
#                 if rank == -1:
#                     continue
#                 mrr_of_hash += (1.0 / (rank*1.0))
#             mrr[hash] = mrr_of_hash / len(lines)
#     except:
#         pass

# #write the mrr to a file
# with open(f'node_mrr.txt', 'w') as f:
#     for hash, mrr_value in mrr.items():
#         f.write(f'{hash} {mrr_value}\n')



# print the highest mrr, lowest mrr and average mrr, also, generate a histogram of mrr

'''
# read the edge_mrr_complete.txt file
mrr_values = []
with open('edge_mrr_completed.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        mrr_values.append(float(line.strip().split(" ")[1]))


import matplotlib.pyplot as plt

plt.hist(mrr_values, color="lightgreen", ec = "black", bins=20)
plt.xlabel('MRR')
plt.ylabel('Frequency of Test Graphs')
plt.title('Edge Prediction MRR for Test Graphs - Model 2')
plt.show()


highest_mrr = max(mrr_values)
lowest_mrr = min(mrr_values)
average_mrr = sum(mrr_values) / len(mrr_values)
print("Total graphs: ", len(mrr_values))
# how many has 0 mrr
print("Zero MRR: ", len([mrr_value for mrr_value in mrr_values if mrr_value == 0.0]))

# how many has 1 mrr
print("1 MRR: ", len([mrr_value for mrr_value in mrr_values if mrr_value == 1.0]))

# median mrr
mrr_values.sort()
median_mrr = mrr_values[len(mrr_values)//2]
print("Median MRR: ", median_mrr)


print(f'Highest MRR: {highest_mrr}')
print(f'Lowest MRR: {lowest_mrr}')
print(f'Average MRR: {average_mrr}')



import pandas as pd
df = pd.DataFrame(mrr_values, columns=['MRR'])
print(df.describe())


'''

model_1_mrr = []
with open('edge_mrr_baseline.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_1_mrr.append(float(line.strip().split(" ")[1]))


model_2_mrr = []
with open('edge_mrr_completed.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        model_2_mrr.append(float(line.strip().split(" ")[1]))


# import matplotlib.pyplot as plt

print(len(model_1_mrr))
print(len(model_2_mrr))

# data = [model_1_mrr, model_2_mrr]

# # Creating boxplot
# plt.boxplot(data, labels=['Model 1', 'Model 2'])
# plt.title('Edge Prediction MRR Distribution: Model 1 vs. Model 2')
# plt.xlabel('Model')
# plt.ylabel('MRR')
# plt.show()


import numpy as np
from scipy.stats import wilcoxon

# Scores before and after the new teaching method
before = model_1_mrr
after = model_2_mrr

# Perform the Wilcoxon signed-rank test
res = wilcoxon(before, after, alternative='greater')

print('Statistics=%.10f, p=%.10f' % (res.statistic, res.pvalue))

# Interpret the result
alpha = 0.05
if res.pvalue > alpha:
    print('Same distribution (fail to reject H0)')
else:
    print('Different distribution (reject H0)')