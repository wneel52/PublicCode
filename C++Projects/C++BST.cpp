// Binary Search Tree Class from Professor Mike Zink's Python implementation 
#include <iostream>
using namespace std;

class TreeNode{
    public:
    int payload;
    int key;
    TreeNode* rightChild = NULL;
    TreeNode* leftChild = NULL;
    TreeNode* parent = NULL;

    TreeNode(int key, int payload) : payload(payload), key(key), rightChild(nullptr), leftChild(nullptr), parent(nullptr) {}

    void nodeInfo(){
        cout << "Current Node " << key << "payload" << payload << " Parent " << parent << " left child " << leftChild << " right child " << rightChild << endl;
    }

    bool hasLeftChild(){
        if( leftChild!=nullptr ){
            return true;
        }
        return false;
    }    

    bool hasRightChild(){
        if(rightChild!=nullptr){
            return true;
        }
        return false;
    }

    bool isLeftChild(){
        if (parent!=nullptr && parent->leftChild == this){
            return true;
        }
        return false;
    }

    bool isRightChild(){
        if (parent!=nullptr && parent->rightChild == this){
            return true;
        }
        return false;
    }

    bool isRoot(){
        return parent == nullptr;
    }

    bool isLeaf(){
        return !(rightChild || leftChild);
    }

    bool hasAnyChildren(){
        return (rightChild != nullptr || leftChild != nullptr);
    }

    bool hasBothChildren(){
        return (rightChild != nullptr && leftChild != nullptr);
    }

    void replaceNodeData(int key, int value, TreeNode* leftChild, TreeNode* rightChild){
        this->key = key;
        this->payload = value;
        this->leftChild = leftChild;
        this->rightChild = rightChild;
        if (hasLeftChild() && leftChild != nullptr){
            leftChild->parent = this;
        }
        if (hasRightChild() && rightChild != nullptr){
            rightChild->parent = this;
        }
    }

};

class BinarySearchTree{
    public:
    TreeNode* root = nullptr;
    int size = 0;

    int treeSize(){
        return size;
    }

    void put(int key, int val){
        if (root !=nullptr){
            _put(key, val, root);
        }
        else{
            root =  new TreeNode(key, val);
        }
        size++;
    }

    void _put(int key, int val, TreeNode* currentNode){
       
        if(key < currentNode->key){
            if(currentNode->hasLeftChild()){
                _put(key, val, currentNode->leftChild);
            }
            else{
                currentNode->leftChild = new TreeNode(key, val);
                currentNode->leftChild->parent = currentNode;
            }
        }
        else{
            if(currentNode->hasRightChild()){
                _put(key, val, currentNode->rightChild);
            }
            else{
                currentNode->rightChild = new TreeNode(key, val);
                currentNode->rightChild->parent = currentNode;
            }

        }

    }





};


int main() {
    // Create some TreeNode objects
    TreeNode* root = new TreeNode(10, 100);
    TreeNode* leftChild = new TreeNode(5, 50);
    TreeNode* rightChild = new TreeNode(15, 150);

    // Set parent-child relationships
    root->leftChild = leftChild;
    root->rightChild = rightChild;
    leftChild->parent = root;
    rightChild->parent = root;

    // Test the methods
    cout << "Root node: " << root->isRoot() << endl;  // Should output 1 (true)
    cout << "Left child node: " << leftChild->isLeftChild() << endl;  // Should output 1 (true)
    cout << "Right child node: " << rightChild->isRightChild() << endl;  // Should output 1 (true)
    cout << "Root node has left child: " << root->hasLeftChild() << endl;  // Should output 1 (true)
    cout << "Root node has right child: " << root->hasRightChild() << endl;  // Should output 1 (true)
    cout << "Root node is a leaf: " << root->isLeaf() << endl;  // Should output 0 (false)

    // Test the replaceNodeData method
    root->replaceNodeData(20, 200, nullptr, nullptr);  // Replace data of the root node
    cout << "Root node key after replacement: " << root->key << endl;  // Should output 20
    cout << "Root node payload after replacement: " << root->payload << endl;  // Should output 200

    // Clean up memory (free allocated memory)
    delete root;
    delete leftChild;
    delete rightChild;

    return 0;
}
