import matplotlib.pyplot as plt

graph_runtime = []
kenlm_runtime = []

with open('graph_elapsed_time_linux.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip().split(" ")
        real = line[0]
        # user = line[1]
        # sys = line[2]

        # user_minutes_seconds = user.split("m")
        # user_minutes = int(user_minutes_seconds[0])
        # user_seconds = float(user_minutes_seconds[1].replace("s", ""))

        # total_user_seconds = user_minutes * 60 + user_seconds

        # sys_minutes_seconds = sys.split("m")
        # sys_minutes = int(sys_minutes_seconds[0])
        # sys_seconds = float(sys_minutes_seconds[1].replace("s", ""))
        
        # total_sys_seconds = sys_minutes * 60 + sys_seconds

        # total_runtime = total_user_seconds + total_sys_seconds
        real_minutes_seconds = real.split("m")
        real_minutes = int(real_minutes_seconds[0])
        real_seconds = float(real_minutes_seconds[1].replace("s", ""))
        total_runtime = real_minutes * 60 + real_seconds
        graph_runtime.append(total_runtime)


# write graph_runtime to a file
with open('graph_runtime_linux_values.txt', 'w') as f:
    for runtime in graph_runtime:
        f.write(str(runtime) + "\n")



with open('kenlm_elapsed_time_linux.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip().split(" ")
        real = line[0]
        # user = line[1]
        # sys = line[2]

        # user_minutes_seconds = user.split("m")
        # user_minutes = int(user_minutes_seconds[0])
        # user_seconds = float(user_minutes_seconds[1].replace("s", ""))

        # total_user_seconds = user_minutes * 60 + user_seconds

        # sys_minutes_seconds = sys.split("m")
        # sys_minutes = int(sys_minutes_seconds[0])
        # sys_seconds = float(sys_minutes_seconds[1].replace("s", ""))
        # total_sys_seconds = sys_minutes * 60 + sys_seconds

        # total_runtime = total_user_seconds + total_sys_seconds

        real_minutes_seconds = real.split("m")
        real_minutes = int(real_minutes_seconds[0])
        real_seconds = float(real_minutes_seconds[1].replace("s", ""))
        total_runtime = real_minutes * 60 + real_seconds
        kenlm_runtime.append(total_runtime)

        
with open('kenlm_runtime_linux_values.txt', 'w') as f:
    for runtime in kenlm_runtime:
        f.write(str(runtime) + "\n")

data = [graph_runtime, kenlm_runtime]

# Creating boxplot
plt.boxplot(data, labels=['Graph Model', 'KenLM'])
plt.title('Runtime Comparison for Node Prediction: Graph model vs. KenLM')
plt.xlabel('Model')
plt.ylabel('Runtime in seconds')
plt.show()

# R code for wilcoxon test

# graph_runtime=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\runtime\\graph_runtime_linux_values.txt");
# kenlm_runtime=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\runtime\\kenlm_runtime_linux_values.txt");
# graph_runtime$V1 <- as.numeric(as.character(graph_runtime$V1))
# kenlm_runtime$V1 <- as.numeric(as.character(kenlm_runtime$V1))
# wilcox.test(graph_runtime$V1, kenlm_runtime$V1)