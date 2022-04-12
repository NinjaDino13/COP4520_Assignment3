#include <iostream>
#include <thread>
#include <mutex>
class Node;

class Node {
    public:
        int item;
        int key;
        Node* next;
        Node(int item) {
            this->item = item;
        }
};

class CoarseList {
    private:
        Node head;
    
    public:
        CoarseList();
        bool add (int item);
        bool remove (int item);
        bool contains (int item);
};

CoarseList::CoarseList() {
    Node this->head(INT_MAX);
    head.next = Node(INT_MIN);
}


int main()
{

}