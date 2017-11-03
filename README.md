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
**Decision Tree trained! Size: 10 nodes.**

Now to test the Decision Tree's performance, type `p` this time, then input the number of test data. We'll see the result: 
> **Reading 1624 data from test.data...finished!  
Predict Result output in 'result.data'.!  
Finished! Correctly predicted 1581 out of 1624!  
Accuracy:! 97.35\%.**

The `result.data` file contains detail of prediction for each test data, confidence and whether it's correct.

### Performance
In my own test, the decision tree clearly works pretty well. In fact, we can know the tree is likely to be implemented correctly, by training the tree with different size of training data and test its performances, my results are as follow:
- Train with 3000 data: accuracy 860/1624, 52.96%
- Train with 4000 data: accuracy 860/1624, 52.96%
- Train with 4100 data: accuracy 1210/1624, 74.51%
- Train with 4300 data: accuracy 1071/1624, 65.95%
- Train with 4500 data: accuracy 1581/1624, 97.35%
- Train with 4800 data: accuracy 1581/1624, 97.35%

We can see that the performance is related with the size of training data.  
(Also, this dataset is pretty interesting, the performance changes dramatically from when we change from using the first 4000 data to using the first 4500 data, but changes very little when increasing the data size over 4500 or decreasing under 4000. This implies that the data is highly unbalanced, so maybe it's a good idea to random shuffle the data and then use them for the three different purposes.)

## Reference
YouTube Playlist: [Decision Tree](https://www.youtube.com/playlist?list=PLBv09BD7ez_4temBw7vLA19p3tdQH6FYO)
