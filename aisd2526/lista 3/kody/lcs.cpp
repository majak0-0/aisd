#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <random>
using namespace std;

void LCS_iteracyjny(string X, string Y) {
    int m = X.size();
    int n = Y.size();

    vector<vector<int>> c(m, vector<int>(n, 0));
    vector<vector<char>> b(m, vector<char>(n, ' '));

    for (int j = 0; j < n; j++) {
        if (X[0] == Y[j]) {
            c[0][j] = 1;
            b[0][j] = 'd';
        } else if (j > 0) {
            c[0][j] = c[0][j-1];
            b[0][j] = 'l';
        }
    }

    for (int i = 0; i < m; i++) {
        if (X[i] == Y[0]) {
            c[i][0] = 1;
            b[i][0] = 'd';
        } else if (i > 0) {
            c[i][0] = c[i-1][0];
            b[i][0] = 'u';
        }
    }

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            if (X[i] == Y[j]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 'd';
            }
            else if (c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = 'u';
            }
            else {
                c[i][j] = c[i][j-1];
                b[i][j] = 'l';
            }
        }
    }
}

string generateRandomString(int length, mt19937& gen) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    result.reserve(length);

    uniform_int_distribution<int> dist(0, chars.length() - 1);

    for (int i = 0; i < length; i++) {
        result += chars[dist(gen)];
    }
    return result;
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

    const int lengths[] ={ 500, 1000, 1500, 2000, 2500,
        3000, 3500, 4000, 4500, 5000, 5500,
        6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000};
    const int m = 5;

    string filename ="lcs" + getTimestamp() + ".csv";
    ofstream file(filename);
    file << "Dlugosc;Czas_ms;Wynik" << endl;

    for (int n : lengths) {
        double total_time =0;
        for (int run=0; run < m; run++) {
            string X = generateRandomString(n,gen);
            string Y = generateRandomString(n, gen);
            clock_t start = clock();
            LCS_iteracyjny(X, Y);
            clock_t end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
        }
            double avg_time = total_time / m;
            file << n << ";" << formatDouble(avg_time) << endl;
        }
        file.close();
        cout << "\nZakonczono testowanie. Wyniki w pliku: " << filename << endl;

        return 0;
    }