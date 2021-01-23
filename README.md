# fastwalk

A multi-thread implementation of node2vec random walk.

### Introduction

This repository provides a multi-thread implementation of node2vec random walk, with alias table based on LRU cache, it can process with limited memory usage, so that walking through a giant graph on a single machine can be possible.

Tested for a graph that contains 23 thousand nodes and 23 million edges, with parameter

`--walk_length=80 --num_walks=10 --workers=20 --max_nodes=50000 --max_edges=100000 --p=10 --q=0.01`

only 11GB memory used, and finished walking within 2 hours.

Visit https://blog.razrlele.com/p/2650 for more.

### Prerequisites

- g++ 4.8+.

### Usage

Prepare input data with format as below:

```
node1 node2 [edge_weight]
node2 node3 [edge_weight]
...
```

edge_weight is 1.0 as default.

compile:

`make`

run with:

`./fastwalk --edge_list <path_to_edgelist> --output <path_to_output> --delimiter space --p 10 --q 0.01 --max_nodes 50000 --max_edges 50000 --workers 10`

If you wanna walk faster, add more workers, if you wanna run with less memory consumption, decrease max_nodes or max_edges, checkout more information with

`./fastwalk --help`

### Reference

> node2vec: Scalable Feature Learning for Networks.<br>
> Aditya Grover and Jure Leskovec.<br>
> Knowledge Discovery and Data Mining, 2016.<br>
> <https://arxiv.org/abs/1607.00653>

### Acknowledgements

I would like to thank Yuanyuan Zhu for discussions on the performance of node2vec, and thanks to Weichen Shen for [his great work](https://github.com/shenweichen/GraphEmbedding).
