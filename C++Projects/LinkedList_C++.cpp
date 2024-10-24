// Linked List -> based off of Zinks code coverted from Python to C++ 
#include <iostream>
using namespace std;

class LLnode {

    public:

        int payload;
        int ID;
        LLnode* prev = NULL;
        LLnode* next = NULL;

        LLnode(int payload) : payload(payload), ID(ID), prev(nullptr), next(nullptr) {}
        
        // Prints basic infotmation about the node we are intersed in
        void nodeInfo(){
            cout << "Current Node " << ID << " Next Node " << next << " Prev Node " << prev << endl;
        }


};

class LList{
    public:

        LLnode head = LLnode(0);
        int length = 0;
        int ID_count = 0;

        bool append(int payload){

            LLnode* newNode = new LLnode(payload);
            newNode->next = nullptr;
            newNode->ID = ID_count;

            LLnode* temp = &head;
            while(temp->next != nullptr){
                cout << "enter while" << endl;
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->prev = temp;
            

            cout << "reached 2" << endl;

            newNode->nodeInfo(); // Print the information of the newly appended node
            cout << endl;
            length += 1;
            ID_count += 1;
            return true;
        }

    void display(){

        LLnode* temp = &head;

        while(temp->next != nullptr){
            cout << temp->payload <<"->";
            temp = temp->next;
        }
        cout << temp->payload << endl;
        cout << endl;
    }

    /*
    Checks if the linked list object is empty
    Returns True given the List is empty
    Returns False given the list is non empty

    A LL is initalized with a head node with a payload of 0
    Thus to check if it is full we look to the next attribute of the head
    */ 
    bool isEmpty(){
        if(head.next == nullptr){
            return true;
        }
        else{
            return false;
        }
    }

    // removes final element of the linked list
    bool pop(){
        LLnode* temp = &head;

        if(isEmpty()){
            return false;
        }        

        while(temp->next->next != nullptr){
            temp = temp->next; 
        }
        
        delete temp->next;
        temp->next = nullptr;

        length--;
        return true;
    }

    int peek(){
        
        LLnode* temp = &head;

        if(isEmpty()){
            return false;
        }

        while(temp->next->next != nullptr){
            temp = temp->next;
        }
        
        int payload = temp->next->payload;

        cout << payload << endl;
        
        return 0;
    }

};


//EOF
