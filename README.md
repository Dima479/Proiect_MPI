# SAT-Solver în C++

## Descriere

Acest proiect oferă două componente principale pentru lucrul cu probleme SAT (Satisfiability):

* **generare.cpp**: Generator de instanțe CNF în format DIMACS.
* **SAT.cpp**: Implementări ale algoritmilor de rezolvare SAT: **DPLL**, **Davis–Putnam** și **Rezoluție**.

Fișierele `dimacs.txt` și `output.txt` sunt exemple pentru formatul de intrare și ieșire.

## Conținutul repository-ului

* `generare.cpp` : Generator de formule CNF.
* `SAT.cpp`      : Solver SAT cu trei algoritmi.
* `dimacs.txt`   : Instanță de exemplu în format DIMACS.
* `output.txt`   : Exemplu de rezultat după rularea solver-ului.

## Cerințe

* Compilator C++17 (g++/clang++ pe Linux/MacOS, Visual Studio pe Windows).
* În Windows, Windows SDK pentru `<psapi.h>` (opțional, pentru raportarea memoriei).

## Compilare

### Linux / MacOS

```bash
g++ -std=c++17 generare.cpp -o generare
g++ -std=c++17 SAT.cpp -o sat
```

### Windows (Developer Command Prompt)

```bat
cl /EHsc generare.cpp /Fe:generare.exe
cl /EHsc SAT.cpp /Fe:sat.exe Psapi.lib
```

## Generare instanță DIMACS

Rulează generatorul și introdu parametrii:

```bash
./generare
```

* Numărul de variabile (NUM\_VARS)
* Numărul de clauze (NUM\_CLAUSES)
* Numerul de literali per clauză (LITS\_PER\_CLAUSE)

Fișierul `dimacs.txt` va fi actualizat cu instanța generată.

## Rulare solver SAT

Rulează solver-ul și selectează algoritmul:

```bash
./sat
```

Introdu una dintre opțiuni:

* `dp`         : Davis–Putnam
* `dpll`       : DPLL
* `resolution` : Rezoluție
* `toate`      : Rulează toți cei trei algoritmi în ordine

Rezultatele și timpii de execuție vor fi afișați în consolă și salvate în `output.txt`.

## Exemple

**dimacs.txt**:

```text
p cnf 3 3
1 -2 -3 0
1 3 -2 0
-1 2 3 0
```

**output.txt**:

```text
DPLL: SAT, Time=0ms
Davis–Putnam: SAT, Time=0ms
Resolution: SAT, Time=0ms
```

## Structura fișierelor

| Fișier       | Descriere                                    |
| ------------ | -------------------------------------------- |
| generare.cpp | Generator de instanțe DIMACS CNF             |
| SAT.cpp      | Implementări DPLL, Davis–Putnam și Rezoluție |
| dimacs.txt   | Exemplu de instanță CNF                      |
| output.txt   | Exemplu de rezultat după rulare              |
