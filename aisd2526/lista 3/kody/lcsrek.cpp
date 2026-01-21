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

vector<vector<int>> c;
vector<vector<char>> b;
string X, Y;

int LCS_rek(int i, int j) {
    if (i < 0 || j < 0) return 0;

    if (c[i][j] != -1) return c[i][j];

    if (X[i] == Y[j]) {
        c[i][j] = LCS_rek(i-1, j-1) + 1;
        b[i][j] = 'd';
    }
    else {
        int gora = LCS_rek(i-1, j);
        int lewo = LCS_rek(i, j-1);

        if (gora >= lewo) {
            c[i][j] = gora;
            b[i][j] = 'u';
        }
        else {
            c[i][j] = lewo;
            b[i][j] = 'l';
        }
    }

    return c[i][j];
}


void print_solution
(int i, int j) {
    if (i < 0 || j < 0) return;

    if (b[i][j] == 'd') {
        print_solution(i-1, j-1);
        cout << X[i];
    }
    else if (b[i][j] == 'u') {
        print_solution(i-1, j);
    }
    else {
        print_solution(i, j-1);
    }
}

int LCS_rekurencyjny(string X1, string Y1) {
    X = X1;
    Y = Y1;
    int m = X.size();
    int n = Y.size();

    c.resize(m, vector<int>(n, -1));
    b.resize(m, vector<char>(n, ' '));

    return LCS_rek(m-1, n-1);
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
    const int lengths[]= {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000,
        5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000};
    const int m = 5;

    string filename ="lcsrek" + getTimestamp() + ".csv";
    ofstream file(filename);
    file << "Dlugosc;Czas_ms" << endl;

    for (int n : lengths) {
        double total_time =0;
        for (int run=0; run < m; run++) {
            string X = generateRandomString(n,gen);
            string Y = generateRandomString(n, gen);
            clock_t start = clock();
            LCS_rekurencyjny(X, Y);
            clock_t end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC * 1000;
            c.clear();
            b.clear();
        }
        double avg_time = total_time / m;
        file << n << ";" << formatDouble(avg_time) << endl;
    }
    file.close();
    cout << "\nZakonczono testowanie. Wyniki w pliku: " << filename << endl;

    return 0;
}
