## Finding Spanned Connected Component in Temporal Graphs

A fast, elegant index structure to find all spanned connected components in a temporal graph.

How to use it:

- Write your graph data into ``graph.txt``; it contains info about all edges with the format $(u,v,t)$; you can use the ``graph-gen.sh`` to generate graph data automatically:

```sh
sh graph-gen.sh
```

- Write your query data into ``query.txt``; it contains info about queries with the format $[t_s,t_e]$; you can use ``query-gen.sh`` to generate query data automatically:

```sh
sh query-gen.sh
```

- Run the following command:

```sh
sh run.sh $1
```

where ``$1`` is "Online" or "Baseline", indicating which solution to use.

The output is in the file ``output.txt``.

## Experiment Results

The experiment is performed on Linux platform and Intel(R) Xeon(R) Silver 4210R CPU @ 2.40GHz with 126GB RAM.

Caching: ✅

Optimized I/O manipulations: ❌

Multithreading: ❌

<table>
    <td> </td>
    <tr>
        <td> </td>
        <td> </td>
        <td> <I><b>contact </td> 
        <td> <I><b>mit </td>
        <td> <I><b>dblp_coauthor </td>
    <tr>
        <td rowspan="3"> <I>data statistics </td>
        <td> <I>n </td> <td> 274 </td> <td> 96 </td> <td> 1824701 </td>
        <tr>
        <td> <I>m </td> <td> 28244 </td> <td> 1086404 </td> <td> 29487744 </td>
        <tr>
        <td> <I>tmax </td> <td> 15661 </td> <td> 33451 </td> <td> 76 </td>
    <tr>
        <td rowspan="4"> <I>query num </td>
        <td> 100 </td> <td> &lt 1s <p> &lt 1s </td> <td> 15s <p> &lt 1s </td> <td> 11min 34s <p> 3min 29s </td>
        <tr>
        <td> 1000 </td> <td> 1s <p> 1s </td> <td> 2min 30s <p> &lt 1s</td> <td> / </td>
        <tr>
        <td> 10000 </td> <td> 15s <p> 2s </td> <td> 25min 16s <p> 1s </td> <td> / </td>
        <tr>
        <td> 100000 </td> <td> 2min 14s <p> 27s </td> <td> / <p> 17s </td> <td> /</td>
    <tr>
    <td> </td> <td> </td> <td> </td> <td> </td> <td> </td>
</table>