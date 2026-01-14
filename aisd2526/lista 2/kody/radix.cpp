#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

long long assignments = 0;

int get_digit(int number, int divisor, int d) {
    return (number / divisor) % d;
}

void counting_sort_by_digit(int A[], int n, int d, int divisor) {
    int* B = new int[n];
    int* C = new int[d];

    for (int j = 0; j < d; j++) {
        C[j] = 0;
    }

    for (int i = 0; i < n; i++) {
        int digit = get_digit(A[i], divisor, d);
        C[digit]++;
    }

    for (int j = 1; j < d; j++) {
        C[j] += C[j - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = get_digit(A[i], divisor, d);
        B[C[digit] - 1] = A[i];
        C[digit]--;
    }

    for (int i = 0; i < n; i++) {
        assignments++;
        A[i] = B[i];
    }

    delete[] B;
    delete[] C;
}

void radix_sort(int A[], int n, int d, int k) {
    for (int i = 0; i < k; i++) {
        int divisor = 1;
        for (int j = 0; j < i; j++) {
            divisor *= d;
        }
        counting_sort_by_digit(A, n, d, divisor);
    }
}

int calculate_k(int max_value, int d) {
    int k = 0;
    int temp = max_value;
    while (temp > 0) {
        k++;
        temp /= d;
    }
    return k > 0 ? k : 1;
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
    const int roz[] = {1000, 5000, 10000, 25000, 50000, 75000, 10000};
    const int numSizes = sizeof(roz) / sizeof(roz[0]);
    const int m = 5;
    const int bases[] = {2, 4, 8, 10, 16};
    const int numBases = sizeof(bases) / sizeof(bases[0]);

    string filename = "radix_sort_results_" + getTimestamp() + ".csv";
    ofstream file(filename);

    file << "Rozmiar tablicy;Podstawa d;Liczba cyfr k;Srednie przypisania;Sredni czas (ms)" << endl;

    cout << "Rozpoczynam testowanie RADIX_SORT..." << endl;
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

        for (int base_idx = 0; base_idx < numBases; base_idx++) {
            const int d = bases[base_idx];
            cout << "  Podstawa d=" << d;

            double totaltime = 0;
            long long totalassignment = 0;
            int k = calculate_k(n * 2, d);

            for (int run = 0; run < m; run++) {
                vector<int> arr = test_arrays[run];
                assignments = 0;

                clock_t start = clock();
                radix_sort(arr.data(), n, d, k);
                clock_t end = clock();

                double timespent = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                totaltime += timespent;
                totalassignment += assignments;

                if (!is_sorted(arr.data(), n)) {
                    cout << " - BLAD: Tablica nieposortowana!" << endl;
                }
            }

            double avgAssignments = (double)totalassignment / m;
            double avgTime = totaltime / m;

            file << n << ";" << d << ";" << k << ";"
                 << (long long)avgAssignments << ";"
                 << formatDouble(avgTime) << endl;

            cout << " - k=" << k << ", przypisania=" << (long long)avgAssignments
                 << ", czas=" << formatDouble(avgTime) << "ms" << endl;
        }
    }

    file.close();
    cout << "\nZapisano wyniki do pliku: " << filename << endl;

    return 0;
}