#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;
long long comparisons = 0;
long long assignments = 0;

void zkopcuj3(int arr[], int n, int i) {
    int largest = i;
    int dzieciak1 = 3*i + 1;
    int dzieciak2 = 3*i + 2;
    int dzieciak3 = 3*i + 3;

    if (dzieciak1 < n) {
        comparisons++;
        if (arr[dzieciak1] > arr[largest]) {
            largest = dzieciak1;
        }
    }
    if (dzieciak2 < n){
        comparisons++;
        if (arr[dzieciak2] > arr[largest]) {
            largest = dzieciak2;
        }
    }
    if (dzieciak3 < n) {
        comparisons++;
        if (arr[dzieciak3] > arr[largest]) {
            largest = dzieciak3;
        }
    }
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        assignments += 3;

        zkopcuj3(arr, n, largest);
    }
}

void budujkopiec3(int arr[], int n) {
    for (int i = (n - 2)/3; i >= 0; i--) {
        zkopcuj3(arr, n, i);
    }
}

void kopiecsort3(int arr[], int n) {
    budujkopiec3(arr, n);
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        assignments += 3;

        zkopcuj3(arr, i, 0);
    }
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
            kopiecsort3(arr, n);
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