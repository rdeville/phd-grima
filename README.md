# PhD Grima

NOTE: This repos contains archived source code I developped during my PhD. It's
an old C++ code which may not be working anymore in current. You can contact me
if you want more information at [<code+grima@romaindeville.fr>](mailto:code+grima@romaindeville.fr)

Here is the input format explication of grima.
All graph for a given set (train or test) are written in one file (either
Train-GraphDB.gri or Test-GraphDB.gri).

A graph is define by the following lines:

```text
  t <ID> <ClassName>
  n <NbEdges>
  dim
  v
  v
  ...
  e
  e
  ...
```

The first line of a graph is:

```text
  t <ID> <ClassName>
```

Where `<ClassName>` is a string with the class of the graph and `<ID>` is an
integer starting from 0 for a class and should be incremented at each graph.
For instance :

```text
  t 0 class1
  ...
  t 1 class1
  ...
  t 0 class2
  ...
  t 1 class2
  ...
```

The second line of a graph is:

```text
  n <NbEdges>
```

Where `<NbEdges>` is an integer that give the total number of undirected edges in
the graph as this implementation of grima is not yet generalized to directed
edges for now.

The third line of a graph is:

```text
  dim <XDim> <YDim>
```

Where `<XDim>` and `<YDim>` are respectively the number of node in X dimension and
in Y dimension. As I work on rectangle, this version of Grima handle only
rectangle graphs.

The next _k_ line of a graph are:

```text
  v <ID> <label>
  v <ID> <label>
  ...
```

The lines starting with 'v' list all the nodes of the graphs where `<ID>` is an
integer starting from 0 and must be incremented and where `<label>` is an integer
representing the label of the node.

The next _n_ line of a graph are:

```text
  e <nodeIDFrom> <nodeIDDest> <label>
  e <nodeIDFrom> <nodeIDDest> <label>
  ...
```

The lines starting with 'n' list all edges where `<nodeIDFrom>` is the ID of the
node from which the edges start and where `<nodeIDDest>` is the ID of the node
that is reached by the edge. Finally `<label>` is an integer representing the
label of the node.

Edges are considered undirected but there is no issue if an edge is listed in
the input file in both direction. For instance, the following lines will not
raise issue:

```text
  e 0 1 255
  e 1 0 255
```

First line describe an edge that go from node 0 to node 1 with a label of 255.
Second line describe an edge that go from node 1 to node 0 with a label of 255.
