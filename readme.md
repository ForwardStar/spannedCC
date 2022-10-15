## Finding Spanned Connected Component in Temporal Graphs

A fast, elegant index structure to find all spanned connected components in a temporal graph.

How to use it:

- Write your graph data into ``graph.txt``; it contains info about all edges with the format ``(u, v, t)``; you can use the ``graph-gen.sh`` to generate graph data automatically:

```sh
sh graph-gen.sh
```

- Write your query data into ``query.txt``; it contains info about queries with the format ``[ts, te]``; you can use ``query-gen.sh`` to generate query data automatically:

```sh
sh query-gen.sh
```

- Run the following command:

```sh
sh run.sh $1
```

where ``$1`` is ``Online``, ``Baseline``, ``OBaseline``, ``Kruskal`` or ``DKruskal``, indicating which solution to use.

The ``Online`` option corresponds to the online algorithm in the paper; the ``Baseline`` option corresponds to U-baseline-index in the paper; the ``DKruskal`` option corresponds to TSF-index in the paper.

The output is in the file ``output.txt``.