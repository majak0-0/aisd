#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;
using namespace std::chrono;


int RECURSIVE_ACTIVITY_SELECTOR(
    const int s[], const int f[],
    int n, int k, int result[]
) {
    int m = k + 1;

    while (m < n && k != -1 && s[m] < f[k])
        m++;

    if (m < n) {
        result[0] = m;
        return 1 + RECURSIVE_ACTIVITY_SELECTOR(s, f, n, m, result + 1);
    }
    return 0;
}


int ACTIVITY_SELECTOR(
    const int s[], const int f[],
    int n, int result[]
) {
    if (n == 0) return 0;

    result[0] = 0;
    int count = 1;
    int k = 0;

    for (int m = 1; m < n; m++) {
        if (s[m] >= f[k]) {
            result[count++] = m;
            k = m;
        }
    }
    return count;
}


string getTimestamp() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", ltm);
    return string(buffer);
}

string formatDouble(double value, int precision = 6) {
    stringstream ss;
    ss << fixed << setprecision(precision) << value;
    string str = ss.str();
    replace(str.begin(), str.end(), '.', ',');
    return str;
}


int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> time_dist(0, 1000);
    uniform_int_distribution<int> dur_dist(1, 100);

    const int lengths[] = {
        5000, 10000, 15000, 20000, 25000,
        30000, 35000, 40000, 45000, 50000,
        55000, 60000, 65000, 70000, 75000,
        80000, 85000, 90000, 95000, 100000
    };

    const int m = 100;

    string filename = "activity_selector_" + getTimestamp() + ".csv";
    ofstream file(filename);
    file << "Dlugosc;Czas_rek_ms;Czas_iter_ms\n";

    for (int n : lengths) {
        cout << "Testowanie dla n = " << n << "...\n";

        // Przygotuj wszystkie zestawy danych z góry
        vector<vector<pair<int, int>>> all_activities(m);
        for (int run = 0; run < m; run++) {
            all_activities[run].resize(n);
            for (int i = 0; i < n; i++) {
                all_activities[run][i].second = time_dist(gen);
                all_activities[run][i].first = all_activities[run][i].second + dur_dist(gen);
            }
            sort(all_activities[run].begin(), all_activities[run].end());
        }

        // Test rekurencyjny - wszystkie iteracje razem
        clock_t start_rek = clock();
        for (int run = 0; run < m; run++) {
            int* s = new int[n];
            int* f = new int[n];
            int* result = new int[n];

            for (int i = 0; i < n; i++) {
                s[i] = all_activities[run][i].second;
                f[i] = all_activities[run][i].first;
            }

            RECURSIVE_ACTIVITY_SELECTOR(s, f, n, -1, result);

            delete[] s;
            delete[] f;
            delete[] result;
        }
        clock_t end_rek = clock();
        double total_rek = (double)(end_rek - start_rek) / CLOCKS_PER_SEC * 1000.0;

        // Test iteracyjny - wszystkie iteracje razem
        clock_t start_iter = clock();
        for (int run = 0; run < m; run++) {
            int* s = new int[n];
            int* f = new int[n];
            int* result = new int[n];

            for (int i = 0; i < n; i++) {
                s[i] = all_activities[run][i].second;
                f[i] = all_activities[run][i].first;
            }

            ACTIVITY_SELECTOR(s, f, n, result);

            delete[] s;
            delete[] f;
            delete[] result;
        }
        clock_t end_iter = clock();
        double total_iter = (double)(end_iter - start_iter) / CLOCKS_PER_SEC * 1000.0;

        double avg_rek = total_rek / m;
        double avg_iter = total_iter / m;

        file << n << ";" << formatDouble(avg_rek, 3) << ";" << formatDouble(avg_iter, 3) << endl;
        cout << "  Sredni czas rek: " << avg_rek << " ms, iter: " << avg_iter << " ms\n";
    }

    file.close();
    cout << "\nZakonczono testowanie. Wyniki w pliku: " << filename << endl;

    return 0;
}