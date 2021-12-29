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

## Experiment Results

The experiment is performed on Linux platform and Intel(R) Xeon(R) Silver 4210R CPU @ 2.40GHz with 126GB RAM.

Caching: ✅

Optimized I/O manipulations: ❌

Multithreading: ❌

<table align="center">
    <tr>
        <td align="center", colspan="2"> <b>Parameter </td align="center">
        <td align="center"> <I><b>contact </td align="center"> 
        <td align="center"> <I><b>mit </td align="center">
        <td align="center"> <I><b>dblp_coauthor </td align="center">
    <tr>
        <td align="center" rowspan="3"> <I>data statistics </td align="center">
        <td align="center"> <I>n </td align="center"> <td align="center"> 274 </td align="center"> <td align="center"> 96 </td align="center"> <td align="center"> 1824701 </td align="center">
        <tr>
        <td align="center"> <I>m </td align="center"> <td align="center"> 28244 </td align="center"> <td align="center"> 1086404 </td align="center"> <td align="center"> 29487744 </td align="center">
        <tr>
        <td align="center"> <I>tmax </td align="center"> <td align="center"> 15661 </td align="center"> <td align="center"> 33451 </td align="center"> <td align="center"> 76 </td align="center">
    <tr>
        <td align="center" rowspan="2"> <I>Index construction </td> <td align="center"> <I>time </td> <td align="center"> 44s </td> <td align="center"> 14min 10s </td> <td align="center"> 41min 39s </td>
        <tr>
        <td align="center"> <I>space </td> <td align="center"> 2571M </td> <td align="center"> 2140M </td> <td align="center"> 81.5G </td>
    <tr>
        <td align="center" rowspan="4"> <I>query num </td align="center">
        <td align="center"> 100 </td align="center"> <td align="center"> &lt 1s <p> &lt 1s </td align="center"> <td align="center"> 15s <p> &lt 1s </td align="center"> <td align="center"> 11min 34s <p> 3min 29s </td align="center">
        <tr>
        <td align="center"> 1000 </td align="center"> <td align="center"> 1s <p> 1s </td align="center"> <td align="center"> 2min 30s <p> &lt 1s</td align="center"> <td align="center"> / </td align="center">
        <tr>
        <td align="center"> 10000 </td align="center"> <td align="center"> 15s <p> 2s </td align="center"> <td align="center"> 25min 16s <p> 1s </td align="center"> <td align="center"> / </td align="center">
        <tr>
        <td align="center"> 100000 </td align="center"> <td align="center"> 2min 14s <p> 27s </td align="center"> <td align="center"> / <p> 17s </td align="center"> <td align="center"> /</td align="center">
    <tr>
</table>