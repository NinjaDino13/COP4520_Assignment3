#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <random>

class Node;
class LazyLinkedList;

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
        Node head;
        bool validate(Node* pred, Node* curr) {
            return !pred->marked && !curr->marked && pred->next == curr;
        }
    
    public:
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

LazyLinkedList present_chain;
std::vector<int> bag_of_gifts;
int fails = 0;

bool addPresent(int item) {
    return present_chain.add(item);
}

bool writeThankYou(int item) {
    return present_chain.remove(item);
}

bool checkForGift(int item) {
    return present_chain.contains(item);
}

void servant(int num) {
    for (int i = num; i < 500000; i += 4) {
        addPresent(i);
        if (checkForGift(i))
            writeThankYou(i);
        else
            fails++;
        //std::cout << "Servant " << num << " just finished gift #" << bag_of_gifts[i] << ".\n";
    }
}

int main()
{
    for(int i = 0; i < 500000; i++) {
        bag_of_gifts.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(bag_of_gifts.begin(), bag_of_gifts.end(), g);
    std::vector<std::thread> servants;

    for (int i = 0; i < 4; i++) servants.push_back(std::thread(servant, i));
    for (std::thread &serv : servants) serv.join();

    if (fails == 0)
        std::cout << "The servants have completed writing thank you notes for all 500000 gifts.";
    else
        std::cout << "The servants failed to write enough thank you notes.";

    return 0;
}