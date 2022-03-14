## Finding Spanned Connected Component in Temporal Graphs

A fast, elegant index structure to find all spanned connected components in a temporal graph.

How to use it:

- Write your graph data into ``graph.txt``; it contains info about all edges with the format (u, v, t); you can use the ``graph-gen.sh`` to generate graph data automatically:

```sh
sh graph-gen.sh
```

- Write your query data into ``query.txt``; it contains info about queries with the format [ts, te]; you can use ``query-gen.sh`` to generate query data automatically:

```sh
sh query-gen.sh
```

- Run the following command:

```sh
sh run.sh $1
```

where ``$1`` is "Online" or "Baseline", indicating which solution to use.

The output is in the file ``output.txt``.

## Experiments

1. contact (n = 275, m = 28244, tmax = 15661, q = 100000):

Online: 2min 7s (query);

Baseline: 902M (index size), 4s (index construction), 4s (query);

Kruskal: 41140K (index size), 3s (index construction), 5s (query);

Revised Kruskal: 31324K (index size), 3s (index construction), 8s (query).

2. mit (n = 97, m = 1086404, tmax = 33451, q = 10000):

Online: 2min 14s (query);

Baseline: 722M (index size), 22s (index construction), 1s (query);

Kruskal: 46908K (index size), 24s (index construction), 1s (query);

Revised Kruskal: 40896K (index size), 27s (index construction), 1s (query).

3. dblp_coauthor (n = 1824702, m = 29487744, tmax = 76, q = 1000):

Online: 14min 29s (query);

Baseline: 26.1G (index size), 2min 6s (index construction), 5min 44s (query);

Kruskal: 1190M (index size), 11s (index construction), 5min 36s (query);

Revised Kruskal: 1118M (index size), 31s (index construction), 9min 5s (query).

4. wikipedia (n = 3223950, m = 9375437, tmax = 202, q = 100):

Online: 2min 7s (query);

Baseline: out of memory;

Kruskal: 5120M (index size), 19s (index construction), 1min 2s (query);

Revised Kruskal: 2989M (index size), 1min 21s (index construction), 1min 29s (query).