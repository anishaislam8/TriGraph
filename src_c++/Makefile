.PHONY: dummy corpus_exe exe

exe: corpus_exe node.exe

corpus_exe: 1_gram.exe 2_grams.exe 3_grams.exe

1_gram.exe: 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp 
	g++ -O3 -fconcepts -o 1_gram.exe 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp -lsqlite3

2_grams.exe: 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp 
	g++ -O3 -fconcepts -o 2_grams.exe 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp -lsqlite3

3_grams.exe:
	g++ -O3 -fconcepts -o 3_grams.exe 3_extract_3_gram_frequencies_db.cpp utils.cpp graph.cpp -lsqlite3

node.exe: 4_node_predictor_local.cpp utils.cpp graph.cpp
	g++ -O3 -fconcepts -o node.exe 4_node_predictor_local.cpp utils.cpp graph.cpp -lsqlite3


bulk_node.exe: 4_node_predictor_db.cpp utils.cpp graph.cpp
	g++ -O3 -fconcepts -o bulk_node.exe 4_node_predictor_db.cpp utils.cpp graph.cpp -lsqlite3

node_example: node.exe
	./node.exe ../sample_jsons/example_basic.json

bulk_node_example: bulk_node.exe
	./bulk_node.exe ./train_test_hashes/model_1/test_hashes_filtered.txt database.db

edge.exe: 4_edge_predictor_local.cpp utils.cpp graph.cpp
	g++ -O3 -fconcepts -o edge.exe 4_edge_predictor_local.cpp utils.cpp graph.cpp -lsqlite3

bulk_edge.exe: 4_edge_predictor_db.cpp utils.cpp graph.cpp 
	g++ -O3 -fconcepts -o bulk_edge.exe 4_edge_predictor_db.cpp utils.cpp graph.cpp -lsqlite3

edge_example: edge.exe
	./edge.exe ../sample_jsons/example_basic.json

bulk_edge_example: bulk_edge.exe
	./bulk_edge.exe ./train_test_hashes/model_1/test_hashes_filtered.txt database.db

UT=vocabulary_frequencies/unique_tokens_train.txt

vocabulary_frequencies/frequency_1_gram_train.txt: 1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db
	./1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db

# This part is annoying since 1_gram.exe makes 2 outputs

$(UT): dummy

# we're saying that dummy needs this file so it'll build it implicitly
dummy: vocabulary_frequencies/frequency_1_gram_train.txt

vocabulary_frequencies/frequency_2_grams_train.txt: 2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db $(UT)
	./2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
vocabulary_frequencies/frequency_3_grams_train.txt: 3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db $(UT)
	./3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db

trains: vocabulary_frequencies/frequency_1_gram_train.txt vocabulary_frequencies/frequency_2_grams_train.txt vocabulary_frequencies/frequency_3_grams_train.txt
