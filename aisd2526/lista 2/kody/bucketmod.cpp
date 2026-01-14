#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
using namespace std;

struct Node {
    int data;
    Node* next;

    Node(int val) : data(val), next(nullptr) {}
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

    void insert(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }

    void insertionSort(int& comparisons, int& assignments) {
        if (!head || !head->next) return;

        Node* sorted = nullptr;
        Node* current = head;

        while (current) {
            Node* next = current->next;

            if (!sorted) {
                current->next = sorted;
                sorted = current;
                assignments += 2;
            } else {
                comparisons++;
                if (sorted->data >= current->data) {
                    current->next = sorted;
                    sorted = current;
                    assignments += 2;
                } else {
                    Node* temp = sorted;
                    while (temp->next) {
                        comparisons++;
                        comparisons++;
                        if (temp->next->data < current->data) {
                            temp = temp->next;
                        } else {
                            break;
                        }
                    }
                    current->next = temp->next;
                    temp->next = current;
                    assignments += 2;
                }
            }

            current = next;
        }

        head = sorted;
        assignments++;
    }

    bool empty() const {
        return head == nullptr;
    }

    Node* getHead() const {
        return head;
    }

    void copyToArray(int arr[], int& index) const {
        Node* current = head;
        while (current) {
            arr[index++] = current->data;
            current = current->next;
        }
    }
};

void bucketSort(int A[], int n, int& comparisons, int& assignments) {
    if (n <= 0) return;

    int min_val = A[0];
    int max_val = A[0];
    for (int i = 1; i < n; i++) {
        comparisons += 2;
        if (A[i] < min_val) min_val = A[i];
        if (A[i] > max_val) max_val = A[i];
    }

    LinkedList* B = new LinkedList[n];

    for (int i = 0; i < n; i++) {
        int bucket_index = (int)((long long)(A[i] - min_val) * n / (max_val - min_val + 1));
        B[bucket_index].insert(A[i]);
        assignments += 2;
    }

    if (min_val != max_val) {
        for (int j = 0; j < n; j++) {
            if (!B[j].empty()) {
                B[j].insertionSort(comparisons, assignments);
            }
        }
    }

    int index = 0;
    for (int j = 0; j < n; j++) {
        B[j].copyToArray(A, index);
    }

    delete[] B;
}

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

bool is_sorted(int A[], int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            return false;
        }
    }
    return true;
}

string getTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", ltm);
    return string(buffer);
}

string formatDouble(double value, int precision = 2) {
    stringstream ss;
    ss << fixed << setprecision(precision) << value;
    string str = ss.str();
    replace(str.begin(), str.end(), '.', ',');
    return str;
}

int main() {
    srand(time(0));
    const int roz[] = {1000, 5000, 10000, 25000, 50000, 75000, 100000};
    const int numSizes = sizeof(roz) / sizeof(roz[0]);
    const int m = 100;

    string filename = "bucket_sort_results_" + getTimestamp() + ".csv";
    ofstream file(filename);

    file << "Rozmiar tablicy;Srednie porownania;Srednie przypisania;Sredni czas (ms)" << endl;

    cout << "Rozpoczynam testowanie BUCKET SORT..." << endl;
    cout << "Wyniki zostana zapisane do: " << filename << endl;

    for (int i = 0; i < numSizes; i++) {
        const int n = roz[i];
        cout << "Testowanie dla rozmiaru: " << n << endl;

        vector<vector<int>> test_arrays(m, vector<int>(n));
        for (int run = 0; run < m; run++) {
            for (int j = 0; j < n; j++) {
                test_arrays[run][j] = rand() % (n * 2) + 1;
            }
        }

        double total_time = 0;
        long long total_comparisons = 0;
        long long total_assignments = 0;

        for (int run = 0; run < m; run++) {
            vector<int> arr = test_arrays[run];
            int comparisons = 0;
            int assignments = 0;

            clock_t start = clock();
            bucketSort(arr.data(), n, comparisons, assignments);
            clock_t end = clock();

            double time_spent = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            total_time += time_spent;
            total_comparisons += comparisons;
            total_assignments += assignments;

            if (!is_sorted(arr.data(), n)) {
                cout << " - BLAD: Tablica nieposortowana!" << endl;
            }
        }

        double avg_time = total_time / m;
        double avg_comparisons = (double)total_comparisons / m;
        double avg_assignments = (double)total_assignments / m;

        file << n << ";" << (long long)avg_comparisons << ";"
             << (long long)avg_assignments << ";" << formatDouble(avg_time) << endl;

        cout << " - porownania=" << (long long)avg_comparisons
             << ", przypisania=" << (long long)avg_assignments
             << ", czas=" << formatDouble(avg_time) << "ms" << endl;
    }

    file.close();
    cout << "\nZapisano wyniki do pliku: " << filename << endl;

    return 0;
}