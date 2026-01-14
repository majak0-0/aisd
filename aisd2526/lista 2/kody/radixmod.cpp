#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;
long long comparisons = 0;
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
        A[i] = B[i];
    }

    delete[] B;
    delete[] C;
}

void radix_sort(int A[], int n, int d, int k) {
    int min_val = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] < min_val) {
            min_val = A[i];
        }
    }

    int shift = 0;
    if (min_val < 0) {
        shift = -min_val;
        for (int i = 0; i < n; i++) {
            A[i] += shift;
        }
    }

    for (int i = 0; i < k; i++) {
        int divisor = 1;
        for (int j = 0; j < i; j++) {
            divisor *= d;
        }
        counting_sort_by_digit(A, n, d, divisor);
    }

    if (shift > 0) {
        for (int i = 0; i < n; i++) {
            A[i] -= shift;
        }
    }
}

void counting_sort(int A[], int n, int k) {
    int* B = new int[n];
    int* C = new int[k + 1];

    for (int j = 0; j <= k; j++) {
        C[j] = 0;
    }

    for (int i = 0; i < n; i++) {
        C[A[i]]++;
    }

    for (int j = 1; j <= k; j++) {
        C[j] += C[j - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        B[C[A[i]] - 1] = A[i];
        C[A[i]]--;
    }

    for (int i = 0; i < n; i++) {
        A[i] = B[i];
    }

    delete[] B;
    delete[] C;
}

int main() {
    srand(time(NULL));

    const int rozmiar = 40;
    int arr[rozmiar];

    for (int i = 0; i < rozmiar; i++) {
        arr[i] = (rand() % 201) - 100;
    }

    cout << "Tablica przed sortowaniem: ";
    for (int i = 0; i < rozmiar; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    clock_t start = clock();
    radix_sort(arr, rozmiar , 10, 3 );
    clock_t end = clock();
    double timespent = (double) (end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Tablica po sortowaniu: ";
    for (int i = 0; i < rozmiar; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << timespent << " seconds." << endl;

    return 0;
}
