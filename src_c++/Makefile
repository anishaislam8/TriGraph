corpus_exe: 1_gram.exe 2_grams.exe 3_grams.exe

1_gram.exe: 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp 
	g++ -O3 -fconcepts -o 1_gram.exe 1_extract_unique_tokens_db.cpp utils.cpp graph.cpp -lsqlite3

2_grams.exe: 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp 
	g++ -O3 -fconcepts -o 2_grams.exe 2_extract_2_gram_frequencies_db.cpp utils.cpp graph.cpp -lsqlite3

3_grams.exe:
	g++ -O3 -fconcepts -o 3_grams.exe 3_extract_3_gram_frequencies_db.cpp utils.cpp graph.cpp -lsqlite3

vocabulary_frequencies/frequency_1_gram_train.txt: 1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db
	./1_gram.exe ./train_test_hashes/model_1/train_hashes.txt database.db
vocabulary_frequencies/frequency_2_grams_train.txt: 2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
	./2_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
vocabulary_frequencies/frequency_3_grams_train.txt: 3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db
	./3_grams.exe ./train_test_hashes/model_1/train_hashes.txt database.db

trains: vocabulary_frequencies/frequency_1_gram_train.txt vocabulary_frequencies/frequency_2_grams_train.txt vocabulary_frequencies/frequency_3_grams_train.txt
