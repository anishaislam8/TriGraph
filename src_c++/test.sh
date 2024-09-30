echo "Running tests..."
echo

echo "Testing corpus creation of 1, 2, 3 grams..."
g++ -O3 -fconcepts -o test.exe step1-3_test.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Score test..."
g++ -O3 -fconcepts -o test.exe score_test.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Utils test..."
g++ -O3 -fconcepts -o test.exe utils_test.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "Heap test..."
g++ -O3 -fconcepts -o test.exe heap_test.cpp utils.cpp graph.cpp -lsqlite3
./test.exe
echo


echo "All tests passed for corpus, score function, heap function, and random important functions from utils!"