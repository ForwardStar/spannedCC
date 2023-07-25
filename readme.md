## Window-CC solutions

This is one of the code repositories of the paper [``On Querying Connected Components in Large Temporal Graphs``](https://dl.acm.org/doi/10.1145/3589315) (SIGMOD 2023).

The repository includes three solutions to solve the window-CC queries: ``U-online``, ``U-baseline`` and ``TSF-index``.

How to use it (you need to run the scripts on a Linux platform):

- Run ``graph-gen.sh`` to generate graph data automatically, which would download datasets from [SNAP](https://snap.stanford.edu/data/index.html) and [KONECT](http://konect.cc/) and process the data into ``graph.txt``:

```sh
sh graph-gen.sh
```

- Run ``query-gen.sh`` and input the size of query time windows (in proportion to ``tmax``) to generate query data automatically, which would write queries into ``query.txt``:

```sh
sh query-gen.sh
```

- Run the following command to run the solutions:

```sh
sh run.sh $1
```

where ``$1`` is ``Online``, ``Baseline``, or ``TSF``, corresponding to ``U-online``, ``U-baseline``, ``TSF-index`` solutions, respectively.

The output is in the file ``output.txt``.

## Index update

Run the following command to simulate an index update process:

```sh
sh run.sh $1 update
```
where ``$1`` corresponds to the solution (``Baseline`` or ``TSF``).

Then input the fraction of timestamps to update. For example, if the fraction is ``0.2``, the program would firstly construct an index with edges in ``[0, 0.8*tmax]``. When the index construction is finished, it starts to update the index with remaining edges in ``[0.8*tmax, tmax]``.

## Index scalability test (Subgraph construction)

Build different sizes of subgraphs to test the scalability of indices:

```sh
sh run.sh $1 subgraph
```

where ``$1`` corresponds to the solution (``Baseline`` or ``TSF``).

Then input the fraction of timestamps in the subgraph. For example, if the fraction is ``0.8``, then the constructed temporal graph would only involve edges in ``[0, 0.8*tmax]``.

## Environment
- System: ``ubuntu-20.04.1``
- Compiler: ``gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0``
- Memory: ``128G``
- CPU: ``Intel(R) Xeon(R) Silver 4210R CPU @ 2.40GHz``

## Running examples
We provided examples about how to derive the experimental data presented in the paper.

``Figure 11`` shows the query efficiency when the length of query time windows is fixed at ``0.8*tmax``. For example, to test the query efficiency of ``U-online`` algorithm under the ``CT(contact)`` dataset:

```sh
> sh graph-gen.sh
Downloading datasets...
Fetching from http://konect.cc/files/download.tsv.contact.tar.bz2...
100%|██████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 106k/106k [00:00<00:00, 140kB/s]
Fetching from http://konect.cc/files/download.tsv.mit.tar.bz2...
100%|████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 1.07M/1.07M [00:01<00:00, 727kB/s]
Fetching from http://konect.cc/files/download.tsv.dblp_coauthor.tar.bz2...
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 131M/131M [00:40<00:00, 3.43MB/s]
Fetching from http://konect.cc/files/download.tsv.facebook-wosn-links.tar.bz2...
100%|████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 4.32M/4.32M [00:06<00:00, 697kB/s]
Fetching from http://konect.cc/files/download.tsv.youtube-u-growth.tar.bz2...
100%|███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 41.9M/41.9M [00:18<00:00, 2.42MB/s]
Fetching from http://konect.cc/files/download.tsv.wikipedia-growth.tar.bz2...
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 179M/179M [00:54<00:00, 3.45MB/s]
Extracting datasets... done
Datasets:
1. naive
2. contact
3. mit
4. facebook-wosn-links
5. youtube-u-growth
6. wikipedia-growth
7. dblp_coauthor
Select a graph dataset (0-6): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> sh query-gen.sh
Enter the number of queries to be generated: 1000
Enter the length of the query windows (0 < x < 1): 0.8
> sh run.sh Online
Compiling...
g++ source/main.cpp -c -std=c++11 -O3
g++ commonfunctions.o temporal_graph.o online_search.o baseline.o optimized_baseline.o kruskal.o differential_kruskal.o main.o -o main -O3
Running...
Building graph...
Build graph success in 13357μs (0s)
n = 275, m = 28244, tmax = 15661, size = 225952 bytes
Running online search...
Average: 3280μs (0s)
Online search completed in 3280614μs (3s)
Program finished in 3296669μs (3s)
```

``Table 4`` shows the scalability of indices. For example, to test the scalability with ``f=0.25`` of ``U-baseline`` index under the ``CT (contact)`` dataset:
```sh
> sh graph-gen.sh
Extracting datasets... done
Datasets:
0. naive
1. contact
2. mit
3. facebook-wosn-links
4. youtube-u-growth
5. wikipedia-growth
6. dblp_coauthor
Select a graph dataset (0-6): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> sh run.sh Baseline subgraph
Compiling...
make: 'main' is up to date.
Running...
Subgraph mode enabled. Please input the fraction of timestamps in the subgraph (0 < x < 1): 0.25
Building graph...
Build graph success in 15084μs (0s)
n = 275, m = 28244, tmax = 3915, size = 225952 bytes
Running baseline...
Constructing the index structure...
Index construction completed in 319360μs (0s)
Index cost 5277312 bytes
```

``Table 5`` shows the average index update time of each edge in ``[0.8*tmax, tmax]``. For example, to test the average index update time of ``U-baseline`` index under the ``CT (contact)`` dataset:
```sh
> sh graph-gen.sh
Extracting datasets... done
Datasets:
0. naive
1. contact
2. mit
3. facebook-wosn-links
4. youtube-u-growth
5. wikipedia-growth
6. dblp_coauthor
Select a graph dataset (0-6): 1
Copying dataset to "graph.txt"... done
Normalizing the graph... done
> sh run.sh Baseline update
Compiling...
make: 'main' is up to date.
Running...
Index update mode enabled. Please input the fraction of timestamps to update (0 < x < 1): 0.2  
Building graph...
Build graph success in 14230μs (0s)
n = 275, m = 28244, tmax = 15661, size = 225952 bytes
Running baseline...
Constructing the index structure...
Index construction completed in 2269964μs (2s)
Updating the index structure...
Number of edges to be updated: 5015
Index update completed in 1189767μs (1s)
Index cost 50781808 bytes
```

## Why do I fail to execute the program with the instruction?
If you fail to execute the step:

```sh
sh graph-gen.sh
```

This may be caused by several reasons:

(1) User's environment is incompatible with the program, and our automatic dependency solver crashes;

Possible solutions: check whether your Python interpreter is installed correctly, and is newer than version 3.5; run the aforementioned command again; or install the dependencies manually by:

```sh
pip install tqdm pathlib
```

(2) User's Internet condition is poor and fails to download the datasets from KONECT and SNAP websites;

Possible solutions: check your Internet; run the aforementioned command again; or download the datasets manually by:

```sh
mkdir datasets
cd datasets
wget -c "http://konect.cc/files/download.tsv.contact.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.mit.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.dblp_coauthor.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.facebook-wosn-links.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.youtube-u-growth.tar.bz2"
wget -c "http://konect.cc/files/download.tsv.wikipedia-growth.tar.bz2"
cd ..
sh graph-gen.sh
```

If you fail to execute the step:
```sh
sh run.sh $1
```

This may be caused by several reasons:

(1) Users' environment is incompatible with the program;

Possible solutions: install or update your gcc and g++ compiler;

(2) User replaces ``$1`` with wrong parameters;

Possible solutions: check the parameter format in the former of this README file.
