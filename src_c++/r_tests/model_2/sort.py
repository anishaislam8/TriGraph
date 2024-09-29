edge_main = []
edge_baseline = []

with open('edge_main_values.txt', 'r') as f:
    data = f.readlines()
    for line in data:
        line = line.strip()
        edge_main.append(float(line))


with open('edge_baseline_values.txt', 'r') as f:
    data = f.readlines()
    for line in data:
        line = line.strip()
        edge_baseline.append(float(line))


edge_main.sort()
edge_baseline.sort()

with open('edge_main_values_sorted.txt', 'w') as f:
    for value in edge_main:
        f.write(str(value) + '\n')


with open('edge_baseline_values_sorted.txt', 'w') as f:
    for value in edge_baseline:
        f.write(str(value) + '\n')


       