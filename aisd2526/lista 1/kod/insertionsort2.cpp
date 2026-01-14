#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

long long comparisons = 0;
long long assignments = 0;

void insertionSortDouble(int arr[], int n) {
    if (n<=1) return;
    for (int i = 0; i < n; i+=2) {
        if (i+1<n) {
            comparisons++;
            if (arr[i] > arr[i+1]) {
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                assignments += 3;
            }
            int maly = arr[i];
            int duzy = arr[i+1];
            int j = i-1;
            while (j >= 0) {
                comparisons++;
                if (arr[j] > duzy) {
                    assignments++;
                    arr[j+2] = arr[j];
                    j = j-1;
                } else {
                    break;
                }
            }
            assignments++;
            arr[j+2] = duzy;
            while (j >= 0) {
                comparisons++;
                if (arr[j] > maly) {
                    assignments++;
                    arr[j+1] = arr[j];
                    j = j-1;
                } else {
                    break;
                }
            }
            assignments++;
            arr[j+1] = maly;
        }
        else {
            int key = arr[i];
            int j = i-1;
            while (j >= 0) {
                comparisons++;
                if (arr[j] > key) {
                    assignments++;
                    arr[j+1] = arr[j];
                    j = j-1;
                } else {
                    break;
                }
            }
            assignments++;
            arr[j+1] = key;
        }
    }
}



void printArray(int arr[], int n){
    for (int i=0; i<n; i++) {
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
            insertionSortDouble(arr, n);
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