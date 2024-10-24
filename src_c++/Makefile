.PHONY: dummy corpus_exe exe clean trains

CC=g++
CFLAGS=-O3 -fconcepts
COMPILE=$(CC) $(CFLAGS)
LDFLAGS=-lsqlite3 
EXES=1_gram.exe   3_grams.exe    bulk_node.exe  node.exe         test_heap.exe   test_utils.exe 2_grams.exe  bulk_edge.exe	bulk_edge_baseline.exe	edge.exe       test_corpus.exe  test_score.exe
exe: $(EXES)

clean:
	rm -f $(EXES)

corpus_exe: 1_gram.exe 2_grams.exe 3_grams.exe

1_gram.exe: 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp 
	$(COMPILE) -o 1_gram.exe 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp $(LDFLAGS)

2_grams.exe: 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp 
	$(COMPILE) -o 2_grams.exe 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp $(LDFLAGS)

3_grams.exe:
	$(COMPILE) -o 3_grams.exe 3_extract_3_gram_frequencies_db.cpp utils.cpp graph.cpp $(LDFLAGS)

node.exe: 4_node_predictor_local.cpp utils.cpp graph.cpp
	$(COMPILE) -o node.exe 4_node_predictor_local.cpp utils.cpp graph.cpp $(LDFLAGS)

bulk_node.exe: 4_node_predictor_db.cpp utils.cpp graph.cpp
	$(COMPILE) -o bulk_node.exe 4_node_predictor_db.cpp utils.cpp graph.cpp $(LDFLAGS)

node_example: node.exe
	./node.exe ../sample_jsons/example_basic.json

bulk_node_example: bulk_node.exe
	mkdir node_rank
	./bulk_node.exe ./train_test_hashes/model_1/test_hashes_filtered.txt database.db

edge.exe: 4_edge_predictor_local.cpp utils.cpp graph.cpp
	$(COMPILE) -o edge.exe 4_edge_predictor_local.cpp utils.cpp graph.cpp $(LDFLAGS)

bulk_edge.exe: 4_edge_predictor_db.cpp utils.cpp graph.cpp 
	$(COMPILE) -o bulk_edge.exe 4_edge_predictor_db.cpp utils.cpp graph.cpp $(LDFLAGS)

bulk_edge_baseline.exe: 4_edge_predictor_baseline_db.cpp utils.cpp graph.cpp 
	$(COMPILE) -o bulk_edge_baseline.exe 4_edge_predictor_baseline_db.cpp utils.cpp graph.cpp $(LDFLAGS)

edge_example: edge.exe
	./edge.exe ../sample_jsons/example_basic.json

bulk_edge_example: bulk_edge.exe
	mkdir edge_rank
	./bulk_edge.exe ./train_test_hashes/model_1/test_hashes_filtered.txt database.db

bulk_edge_baseline_example: bulk_edge_baseline.exe
	mkdir edge_rank_baseline
	./bulk_edge_baseline.exe ./train_test_hashes/model_1/test_hashes_filtered.txt database.db

UT=vocabulary_frequencies/unique_tokens_train.txt

vocabulary_frequencies/frequency_1_gram_train.txt: 1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db
	./1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db

# This part is annoying since 1_gram.exe makes 2 outputs

$(UT): dummy

# we're saying that dummy needs this file so it'll build it implicitly
dummy: vocabulary_frequencies/frequency_1_gram_train.txt

vocabulary_frequencies/frequency_2_grams_train.txt: 2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db $(UT)
	./2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
vocabulary_frequencies/frequency_3_grams_train_all.txt: 3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db $(UT)
	./3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
vocabulary_frequencies/frequency_3_grams_train.txt: merge.py vocabulary_frequencies/frequency_3_grams_train_all.txt
	python3 merge.py
observed_3_grams: vocabulary_frequencies/frequency_3_grams_train.txt
	cat vocabulary_frequencies/frequency_3_grams_train.txt | cut -d "," -f 1-3 | sort -u > vocabulary_frequencies/observed_3_grams_train.txt

test_corpus.exe: test_corpus_creation.cpp utils.cpp graph.cpp 
	$(COMPILE) -o test_corpus.exe test_corpus_creation.cpp utils.cpp graph.cpp $(LDFLAGS)
test_score.exe: test_score.cpp utils.cpp graph.cpp 
	$(COMPILE) -o test_score.exe test_score.cpp utils.cpp graph.cpp $(LDFLAGS)
test_utils.exe: test_utils.cpp utils.cpp graph.cpp 
	$(COMPILE) -o test_utils.exe test_utils.cpp utils.cpp graph.cpp $(LDFLAGS)
test_heap.exe: test_heap.cpp utils.cpp graph.cpp 
	$(COMPILE) -o test_heap.exe test_heap.cpp utils.cpp graph.cpp $(LDFLAGS)

.PHONY: run_tests
run_tests: test_corpus.exe test_score.exe test_utils.exe test_heap.exe
	./test_corpus.exe
	./test_score.exe
	./test_utils.exe
	./test_heap.exe

trains: vocabulary_frequencies/frequency_1_gram_train.txt vocabulary_frequencies/frequency_2_grams_train.txt vocabulary_frequencies/frequency_3_grams_train_all.txt vocabulary_frequencies/frequency_3_grams_train.txt observed_3_grams
predicts: node_example edge_example
bulk_predicts: bulk_node_example bulk_edge_example bulk_edge_baseline_example
tests: run_tests

