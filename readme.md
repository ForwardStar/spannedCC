## Finding Spanned Connected Component in Temporal Graphs

A fast, elegant index structure to find all spanned connected components in a temporal graph.

How to use it:

- Write your graph data into ``graph.txt``; it contains info about all edges with the format $(u,v,t)$;
- Run the following command:

```sh
sh run.sh $1 $2
```

where ``$1`` is "Directed" or "Undirected", indicating the graph type; ``$2`` is "Online" or "Baseline", indicating which solution to use.

The output is in the file ``output.txt``.