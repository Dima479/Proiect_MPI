#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include<algorithm>
  #include <windows.h>
  #include <psapi.h>
  #pragma comment(lib, "Psapi.lib")

using namespace std;
using namespace std::chrono;
vector<int> assignment;
using Clause = vector<int>;
using Formula = vector<Clause>;
int numClause=0;
Formula readDIMACS(const string &filename, int &numVars) {
    ifstream in("dimacs.txt");
    string line;
    Formula F;
    numVars = 0;
    while (getline(in, line)) {
        if (line.empty() || line[0] == 'c') { continue;}
        if (line[0] == 'p') {
            istringstream iss(line);
            string tmp; int numClauses;
            iss >> tmp >> tmp >> numVars >> numClauses;
        } else {
            istringstream iss(line);
            Clause C;
            int lit;
            while (iss >> lit && lit != 0) C.push_back(lit);
            sort(C.begin(), C.end());
            F.push_back(C); numClause++;
        }
    }
    return F;
}

bool solveResolution(Formula F, int numVars) {
    for ( int var = 1; var <= numVars; ++var) {
        Formula newF;
        vector<Clause> pos, neg;
        for (auto &C : F) {
            bool hasPos = binary_search(C.begin(), C.end(), var);
            bool hasNeg = binary_search(C.begin(), C.end(), -var);
            if (hasPos && hasNeg) continue;
            if (hasPos) pos.push_back(C);
            else if (hasNeg) neg.push_back(C);
            else newF.push_back(C);
        }
        for (auto &Cp : pos) for (auto &Cn : neg) {
            Clause R;
            set_union(Cp.begin(), Cp.end(), Cn.begin(), Cn.end(), back_inserter(R));
            R.erase(remove(R.begin(), R.end(), var), R.end());
            R.erase(remove(R.begin(), R.end(), -var), R.end());
            if (R.empty()) return false;
            sort(R.begin(), R.end());
            newF.push_back(R);
        }
        sort(newF.begin(), newF.end());
        newF.erase(unique(newF.begin(), newF.end()), newF.end());
        F.swap(newF);
    }
    return true;
}
bool solveDPLL(Formula &F, vector<int> &assignment) {
    bool changed;
    do {
        changed = false;
        for (auto &C : F) if (C.size()==1) {
            int lit = C[0];
            changed = true;
            assignment.push_back(lit);
            Formula F2;
            for (auto &D : F) {
                if (binary_search(D.begin(),D.end(), lit)) continue;
                Clause D2;
                for (int x : D) if (x != -lit) D2.push_back(x);
                if (D2.empty()) return false;
                F2.push_back(D2);
            }
            F.swap(F2);
            break;
        }
        if (changed) continue;
        unordered_set<int> lits;
        for (auto &C : F) for (int x : C) lits.insert(x);
        int pureLit = 0;
        for (int x : lits) if (!lits.count(-x)) { pureLit = x; break; }
        if (pureLit) {
            changed = true;
            assignment.push_back(pureLit);
            Formula F2;
            for (auto &C : F) {
                if (!binary_search(C.begin(), C.end(), pureLit)) F2.push_back(C);
            }
            F.swap(F2);
        }
    } while (changed);
    if (F.empty()) return true;
    int lit = F[0][0];
    {
        Formula F2 = F;
        vector<int> A2 = assignment; A2.push_back(lit);
        Formula F3;
        for (auto &C : F2) {
            if (binary_search(C.begin(), C.end(), lit)) continue;
            Clause D;
            for (int x:C) if (x != -lit) D.push_back(x);
            if (D.empty()) { F3.clear(); break; }
            F3.push_back(D);
        }
        if (!F3.empty() && solveDPLL(F3, A2)) { assignment = A2; return true; }
    }
    {
        Formula F2 = F;
        vector<int> A2 = assignment; A2.push_back(-lit);
        Formula F3;
        for (auto &C : F2) {
            if (binary_search(C.begin(), C.end(), -lit)) continue;
            Clause D;
            for (int x:C) if (x != lit) D.push_back(x);
            if (D.empty()) { F3.clear(); break; }
            F3.push_back(D);
        }
        if (!F3.empty() && solveDPLL(F3, A2)) { assignment = A2; return true; }
    }
    return false;
}
bool solveDP(Formula F, int numVars) {
    bool changed;
    bool a=false;
    do {
        changed = false;

        vector<Clause> F2;
        for (auto &C : F) {
            if (C.size() == 1) {
                int lit = C[0];
                changed = true;
                for (auto &D : F) {
                    if (!binary_search(D.begin(), D.end(), lit)) {
                        Clause D2;
                        for (int x : D) if (x != -lit) D2.push_back(x);
                        if (D2.empty()) return false;
                        F2.push_back(D2);
                    }
                }
                break;
            }
        }
        if (changed) {
            for (auto &C : F2) { sort(C.begin(), C.end()); }
            sort(F2.begin(), F2.end());
            F2.erase(unique(F2.begin(), F2.end()), F2.end());
            F.swap(F2);
            continue;
        }
        unordered_set<int> pos, neg;
        for (auto &C : F) for (int x : C) {
            if (x > 0) pos.insert(x);
            else       neg.insert(-x);
        }
        int pure = 0;
        for (int x : pos) if (!neg.count(x)) { pure = x; break; }
        if (!pure) {
            for (int x : neg) if (!pos.count(x)) { pure = -x; break; }
        }
        if (pure) {
            changed = true;
            F2.clear();
            for (auto &C : F) {
                if (!binary_search(C.begin(), C.end(), pure))
                    F2.push_back(C);
            }
            sort(F2.begin(), F2.end());
            F2.erase(unique(F2.begin(), F2.end()), F2.end());
            F.swap(F2);
        }
    } while (changed);
    return solveResolution(F, numVars);
}

int main() {
    int numVars;
    Formula F = readDIMACS("dimacs.txt", numVars);
    ofstream out;
    out.open("output.txt");
auto report = [&](const string &name, bool result, long dur){
    cout << name << ": "
         << (result ? "SAT" : "UNSAT")
         << ", Time=" << dur << "ms"<<endl;
        out<<name << ": " << (result ? "SAT" : "UNSAT")<< ", Time=" << dur << "ms"
         << "\n"<<endl;
};

string t;
cout<<"Introduceti algoritmul dorit: (dp, dpll, resolution, toate)"<<endl;
cin>>t;

if(t=="dpll")
{

auto t1 = high_resolution_clock::now();
bool resDPLL = solveDPLL(F, assignment);
auto t2 = high_resolution_clock::now();
report("DPLL", resDPLL, duration_cast<milliseconds>(t2 - t1).count() );

}
if(t=="dp")
{
    auto t1 = high_resolution_clock::now();
bool resDP2 = solveDP(F, numVars);
auto t2 = high_resolution_clock::now();
report("Davis–Putnam", resDP2, duration_cast<milliseconds>(t2 - t1).count());

}

if(t=="resolution")
{

auto t1 = high_resolution_clock::now();
bool resDP1 = solveResolution(F, numVars);
auto t2 = high_resolution_clock::now();
report("Resolution", resDP1, duration_cast<milliseconds>(t2 - t1).count());
}
if(t=="toate")
{
auto t1 = high_resolution_clock::now();
bool resDPLL = solveDPLL(F, assignment);
auto t2 = high_resolution_clock::now();
report("DPLL", resDPLL, duration_cast<milliseconds>(t2 - t1).count());


t1 = high_resolution_clock::now();
bool resDP2 = solveDP(F, numVars);
t2 = high_resolution_clock::now();
report("Davis–Putnam", resDP2, duration_cast<milliseconds>(t2 - t1).count());


 t1 = high_resolution_clock::now();
bool resDP1 = solveResolution(F, numVars);
 t2 = high_resolution_clock::now();
report("Resolution", resDP1, duration_cast<milliseconds>(t2 - t1).count());


}


    return 0;
}
