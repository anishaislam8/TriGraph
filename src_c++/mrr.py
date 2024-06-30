# hashes = []

# with open('/media/baguette/aislam4/paths/train_test_split/test_hashes_filtered.txt', 'r') as f:
#     lines = f.readlines()
#     for line in lines:
#         hashes.append(line.strip())




# folder = "edge_rank"

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

# write the mrr to a file
# with open(f'edge_mrr.txt', 'w') as f:
#     for hash, mrr_value in mrr.items():
#         f.write(f'{hash} {mrr_value}\n')



# print the highest mrr, lowest mrr and average mrr, also, generate a histogram of mrr

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
plt.title('Histogram of Edge Prediction MRR for Test Graphs')
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




