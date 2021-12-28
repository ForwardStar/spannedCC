echo "Compiling..."
g++ -O3 source/*.cpp -o main -std=c++11
echo "Running..."
./main graph.txt query.txt output.txt $1