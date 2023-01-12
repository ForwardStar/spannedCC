## Span-CC Solutions

The repository includes three solutions to solve the span-CC queries: ``U-online``, ``U-baseline`` and ``TSF-index``.

How to use it (you need to run the scripts on a Linux platform):

- Run ``graph-gen.sh`` to generate graph data automatically, which would download datasets and process the data into ``graph.txt``:

```sh
sh graph-gen.sh
```

- Run ``query-gen.sh`` to generate query data automatically, which would write queries into ``query.txt``:

```sh
sh query-gen.sh
```

- Run the following command to run the solutions:

```sh
sh run.sh $1
```

where ``$1`` is ``Online``, ``Baseline``, or ``TSF``, corresponding to ``U-online``, ``U-baseline``, ``TSF-index`` solutions, respectively.

The output is in the file ``output.txt``.

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
