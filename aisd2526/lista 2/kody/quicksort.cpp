#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
long long comparisons = 0;
long long assignments = 0;

int Partition(int A[], int poczatek, int koniec) {
    int x = A[koniec];
    int i = poczatek - 1;

    for (int j = poczatek; j <= koniec - 1; j++) {
        comparisons++;
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
            assignments+=3;
        }
    }
    swap(A[i + 1], A[koniec]);
    assignments+=3;

    return i + 1;
}

void quickSort(int A[], int p, int k) {
    if (p < k) {
        int s = Partition(A, p, k);
        quickSort(A, p, s-1);
        quickSort(A, s + 1, k);
    }
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

    string filename = "quick_sort_results_" + getTimestamp() + ".csv";
    ofstream file(filename);

    file << "Rozmiar tablicy;Srednie porownania;Srednie przypisania;Sredni czas (ms)" << endl;

    cout << "Rozpoczynam testowanie QUICKSORT..." << endl;
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
            comparisons = 0;
            assignments = 0;

            clock_t start = clock();
            quickSort(arr.data(), 0, n - 1);
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