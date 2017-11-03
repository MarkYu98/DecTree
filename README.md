# DecTree
Binary Classifier using a Decision Tree.
Author: Yifan (Mark) Yu.
Oct. 2017

## About the Project
Orginally an extension task prompt (star point) for UCSD course CSE 100 Fall 17 Programming Assignment 1 (Binary search tree implementation and application). I further developed it to be a useful, reliable binary classifier using knowledge and techniques learned in the reference videos.

**Language:** C++ with Standard Template Library(STL)

**Code:** `DecTree.cpp` (All declarations and implementations are in this cpp file (just for the sake of simplicity...), though it is not a good way to program in general)

**Function implemented:** To train, validate(prune) a decision tree and use it to perform classication task on binary classication problems. However, it requires the data to be formatted and only supports string-type(discrete) attributes.

## Test and Experiment (Also a manual)
### Set up
The dataset included (`agaricus.data`) is from: http://archive.ics.uci.edu/ml/datasets/Mushroom

To compile, simply use `make`. I already included a `Makefile`.

To run, call `DecTree.cpp`.

To use this decision tree program, first format the data as *"Class Attribute1 Attribute2 ... "* (all strings(chars)) as in `agaricus.data`, and configure the program as instructed (generate a `Classifier.config` file, I already configured it for this dataset so this config file already exists).

For the testing purpose, divide the dataset into **training**, **validation** and **test set** using `head` and `sed` shell(bash) command.

### Training and testing
For my test, I use first 5000 data as **training set**, 5001-6500(1500 data) as **validation set** for pruning and 6501-8124(1624 data) as **test set**.

Now we can start training and testing, as the program prompted 
> **Waiting for command:**

type `t` and enter, then input the number of test data to train. 
If we use all 5000 data from the training set to train, we get the result of: 

> **Reading 5000 data from train.data...finished!**
