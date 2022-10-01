echo "Compiling..."
make
echo "Running..."
if [ -n "$1" ]; then
    ./main graph.txt query.txt output.txt $1
else
    ./main graph.txt query.txt output_online.txt Online > log_online.txt
    ./main graph.txt query.txt output_baseline.txt Baseline > log_baseline.txt
    ./main graph.txt query.txt output_differential_kruskal.txt DKruskal > log_differential_kruskal.txt
fi
