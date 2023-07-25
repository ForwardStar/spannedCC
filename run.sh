echo "Compiling..."
make
echo "Running..."
if [ -n "$3" ]; then
    ./main graph.txt query.txt output.txt $1 $2 $3
elif [ -n "$2" ]; then
    ./main graph.txt query.txt output.txt $1 $2
elif [ -n "$1" ]; then
    ./main graph.txt query.txt output.txt $1
else
    ./main graph.txt query.txt output_online.txt Online > log_online.txt
    ./main graph.txt query.txt output_baseline.txt Baseline > log_baseline.txt
    ./main graph.txt query.txt output_TSF.txt TSF > log_TSF.txt
fi
