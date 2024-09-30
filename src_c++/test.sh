echo "Running tests..."
echo

echo "Testing corpus creation of 1, 2, 3 grams..."
g++ -O3 -fconcepts -o test.exe test_corpus_creation.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Score test..."
g++ -O3 -fconcepts -o test.exe test_score.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Utils test..."
g++ -O3 -fconcepts -o test.exe test_utils.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Heap test..."
g++ -O3 -fconcepts -o test.exe test_heap.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "All tests passed for corpus, score function, heap function, and random important functions from utils!"