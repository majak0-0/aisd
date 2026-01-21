#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <random>
using namespace std;


int naive_cut_rod (int p[], int n) {
    if (n == 0) return 0;
    int q = INT_MIN;
    for (int i = 1; i <= n; i++) {
      q = max(q, p[i-1]+naive_cut_rod(p,n-i));
        }
    return q;
    }
int memorized_cut_rod(int p[], int n, int r[], int s[]) {
    if (r[n] >= 0) return r[n];

    if (n == 0) {
        r[0] = 0;
        s[0] = 0;
        return 0;
    } else {
        int q = INT_MIN;
        for (int i = 1; i <= n; i++) {
            int current = p[i-1] + memorized_cut_rod(p, n-i, r, s);
            if (q < current) {
                q = current;
                s[n] = i;
            }
        }
        r[n] = q;
        return q;
    }
}

void print_solution_memo(int p[], int n) {
    int r[n+1];
    int s[n+1];
    int max_zysk = memorized_cut_rod(p, n, r, s);
    cout << "max zysk (memo): " << max_zysk << endl;
    cout << "optimalne ciecia: ";
    int pozostalo = n;
    while (pozostalo > 0) {
        cout << s[pozostalo] << " ";
        pozostalo -= s[pozostalo];
    }
    cout << endl;
}
int ext_cut_rod(const int p[], int n, int r[], int s[]) {
    r[0] = 0;
    s[0] = 0;

    for (int j = 1; j <= n; j++) {
        int q = INT_MIN;
        s[j] = 0;

        for (int i = 1; i <= j; i++) {
            int current = p[i-1] + r[j - i];
            if (q < current) {
                q = current;
                s[j] = i;
            }
        }

        r[j] = q;
    }
    return r[n];
}
void print_solution_ext(int p[], int n) {
    int r[n+1];
    int s[n+1];
    int max_zysk = ext_cut_rod(p, n, r, s);
    cout << "max zysk: " <<max_zysk << endl;
    cout << "optimalne ciecia: ";
    int pozostalo = n;
    while (pozostalo > 0) {
        cout << s[pozostalo] << " ";
        pozostalo -= s[pozostalo];
    }
    cout << endl;
}

string getTimestamp() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", ltm);
    return (buffer);
}

string formatDouble(double value, int precision = 2) {
    stringstream ss;
    ss << fixed << setprecision(precision) << value;
    string str = ss.str();
    replace(str.begin(), str.end(), '.', ',');
    return str;
}


int main() {
    srand(time(nullptr));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 10);

    const int naive_lengths[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    const int m_naive = 100;

    string filename_naive ="naive_cut_rod" + getTimestamp() + ".csv";
    ofstream naive_file(filename_naive);
    naive_file << "Dlugosc;Czas_ms" << endl;

    for (int n : naive_lengths) {
        int* prices = new int[n];
        prices[0] = dist(gen);
        for (int j = 1; j < n; j++) {
            prices[j] = prices[j-1] + dist(gen);
        }

        double total_time = 0;

        for (int run = 0; run < m_naive; run++) {
            clock_t start = clock();
            naive_cut_rod(prices, n);
            clock_t end = clock();

            double time_spent = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            total_time += time_spent;
        }

        double avg_time = total_time / m_naive;

        naive_file << n << ";" << formatDouble(avg_time) << endl;
        delete[] prices;
    }

    naive_file.close();

/*
    const int dp_lengths[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
        1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};
    string filename_dp ="dp_lengths" + getTimestamp() + ".csv";
    ofstream dp_file( filename_dp);
    dp_file << "Dlugosc;Czas_Memo_ms;Czas_Ext_ms;" << endl;

    for (int n : dp_lengths) {
        const int m_dp = 100;
        int* prices = new int[n];
        prices[0] = dist(gen);
        for (int j = 1; j < n; j++) {
            prices[j] = prices[j-1] + dist(gen);
        }

        double total_time_memo = 0, total_time_ext = 0;


        for (int run = 0; run < m_dp; run++) {
            // Memoized
            int* r_memo = new int[n+1];
            int* s_memo   = new int[n+1];
            for (int k = 0; k <= n; k++) r_memo[k] = INT_MIN;

            clock_t start = clock();
            memorized_cut_rod(prices, n, r_memo, s_memo);
            clock_t end = clock();

            total_time_memo += (double)(end - start) / CLOCKS_PER_SEC * 1000;

            delete[] r_memo;

            // Extended
            int* r_ext = new int[n+1];
            int* s_ext = new int[n+1];

            clock_t start_ext = clock();
            ext_cut_rod(prices, n, r_ext, s_ext);
            clock_t end_ext = clock();

            total_time_ext += (double)(end_ext - start_ext) / CLOCKS_PER_SEC * 1000;


            delete[] r_ext;
            delete[] s_ext;
        }

        double avg_time_memo = total_time_memo / m_dp;
        double avg_time_ext = total_time_ext / m_dp;

        dp_file << n << ";"
                << formatDouble(avg_time_memo) << ";"
                << formatDouble(avg_time_ext) << endl;

        delete[] prices;
    }

    dp_file.close();
*/
    return 0;
}

