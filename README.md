# DecTree
Binary Classifier using a Decision Tree.
Author: Yifan (Mark) Yu.
Oct. 2017

## About the Project
Orginally an extension task prompt (star point) for UCSD course CSE 100 Fall 17 Programming Assignment 1 (Binary search tree implementation and application). I further developed it to be a useful, reliable binary classifier using knowledge and techniques learned in the reference videos.

*Language:* C++ with Standard Template Library(STL)

*Code:* `DecTree.cpp` (All declarations and implementations are in this cpp file (just for the sake of simplicity...), though it is not a good way to program in general)

*Function implemented:* To train, validate(prune) a decision tree and use it to perform classication task on binary classication problems. However, it requires the data to be formatted and only supports string-type(discrete) attributes.

## Test and Experiment (Also a manual)
The dataset included (agaricus.data) is from: http://archive.ics.uci.edu/ml/datasets/Mushroom

To compile, simply use `make`. I already included a `Makefile`.
To run, call `DecTree.cpp`.
