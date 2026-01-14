#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

long long comparisons = 0;
long long assignments = 0;

void merge(int arr[], int p, int s, int k) {
    int n1 = s - p + 1;
    int n2 = k - s;

    int* L = new int[n1 + 1];
    int* R = new int[n2 + 1];

    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    for (int i = 0; i < n1; i++) {
        L[i] = arr[p + i];
        assignments++;
    }

    for (int j = 0; j < n2; j++) {
        R[j] = arr[s + 1 + j];
        assignments++;
    }

    int i = 0;
    int j = 0;
    for (int l = p; l <= k; l++) {

        comparisons++;
        if (L[i] <= R[j]) {
            arr[l] = L[i];
            assignments++;
            i++;
        } else {
            arr[l] = R[j];
            assignments++;
            j++;
        }
    }
    delete[] L;
    delete[] R;
}

void mergesort(int arr[], int p, int k) {
    if (p < k) {
        int s = (p + k) / 2;
        mergesort(arr, p, s);
        mergesort(arr, s + 1, k);
        merge(arr, p, s, k);
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {

    srand(time(0));
    const int roz[] = {5000, 10000, 20000, 50000, 80000, 100000, 150000};
    const int numSizes = sizeof(roz) / sizeof(roz[0]);
    const int m = 100;
    fstream file;
    file.open("wyniki.txt", ios::out);
    file << "Rozmiar tablicy,Srednie porownania,Srednie przypisania,Sredni czas (ms)\n";

    for (int i = 0; i < numSizes; i++) {
        const int n = roz[i];
        double totaltime = 0;
        long long totalassignment = 0;
        long long totalcomparisons = 0;
        cout << "testowanie dla " << n << endl;
        for (int run = 0; run < m; run++) {
            int* arr = new int[n];
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % n + 1;
            }
            comparisons = 0;
            assignments = 0;
            clock_t start = clock();
            mergesort(arr, 1, n);
            clock_t end = clock();
            double timespent = (double) (end - start) / CLOCKS_PER_SEC * 1000;
            totaltime += timespent;
            totalassignment += assignments;
            totalcomparisons += comparisons;

            delete[] arr;
        }
        double Comparisons = (double)totalcomparisons / m;
        double Assignments = (double)totalassignment / m;
        double Time = totaltime / m;

        file << "dla " << n << " ";
        file << Comparisons << " ";
        file << Assignments <<" ";
        file << Time <<endl;

    }
    file.close();
    cout << " zapisano do pliku";
    return 0;
}

