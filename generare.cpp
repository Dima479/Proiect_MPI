#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
int main() {
    int NUM_VARS;
    int NUM_CLAUSES;
    int LITS_PER_CLAUSE;
    cout<<"Introduceti numarul de variabile:";
    cin>>NUM_VARS;
    cout<<"Introduceti numarul de clauze:";
    cin>>NUM_CLAUSES;
    cout<<"Introduceti numarul de literali in ficare clauza:";
    cin>>LITS_PER_CLAUSE;
    const char* OUT_FILE      = "dimacs.txt";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> varDist(1, NUM_VARS);
    std::uniform_int_distribution<> signDist(0, 1);
    std::ofstream out(OUT_FILE);
    if (!out) {
        std::cerr << "Eroare: nu pot deschide fișierul " << OUT_FILE << "\n";
        return 1;
    }
    out << "p cnf " << NUM_VARS << " " << NUM_CLAUSES << "\n";
    std::vector<int> pool(NUM_VARS);
    for (int i = 0; i < NUM_VARS; ++i) pool[i] = i + 1;

    for (int c = 0; c < NUM_CLAUSES; ++c) {
        std::shuffle(pool.begin(), pool.end(), gen);
        for (int j = 0; j < LITS_PER_CLAUSE; ++j) {
            int var = pool[j];
            int sign = signDist(gen) ? +1 : -1;
            out << (sign * var) << " ";
        }
        out << "0\n";
    }
    out.close();
    std::cout << "Am generat " << NUM_CLAUSES << " clauze in "
              << OUT_FILE << " cu " << NUM_VARS
              << " variabile si cate "
              << LITS_PER_CLAUSE << " literali per clauza.\n";
              exit(0);
    return 0;
}
