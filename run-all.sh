if [ -d "results" ]; then
    rm -rf results
fi
mkdir "results"

sh graph-gen.sh 1
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/1"
mv log* "results/1"

sh graph-gen.sh 2
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/2"
mv log* "results/2"

sh graph-gen.sh 3
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/3"
mv log* "results/3"

sh graph-gen.sh 4
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/4"
mv log* "results/4"

sh graph-gen.sh 5
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/5"
mv log* "results/5"

sh graph-gen.sh 6
echo "1000\n0.8" > "query-setting.txt"
sh query-gen.sh < query-setting.txt
sh run.sh
mkdir "results/6"
mv log* "results/6"

rm query-setting.txt