graph_main_model_1=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_1\\edge_main_values_sorted.txt")
graph_baseline_model_1=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_1\\edge_baseline_values_sorted.txt")
graph_main_model_1$V1 <- as.numeric(as.character(graph_main_model_1$V1))
graph_baseline_model_1$V1 <- as.numeric(as.character(graph_baseline_model_1$V1))
wilcox.test(graph_main_model_1$V1, graph_baseline_model_1$V1)


graph_main_model_2=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_2\\edge_main_values_sorted.txt")
graph_baseline_model_2=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_2\\edge_baseline_values_sorted.txt")
graph_main_model_2$V1 <- as.numeric(as.character(graph_main_model_2$V1))
graph_baseline_model_2$V1 <- as.numeric(as.character(graph_baseline_model_2$V1))
wilcox.test(graph_main_model_2$V1, graph_baseline_model_2$V1)


graph_main_model_3=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_3\\edge_main_values_sorted.txt")
graph_baseline_model_3=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_3\\edge_baseline_values_sorted.txt")
graph_main_model_3$V1 <- as.numeric(as.character(graph_main_model_3$V1))
graph_baseline_model_3$V1 <- as.numeric(as.character(graph_baseline_model_3$V1))
wilcox.test(graph_main_model_3$V1, graph_baseline_model_3$V1)


graph_main_model_4=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_4\\edge_main_values_sorted.txt")
graph_baseline_model_4=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_4\\edge_baseline_values_sorted.txt")
graph_main_model_4$V1 <- as.numeric(as.character(graph_main_model_4$V1))
graph_baseline_model_4$V1 <- as.numeric(as.character(graph_baseline_model_4$V1))
wilcox.test(graph_main_model_4$V1, graph_baseline_model_4$V1)



graph_main_model_5=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_5\\edge_main_values_sorted.txt")
graph_baseline_model_5=read.delim(header = FALSE, file = "D:\\Masters\\Thesis\\models\\my_model\\src_c++\\r_tests\\model_5\\edge_baseline_values_sorted.txt")
graph_main_model_5$V1 <- as.numeric(as.character(graph_main_model_5$V1))
graph_baseline_model_5$V1 <- as.numeric(as.character(graph_baseline_model_5$V1))
wilcox.test(graph_main_model_5$V1, graph_baseline_model_5$V1)

