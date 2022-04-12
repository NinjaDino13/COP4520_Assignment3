#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
class Node;

class Node {
    public:
        int item;
        bool marked;
        Node* next;
        std::mutex lock;

        Node() {
            this->marked = false;
        }

        Node(int item) {
            this->item = item;
            this->marked = false;
        }
};

class LazyLinkedList {
    private:
        bool validate(Node* pred, Node* curr) {
            return !pred->marked && !curr->marked && pred->next == curr;
        }
    
    public:
        Node head;
        LazyLinkedList() {
            head.item = INT_MIN;
            this->head.next = new Node(INT_MAX);
        }

        bool add(int item) {
            while (true) {
                Node* pred = &head;
                Node* curr = head.next;
                while (curr->item < item) {
                    pred = curr; curr = curr->next;
                }
                std::lock_guard<std::mutex> lock(pred->lock);
                try {
                    std::lock_guard<std::mutex> lock(curr->lock);
                    try {
                        if (validate(pred, curr)) {
                            if (curr->item == item) {
                                std::cerr << "Failed to add " << item << " to the list.\n";
                                return false;
                            } else {
                                Node* node = new Node(item);
                                node->next = curr;
                                pred->next = node;
                                std::cout << "Succeeded in adding " << item << " to the list.\n";
                                return true;
                            }
                        }
                    } catch (...) {
                        std::cerr << "Unknown Failure Occured.\n";
                    }
                } catch (...) {
                    std::cerr << "Unknown Failure Occured.\n";
                }
            }
        }

        bool remove(int item) {
            while (true) {
                Node* pred = &head;
                Node* curr = head.next;
                while (curr->item < item) {
                    pred = curr; curr = curr->next;
                }
                std::lock_guard<std::mutex> lock(pred->lock);
                try {
                    std::lock_guard<std::mutex> lock(curr->lock);
                    try {
                        if (validate(pred, curr)) {
                            if (curr->item != item) {
                                std::cerr << "Failed to remove " << item << " from the list.\n";
                                return false;
                            } else {
                                curr->marked = true;
                                pred->next = curr->next;
                                std::cout << "Succeeded in removing " << item << " from the list.\n";
                                return true;
                            }
                        }
                    } catch (...) {
                        std::cerr << "Unknown Failure Occured.\n";
                    }
                } catch (...) {
                    std::cerr << "Unknown Failure Occured.\n";
                }
            }
        }

        bool contains(int item) {
            Node* curr = &head;
            while (curr->item < item)
                curr = curr->next;
            return curr->item == item && !curr->marked;
        }
};

void servant(int low, int high) {
    
}

int main()
{
    std::cout << "gaming" << std::endl;
    LazyLinkedList test;
    test.add(1);
    test.add(2);
    test.add(3);
    if (test.contains(1)) std::cout << "Found 1\n";
    if (test.contains(2)) std::cout << "Found 2\n";
    if (test.contains(3)) std::cout << "Found 3\n";
    test.remove(1);
    test.remove(2);
    test.remove(3);
    if (!test.contains(1)) std::cout << "Did not find 1\n";
    if (!test.contains(2)) std::cout << "Did not find 2\n";
    if (!test.contains(3)) std::cout << "Did not find 3\n";
    //std::cout << test.head.item << " " << test.head.next->item << " " << test.head.next->next->item << " " << test.head.next->next->next->item << "\n";

    return 0;
}