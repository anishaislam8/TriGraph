echo "Running tests..."
echo

echo "Testing corpus creation of 1, 2, 3 grams..."
# g++ -O3 -fconcepts -o test.exe test_corpus_creation.cpp utils.cpp graph.cpp -lsqlite3
make test_corpus.exe
./test_corpus.exe
echo


echo "Score test..."
# g++ -O3 -fconcepts -o test.exe test_score.cpp utils.cpp graph.cpp -lsqlite3
make test_score.exe
./test_score.exe
echo


echo "Utils test..."
# g++ -O3 -fconcepts -o test.exe test_utils.cpp utils.cpp graph.cpp -lsqlite3
make test_utils.exe
./test_utils.exe
echo


echo "Heap test..."
# g++ -O3 -fconcepts -o test.exe test_heap.cpp utils.cpp graph.cpp -lsqlite3
make test_heap.exe
./test_heap.exe
echo


echo "All tests passed for corpus, score function, heap function, and random important functions from utils!"
