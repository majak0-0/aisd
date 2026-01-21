#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

int ACTIVITY_SELECTOR_GREEDY(const int s[], const int f[], int n, int result[]) {
    if (n < 1) return 0;

    result[0] = n;
    int count = 1;
    int k = n;

    for (int m = n - 1; m >= 1; m--) {

        if (f[m] <= s[k]) {
            result[count] = m;
            count++;
            k = m;
        }
    }


    reverse(result, result + count);

    return count;
}

int DYNAMIC_ACTIVITY_SELECTOR(const int s[], const int f[], int n, int result[]) {
    if (n < 1) return 0;

    vector<int> dp(n + 1, 0);
    vector<int> choice(n + 1, 0);

    dp[0] = 0;

    for (int i = 1; i <= n; i++) {

        int left = 0, right = i - 1, best = 0;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (f[mid] <= s[i]) {
                best = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        int take = dp[best] + 1;
        int skip = dp[i-1];

        if (take > skip) {
            dp[i] = take;
            choice[i] = 1;
        } else {
            dp[i] = skip;
            choice[i] = 0;
        }
    }


    int count = dp[n];
    vector<int> temp;
    temp.reserve(count);

    int i = n;
    while (i > 0) {
        if (choice[i] == 1) {
            temp.push_back(i);

            int left = 0, right = i - 1, best = 0;
            while (left <= right) {
                int mid = (left + right) / 2;
                if (f[mid] <= s[i]) {
                    best = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            i = best;
        } else {
            i--;
        }
    }

    reverse(temp.begin(), temp.end());
    for (int j = 0; j < count; j++) {
        result[j] = temp[j];
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

    const int lengths[] = {5000, 10000, 15000, 20000, 25000,
        30000, 35000, 40000, 45000, 50000,
        55000, 60000, 65000, 70000, 75000,
        80000, 85000, 90000, 95000, 100000};
    const int m = 100;

    string filename = "activity_comparison_" + getTimestamp() + ".csv";
    ofstream file(filename);
    file << "Dlugosc;Czas_dynamiczny_ms" << endl;

    for (int n : lengths) {
        cout << "Testowanie dla n = " << n << "..." << flush;

        double total_dp = 0;

        for (int run = 0; run < m; run++) {
            // Generowanie danych
            vector<pair<int, int>> activities(n);
            for (int i = 0; i < n; i++) {
                activities[i].first = time_dist(gen);   // czas rozpoczęcia
                activities[i].second = activities[i].first + dur_dist(gen);  // czas zakończenia
            }

            // SORTOWANIE PO CZASIE ROZPOCZĘCIA
            sort(activities.begin(), activities.end());

            int* s = new int[n+1];
            int* f = new int[n+1];
            int* result = new int[n+1];

            for (int i = 0; i < n; i++) {
                s[i+1] = activities[i].first;
                f[i+1] = activities[i].second;
            }

            // Test algorytmu dynamicznego
            clock_t start_dp = clock();
            DYNAMIC_ACTIVITY_SELECTOR(s, f, n, result);
            clock_t end_dp = clock();
            total_dp += (double)(end_dp - start_dp) / CLOCKS_PER_SEC * 1000;

            delete[] s;
            delete[] f;
            delete[] result;
        }

        double avg_dp = total_dp / m;

        file << n << ";" << formatDouble(avg_dp) << endl;
    }

    file.close();
    cout << "Wyniki zapisano do pliku: " << filename << endl;

    return 0;
}