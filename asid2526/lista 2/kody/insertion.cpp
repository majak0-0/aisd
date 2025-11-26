#include <iostream>
#include <cstdlib>
#include <ctime>

struct Node {
    double data;
    Node* next;

    Node(double val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void insert(double val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }

    void insertionSort() {
        if (!head || !head->next) return;

        Node* sorted = nullptr;
        Node* current = head;

        while (current) {
            Node* next = current->next;

            if (!sorted || sorted->data >= current->data) {
                current->next = sorted;
                sorted = current;
            } else {
                Node* temp = sorted;
                while (temp->next && temp->next->data < current->data) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }

            current = next;
        }

        head = sorted;
    }

    bool empty() const {
        return head == nullptr;
    }

    Node* getHead() const {
        return head;
    }

    void copyToArray(double arr[], int& index) const {
        Node* current = head;
        while (current) {
            arr[index++] = current->data;
            current = current->next;
        }
    }
    void display() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    srand(time(0));

    const int ELEMENT_COUNT = 20;
    LinkedList list;

    for (int i = 0; i < ELEMENT_COUNT; ++i) {
        double randomValue = (rand() % 1000);
        list.insert(randomValue);
    }

    std::cout << "Lista przed sortowaniem: ";
    list.display();

    list.insertionSort();

    std::cout << "Lista po sortowaniu: ";
    list.display();

    return 0;
}