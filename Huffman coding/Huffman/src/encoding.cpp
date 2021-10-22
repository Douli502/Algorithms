/*
 * TDDD86 encoding
 * Douglas Liao (douli502)
 *
 * This file contains implementation of Huffmancoding. Reading and writing tools is used from bitstream.h.
 * Creating the tree by using HuffmanNode.h.
 */
#include "encoding.h"
#include <queue>
#include <vector>
#include <map>

/*
 * Creates a frequency table, holding the amount of char appearens in file.
 */
map<int, int> buildFrequencyTable(istream& input) {
    input.clear();  // Removes any current eof/failure flags.
    input.seekg(0, ios::beg);  // Tells the stream to seek back to the beginning.

    map<int, int> freqTable;
    int key = input.get();
    while(key != -1) {
        // Add key to freqtable or increment val of key.
        freqTable.count(key) ? freqTable[key]++ : freqTable[key] = 1;
        key = input.get();
    }
    // Add end of file as a key.
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}
struct compare {
    bool operator()(const HuffmanNode* lhs, HuffmanNode* rhs) {
        return *lhs < *rhs;
    }
};

/*
 * Creates an encoding node tree based on freqency table.
 */
HuffmanNode* buildEncodingTree(const map<int, int>& freqTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> queue;
    // Use key and val as inparameters for HuffmanNode .
    // Add HuffmanNode to a priority queue.
    for(auto freq:freqTable) {
        queue.push(new HuffmanNode(freq.first, freq.second));
    }
    /*
     * The algorithm for building the tree is following:
     * Lopp:
     * Pop the two top elem (elem with lowest count).
     * Create a new HuffmanNode with count as the sum of the two pop elem->count
     * The popped elem will be left and right child of the new HuffmanNode
     * End if queue only has one elem
     * Jump to Loop
     */
    while(queue.size() != 1) {
        HuffmanNode* zero = queue.top();
        queue.pop();
        HuffmanNode* one = queue.top();
        queue.pop();
        int count = zero->count + one->count;
        queue.push(new HuffmanNode(NOT_A_CHAR, count, zero,one));
    }
    return queue.top();
}

/*
 * Creates a map that represent the tree.
 */
map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    /*
     *  Going left in the tree is represented as '0' respective '1' for right
     *  Every Character in the tree will have a representation in binary
     */
    map<int, string> encodingMap;
    string binary = "";
    inorder(encodingTree, encodingMap, binary);
    return encodingMap;
}

/*
 * Encodes each character based on encodingmap and returns binary reprensentation of input
 */
void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    input.clear();  // Removes any current eof/failure flags.
    input.seekg(0, ios::beg);  // Tells the stream to seek back to the beginning.

    int key = input.get();
    while(key != -1) {
        // Write the chars representation in binary to the obitstream (write to file).
        for(const char& c : encodingMap.at(key)) {
            output.writeBit(c - '0');
        }
        key = input.get();
    }
    // Write binary of PSEUDO_EOF to the obitstream (write to file).
    for(const char& c : encodingMap.at(PSEUDO_EOF)) {
        output.writeBit(c - '0');
    }
}

/*
 * Decodes the node tree.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    input.clear();  // Removes any current eof/failure flags.
    input.seekg(0, ios::beg);  // Tells the stream to seek back to the beginning.

    // Translate binary code to string by traversing data on tree
    int seq = decodeSeq(input,encodingTree);
    while(seq != PSEUDO_EOF) {
        output.put(seq);
        seq = decodeSeq(input,encodingTree);
    }   
}

/*
 * Compresses the inputs using the encoding and the saves it to output file.
 */
void compress(istream& input, obitstream& output) {
    input.clear();  // Removes any current eof/failure flags.
    input.seekg(0, ios::beg);  // Tells the stream to seek back to the beginning.

    map<int,int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    map<int,string> encodingMap = buildEncodingMap(encodingTree);
    // Write data as a header accordingly.
    string data = "{";
    for(auto const& elem: freqTable) {
        data += to_string(elem.first) + ":" + to_string(elem.second) + ", ";
    }
    data.pop_back();
    data.pop_back();
    data += "}";
    for(char const c:data) {
        output.put(c); // Write to header
    }
    encodeData(input, encodingMap, output);
    freeTree(encodingTree); 
}

/*
 * Decompresses encoded inputs and saves it to output file.
 */
void decompress(ibitstream& input, ostream& output) {
    map<int,int> freqTable;
    char curChar = input.get();
    string curKey;
    string curVal;  
    while(curChar != '}'){
        if (curChar == ':' || curChar == ' ' || curChar == '{'){
            /* Skipped*/
        } else if(curChar == ','){
            // Done with first element
            // Stoi convert string to int
            int key = stoi(curKey);
            int val = stoi(curVal);
            freqTable[key] = val;
            // Reset variables
            curKey.clear();
            curVal.clear();
        } else{
            curKey += curChar;
        }
        curChar = input.get();
    }
    // Add end of file as key.
    freqTable[PSEUDO_EOF] = 1;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/*
 * Traverse the tree to deallocate memory.
 */
void freeTree(HuffmanNode* node) {
    if(node != nullptr) {
        freeTree(node->one);
        freeTree(node->zero);
        delete node; //deallocate memory
    }
    return;
}

/*
 * Helper function.
 */
void inorder(HuffmanNode *encodingTree, map<int, string>& encodingMap, string binary) {
    // End search when reaching a leaf
    if(encodingTree->isLeaf()) {
        encodingMap.insert({encodingTree->character, binary});
        return;
    } else {
        inorder(encodingTree->zero, encodingMap, binary + "0");
        inorder(encodingTree->one, encodingMap, binary + "1");
    }
}

/*
 * Recursive function for traversing encoding tree according to input.
 */
int decodeSeq(ibitstream &input, HuffmanNode *encodingTree) {
    if(encodingTree->isLeaf()) {
        return encodingTree->character;
    }
    return input.readBit() ? decodeSeq(input, encodingTree->one):
                             decodeSeq(input, encodingTree->zero);
}

