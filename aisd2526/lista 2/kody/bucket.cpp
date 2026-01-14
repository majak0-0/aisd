#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

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
};

void bucketSort(double A[], int n) {
    LinkedList* B = new LinkedList[n];

    for (int i = 0; i < n; i++) {
        int bucket_index = std::ceil(n * A[i]) - 1;
        bucket_index = std::max(0, std::min(n - 1, bucket_index));
        B[bucket_index].insert(A[i]);
    }

    for (int j = 0; j < n; j++) {
        if (!B[j].empty()) {
            B[j].insertionSort();
        }
    }

    int index = 0;
    for (int j = 0; j < n; j++) {
        B[j].copyToArray(A, index);
    }

    delete[] B;
}

double randomDouble() {
    return static_cast<double>(rand()) / RAND_MAX;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const int n = 20;
    double A[n];

    for (int i = 0; i < n; i++) {
        A[i] = randomDouble();
    }

    std::cout << "Przed sortowaniem: ";
    for (int i = 0; i < n; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    bucketSort(A, n);

    std::cout << "Po sortowaniu: ";
    for (int i = 0; i < n; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}