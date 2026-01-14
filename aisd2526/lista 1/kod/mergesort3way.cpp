#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

long long comparisons = 0;
long long assignments = 0;

void merge3way(int arr[], int l, int m1, int m2, int r) {
    int n1 = m1 - l + 1;
    int n2 = m2 - m1;
    int n3 = r - m2;

    int* L = new int[n1 + 1];
    int* M = new int[n2 + 1];
    int* R = new int[n3 + 1];

    L[n1] = INT_MAX;
    M[n2] = INT_MAX;
    R[n3] = INT_MAX;

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
        assignments++;
    }
    for (int i = 0; i < n2; i++) {
        M[i] = arr[m1 + 1 + i];
        assignments++;
    }
    for (int i = 0; i < n3; i++) {
        R[i] = arr[m2 + 1 + i];
        assignments++;
    }

    int i = 0, j = 0, k = 0, pos = l;

    while (i < n1 && j < n2 && k < n3) {
        comparisons += 2;
        if (L[i] <= M[j] && L[i] <= R[k]) {
            arr[pos] = L[i];
            assignments++;
            pos++; i++;
        } else {
            comparisons += 2;
            if (M[j] <= L[i] && M[j] <= R[k]) {
                arr[pos] = M[j];
                assignments++;
                pos++; j++;
            } else {
                arr[pos] = R[k];
                assignments++;
                pos++; k++;
            }
        }
    }

    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= M[j]) {
            arr[pos] = L[i];
            assignments++;
            pos++; i++;
        } else {
            arr[pos] = M[j];
            assignments++;
            pos++; j++;
        }
    }

    while (i < n1 && k < n3) {
        comparisons++;
        if (L[i] <= R[k]) {
            arr[pos] = L[i];
            assignments++;
            pos++; i++;
        } else {
            arr[pos] = R[k];
            assignments++;
            pos++; k++;
        }
    }

    while (j < n2 && k < n3) {
        comparisons++;
        if (M[j] <= R[k]) {
            arr[pos] = M[j];
            assignments++;
            pos++; j++;
        } else {
            arr[pos] = R[k];
            assignments++;
            pos++; k++;
        }
    }

    while (i < n1) {
        arr[pos] = L[i];
        assignments++;
        pos++; i++;
    }
    while (j < n2) {
        arr[pos] = M[j];
        assignments++;
        pos++; j++;
    }
    while (k < n3) {
        arr[pos] = R[k];
        assignments++;
        pos++; k++;
    }

    delete[] L;
    delete[] M;
    delete[] R;
}

void mergesort3way(int arr[], int l, int r) {
    if (l < r) {
        if (r - l < 2) {
            comparisons++;
            if (arr[l] > arr[r]) {
                int temp = arr[l];
                arr[l] = arr[r];
                arr[r] = temp;
                assignments += 3;
            }
            return;
        }

        int m1 = l + (r - l) / 3;
        int m2 = l + 2 * (r - l) / 3;

        mergesort3way(arr, l, m1);
        mergesort3way(arr, m1 + 1, m2);
        mergesort3way(arr, m2 + 1, r);

        merge3way(arr, l, m1, m2, r);
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
            mergesort3way(arr, 1, n);
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

