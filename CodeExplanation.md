# Classes and Functions:
Also see code for more information.

### log_2 function: Calculate $log_2(x)$ of a real number $x$, helper for calculating Entropy.

\item
Entropy function: Calculate and return the Entropy for a set of data contains $num\_0$ data and $num\_1$ data for the respective classes.
($Entropy := -p_+log_2(p_+) - p_-log_2(p_-)$)

\item
Attribute\_info class: Class for the information of different types of attributes in the dataset. Contains attribute ID($attr\_id$), number of possible values($val\_num$), all possible values for this attribute in a vector($values$), and a map to store the corresponding ID for each value($valmap$). Some simple methods are implemented for related operations.

\item
Attribute class: Class for a specific attribute of a data. Contains the value($value$) and the value's ID($value_id$) of the attribute.

\item
Classify\_type class: Class for the two classes to classify data into. Contains the typenames($tpname$), and ID($id$, in fact just 0 or 1) for both classes. Some get method are implemented for get the private member variables.

\item
{\bf Data class}: Class for all the data of training, validation and test set. Contains a vector of attributes($vec$), the class the data belongs to($type$).

\item
{\bf Tnode class}: Class of the nodes in the decision tree. Contains a vector of children($children$), a vector of pointers to the data under current node($dataset$), variables maintaining information of the attribute this node split on to its children($splitAttr$) and the value of attribute its parent splited on($splitValue$), numbers of data of both classes in current dataset($num\_0 and num\_1$), the decision we are going to make when a validation or test data arrives at this node($decision$) along with the confidence of making such decision($confidence$), a marker to mark if this is the node(and its children) we are trying to prune when pruning($prune\_end$). Also some simple set and check methods.

\item
config function: Configure to the dataset we are going to train and test the decision tree on (attributes numbers, possible values, classes to classify into, etc.)

\item
manualSetup function: Just a procedure to Setup from stdin.

\item
printMenu function: Print the menu to operate the program to configure, train, prune and test the tree.

\item
{\bf readData function}: Read data with the size of $Size$ into the vector $datalist$(which is one of the three data vectors each for training, pruning and test).

\item
{\bf BuildTree function}: Build the tree recursively from root down. First set variables of the current node, then use findSplit function to decide which attributes to split on (or to decide whether to split or not), then create children for each possible values of the attribute and split the dataset into its child nodes. Call BuildTree for its children to continue building the tree recursively.

\item
{\bf findSplit function}: Determine which attribute to split of the current node by go through all options to get the max {\bf information gain}. Also check if all data is the same class or if the best information gain is to not split then return -1 for not splitting.

\item
CalcGain function: Calculate the information gain given the current node (dataset under it) and the attribute to try to split on, also gives back how many parts after splitting.

\item
{\bf predict function}: Using the current decision tree to predict the class of a given data $d$. Implemented by traverse the tree to a leaf node and make corresponding decision based on the decision and confidence of the node. Returns a pair of classification decision and confidence.

\item
{\bf prune function}: Using DFS to traverse the tree and try prune on each node, see if the performance (accuracy) on validation set increases. Each time select the node with most significant improvement after pruning and delete its children (make it a leaf node). Keep doing so until no improvements can be made.

\item
DeleteTree function: Delete $root$ and all sub-trees under it.
