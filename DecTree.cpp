#include <cstdio>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

/* Decision Tree Binary Classifier
 * Author: Yifan Yu, Oct.12, 2017
 * (CSE 100 PA1 Extension Project)
**/

// Calculate log2(x), helper for calculating Entropy
double log_2(double x) {
    if (x <= 0) return 0;
    return log(x)/log(2);
}

/* Calculate and return the Entropy for a set
   with num_0 and num_1 data for each class */
double Entropy(int num_0, int num_1) {
    double p_0 = double(num_0)/(double)(num_0+num_1);
    double p_1 = double(num_1)/(double)(num_0+num_1);
    return -(p_0*log_2(p_0) + p_1*log_2(p_1));
}

// Class for the information of types of attributes
class Attribute_info {
private:
    int attr_id; // Attribute ID
    int val_num; // Number of possible values
    vector<string> values; // Store all possible values
    map<string, int> valmap; // Map value to value's ID

public:
    Attribute_info(const int & id) : attr_id(id), val_num(0) {
        values.clear();
    }
    bool hasValue(const string & s) {
        return (valmap.find(s) != valmap.end());
    }
    void addValue(const string & s) { // Add a new value
        values.push_back(s);
        valmap[s] = val_num;
        val_num++;
    }
    int id() const {
        return attr_id;
    }
    int vnum() const {
        return val_num;
    }
    int idLookUp(const string & s) {
        return valmap[s];
    }
    friend ostream & operator<< (ostream &os, const Attribute_info & a) {
        // Used when output to .config file
        os << a.val_num;
        for (int i = 0; i < a.values.size(); i++)
            os << ' ' << a.values[i];
        os << endl;
        return os;
    }
};

// Class for a specific attribute of a data
class Attribute {
private:
    string value;
    int value_id;

public:
    Attribute(const string & s, const int & x) : value(s), value_id(x) {}
    int getid() const {
        return value_id;
    }
};

// Class for the two classes to classify into
class Classify_type {
private:
    string tpname;
    int id; // 0 or 1

public:
    Classify_type() {}
    Classify_type(const string & name, const int & x) : tpname(name), id(x) {}
    int getid() const {
        return id;
    }
    string getname() const {
        return tpname;
    }
};

// Class for all data in dataset
class Data {
public:
    vector<Attribute> vec; // Stores all attributes of the data
    int type; // Classification (0 or 1)
    Data() {}
    Data(const int typ) : type(typ) {
        vec.clear();
    }
    void addAttr(const Attribute & atr) {
        vec.push_back(atr);
    }
};

class Tnode {
public:
    vector<Tnode*> children;

    int splitAttr; // The attribute this node split to its children
    int splitValue; // Value of attribute the node's parent split on

    int num_0, num_1; // Number of data in each classification category (0 or 1)
    int decision;
    double confidence;

    bool prune_end; // Mark if the node is currently tried for prunning

    vector<const Data*> dataset; // Pointers to all data under the node

    Tnode() : splitAttr(0), splitValue(0), num_0(0), num_1(0),
              decision(-1), confidence(0), prune_end(false) {}
    void SetSplit(const int & x) {
        splitAttr = x;
    }

    bool empty() {
        return (num_0 == 0 && num_1 == 0);
    }
};

int Attr_num; // Number of attributes for a data
bool Configured = false;
Classify_type CTYPE[2];
vector<Attribute_info> ATTRvec; // Store info for all types of attributes

// Three vector for three tasks
vector<Data> trainlist;
vector<Data> validationlist;
vector<Data> testlist;

Tnode* DecTree_root;
int NodeCounter = 0;

// Configure to the dataset
void config();

// A procedure to Setup from stdin
void manualSetup();

// Print the menu to operate the program to configure, train, prune and test
void printMenu();

// Read data with the size of Size into datalist
void readData(vector<Data> & datalist, const char* filename, int Size);

// Build the tree recursively from root down
void BuildTree(Tnode* root);

// Determine which attribute to split on, returns the ID
int findSplit(const Tnode* node);

// Calculate the information gain given node and ID of the attribute for splitting
// num_splited helps to pass the number of parts after splitting
double CalcGain(const Tnode* node, const int & split, int & num_splited);

// Using the current decision tree to predict the class of a given data d
// Returns a pair of classification decision and confidence
pair<int, double> predict(const Tnode *node, const Data & d);

// Using DFS to traverse the tree and try prune on each node
// See if certain pruning improves accuracy
void prune(Tnode* node, const Tnode* pnode, double & accy);

// Delete root and all its sub-trees
void DeleteTree(Tnode* root);

int main()
{
    char cmd;
    printf("\nDecision Tree for binary classification problems.\n");
    printf("--------------Author: Yifan Yu, 2017.-------------\n");
    printMenu();

    while (1) {
        if (!Configured) {
            printf("Need to be configured first\n");
            config();
        }
        printf("\nWaiting for command: ");
        cin >> cmd;
        if (cmd == 'h' || cmd == 'm') printMenu();
        else if (cmd == 't') {
            int Size;
            printf("Input training set size: ");
            cin >> Size;
            readData(trainlist, "train.data", Size);

            if (DecTree_root) DeleteTree(DecTree_root);
            DecTree_root = new Tnode();

            for (int i = 0; i < trainlist.size(); i++) {
                const Data* ptr = &trainlist[i];
                DecTree_root->dataset.push_back(ptr);
                if (ptr->type == 0) DecTree_root->num_0++;
                else if (ptr->type == 1) DecTree_root->num_1++;
            }

            BuildTree(DecTree_root);
            printf("Decision Tree trained! Size: %d nodes.\n", NodeCounter);
        }
        else if (cmd == 'v') {
            int Size;
            printf("Input validation set size: ");
            cin >> Size;
            readData(validationlist, "validation.data", Size);

            int Correct_num = 0;
            pair<int, double> predictResult;
            for (int i = 0; i < validationlist.size(); i++) {
                predictResult = predict(DecTree_root, validationlist[i]);
                if (predictResult.first == validationlist[i].type)
                    Correct_num++;
            }

            double accuracy_0 = double(Correct_num)/(double)validationlist.size();
            double accuracy = accuracy_0;
            double tmp = 1;
            Tnode* ptr = nullptr;
            int prune_counter = 0;

            while (tmp > accuracy) {
                tmp = accuracy;
                prune(DecTree_root, ptr, tmp);
                if (ptr) {
                    for (int i = 0; i < ptr->children.size(); i++)
                        DeleteTree(ptr->children[i]);
                    ptr->children.clear();
                    prune_counter++;
                }
            }

            printf("Pruned %d times, new decision tree size: %d nodes.\n",
                    prune_counter, NodeCounter);
        }
        else if (cmd == 'p') {
            int Size;
            printf("Input test set size: ");
            cin >> Size;
            readData(testlist, "test.data", Size);
            printf("Predict Result output in 'result.data'.\n");
            ofstream fout("result.data");

            int Correct_num = 0;
            pair<int, double> predictResult;
            for (int i = 0; i < testlist.size(); i++) {
                predictResult = predict(DecTree_root, testlist[i]);
                fout << "Test data #" << i+1 << ": Predicted Class: "
                     << CTYPE[predictResult.first].getname() << ", Confidence: "
                     << double(int(predictResult.second*10000))/100 << "%, " ;
                if (predictResult.first == testlist[i].type) {
                    Correct_num++;
                    fout << "Correct." << endl;
                }
                else fout << "Wrong." << endl;
            }

            double accuracy = double(Correct_num)/(double)testlist.size();
            printf("Finished! Correctly predicted %d out of %d\n", Correct_num, (int)testlist.size());
            printf("Accuracy: %.2lf%%.\n", accuracy*100);
        }
        else if (cmd == 'q') {
            printf("Exit!\n");
            break;
        }
    }
    return 0;
}

void printMenu() {
    printf("Commands Menu: \n");
    printf("h: help(menu)\n");
    printf("q: exit\n");
    printf("t: training using formatted 'train.data'\n");
    printf("v: validation and pruning using formatted 'validation.data'\n");
    printf("p: predict and test using data in formatted 'test.data'\n");
    printf("-------------------------------------------------------------\n\n");
}

void config() {
    ifstream fin("Classifier.config");
    Configured = true;
    if (!fin) {
        manualSetup();
        return;
    }
    else {
        printf("Classifier configuring using \"Classifier.config\".\n");
        string name[2];
        fin >> name[0] >> name[1];

        CTYPE[0] = Classify_type(name[0], 0);
        CTYPE[1] = Classify_type(name[1], 1);

        fin >> Attr_num;

        string val;
        int num;
        for (int i = 0; i < Attr_num; i++) {
            Attribute_info AT = Attribute_info(i);
            fin >> num;;
            for (int i = 0; i < num; i++) {
                fin >> val;
                AT.addValue(val);
            }
            ATTRvec.push_back(AT);
        }

        printf("Classifier successfully configured!\nType 'm' to manual configure. Type anything else to continue.\n");
        char c = getchar();
        if (c == 'm') manualSetup();
    }
}

void manualSetup() {
    ofstream fout("Classifier.config");
    printf("-------Manual Configuration-------\n");
    printf("Please input two class name for classification: ");
    string name[2];
    cin >> name[0] >> name[1];
    fout << name[0] << ' ' << name[1] << endl;

    CTYPE[0] = Classify_type(name[0], 0);
    CTYPE[1] = Classify_type(name[1], 1);

    printf("Please input number of attributes: ");
    cin >> Attr_num;
    fout << Attr_num << endl;

    string val;
    for (int i = 0; i < Attr_num; i++) {
        Attribute_info AT = Attribute_info(i);
        printf("\nPlease input all possible values for attribute #%d (split with space, end with '#'): ", i+1);
        cin >> val;
        while (val != "#") {
            if (!AT.hasValue(val))
                AT.addValue(val);
            cin >> val;
        }
        ATTRvec.push_back(AT);
        fout << AT;
    }

    printf("Configure completed!\n");
}

void readData(vector<Data> & datalist, const char* filename, int Size) {
    printf("Reading %d data from %s...", Size, filename);
    ifstream indata(filename);
    datalist.clear();
    string str;
    Data curr;
    for (int i = 0; i < Size; i++) {
        indata >> str;
        if (str == CTYPE[0].getname()) curr = Data(0);
        else curr = Data(1);
        for (int atr = 0; atr < Attr_num; atr++) {
            indata >> str;
            curr.addAttr(Attribute(str, ATTRvec[atr].idLookUp(str)));
        }
        datalist.push_back(curr);
    }
    printf("finished!\n");
}

void BuildTree(Tnode* root) {
    NodeCounter++;
    for (int i = 0; i < root->children.size(); i++) {
        if (root->children[i]) DeleteTree(root->children[i]);
        root->children[i] = nullptr;
    }
    root->children.clear();

    // Calculate decision and confidence
    if (root->num_0 >= root->num_1 && root->num_0 > 0) { // Make sure no "divide-by-zero"
        root->decision = 0;
        root->confidence = (double)root->num_0/(double)(root->num_0+root->num_1);
    }
    else if (root->num_0 < root->num_1) {
        root->decision = 1;
        root->confidence = (double)root->num_1/(double)(root->num_0+root->num_1);
    }

    int split = findSplit(root);
    root->SetSplit(split);
    if (split == -1) return;

    for (int i = 0; i < ATTRvec[split].vnum(); i++) {
        Tnode* child = new Tnode();
        child->splitValue = i;
        root->children.push_back(child);
    }

    for (int i = 0; i < root->dataset.size(); i++) {
        int val_id = root->dataset[i]->vec[split].getid();
        root->children[val_id]->dataset.push_back(root->dataset[i]);
        if (root->dataset[i]->type == 0) root->children[val_id]->num_0++;
        else if (root->dataset[i]->type == 1) root->children[val_id]->num_1++;
    }

    for (int i = 0; i < ATTRvec[split].vnum(); i++) {
        BuildTree(root->children[i]);
    }
}

int findSplit(const Tnode* node) {
    if (node->num_0 == 0 || node->num_1 == 0) return -1;

    double maxGain = -9999.99;
    int split = -1;

    int num_splited = 2;
    for (int i = 0; i < Attr_num; i++) {
        int num = 0;
        double gain = CalcGain(node, i, num);
        if (maxGain < gain) {
            maxGain = gain;
            split = i;
            num_splited = num;
        }
    }

    if (num_splited < 2) return -1; // If the best way is not to split
    return split;
}

double CalcGain(const Tnode* node, const int & split, int & num_splited) {
    double Entropy_0 = Entropy(node->num_0, node->num_1);

    map<int, pair<int, int> > count;
    int dsize = node->dataset.size();
    for (int i = 0; i < dsize; i++) {
        int val_id = node->dataset[i]->vec[split].getid();
        if (node->dataset[i]->type == 0) count[val_id].first++;
        else if (node->dataset[i]->type == 1) count[val_id].second++;
    }

    double Information_Gain = Entropy_0;
    for (int i = 0; i < ATTRvec[node->splitAttr].vnum(); i++) {
        if (count.find(i) != count.end())
            Information_Gain -= (((double)(count[i].first+count[i].second))/(double)dsize)
                                * Entropy(count[i].first, count[i].second);
    }

    num_splited = count.size(); // Pass back the num_splited to findSplit function
    return Information_Gain;
}

pair<int, double> predict(const Tnode *node, const Data & d) {
    int splitAttr = node->splitAttr;
    if (node->prune_end) return make_pair(node->decision, node->confidence);
    for (int i = 0; i < node->children.size(); i++) {
        if (!node->children[i]->empty() &&
            node->children[i]->splitValue == d.vec[splitAttr].getid())
            return predict(node->children[i], d);
    }
    return make_pair(node->decision, node->confidence);
}

void prune(Tnode* node, const Tnode* pnode, double & accy) {
    for (int i = 0; i < node->children.size(); i++) {
        if (!node->children[i]->empty())
            prune(node->children[i], pnode, accy);
    }
    node->prune_end = true;

    int Correct_num = 0;
    pair<int, double> predictResult;
    for (int i = 0; i < validationlist.size(); i++) {
        predictResult = predict(DecTree_root, validationlist[i]);
        if (predictResult.first == validationlist[i].type)
            Correct_num++;
    }

    double accuracy = double(Correct_num)/(double)validationlist.size();

    if (accuracy > accy) {
        accy = accuracy;
        pnode = node;
    }

    node->prune_end = false;
}

void DeleteTree(Tnode* root) {
    for (int i = 0; i < root->children.size(); i++)
        DeleteTree(root->children[i]);
    delete root;
    NodeCounter--;
    root = nullptr;
}
