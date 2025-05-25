#ifndef PRIMA_H
#define PRIMA_H

#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>
#include "GenerowanieGrafu.h"

using namespace std;

class Prima {
public:
    // Algorytm Prima dla reprezentacji listowej
    vector<Krawedz> wykonajPrimaListowo(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow) {
        vector<vector<pair<int, int>>> listaSasiedztwa(liczbaWierzcholkow);
        for (const auto& k : krawedzie) {
            listaSasiedztwa[k.poczatek].push_back({k.koniec, k.waga});
            listaSasiedztwa[k.koniec].push_back({k.poczatek, k.waga}); // nieskierowany
        }

        vector<int> klucz(liczbaWierzcholkow, INT_MAX);
        vector<int> rodzic(liczbaWierzcholkow, -1);
        vector<bool> wMST(liczbaWierzcholkow, false);
        klucz[0] = 0;

        for (int licznik = 0; licznik < liczbaWierzcholkow - 1; licznik++) {
            // Znajdź wierzchołek o najmniejszym kluczu spoza MST
            int min = INT_MAX, u = -1;
            for (int v = 0; v < liczbaWierzcholkow; v++)
                if (!wMST[v] && klucz[v] < min)
                    min = klucz[v], u = v;
            if (u == -1) break; // graf niespójny

            wMST[u] = true;

            // Aktualizuj klucze sąsiadów
            for (const auto& para : listaSasiedztwa[u]) {
                int v = para.first;
                int waga = para.second;
                if (!wMST[v] && waga < klucz[v]) {
                    klucz[v] = waga;
                    rodzic[v] = u;
                }
            }
        }

        vector<Krawedz> mst;
        for (int v = 1; v < liczbaWierzcholkow; v++) {
            if (rodzic[v] != -1) {
                mst.push_back({rodzic[v], v, klucz[v]});
            }
        }
        return mst;
    }

    // Algorytm Prima dla reprezentacji macierzowej (macierz incydencji)
    vector<Krawedz> wykonajPrimaMacierzowo(const vector<vector<int>>& macierz, const vector<int>& wagi, int liczbaWierzcholkow, int liczbaKrawedzi) {
        vector<int> klucz(liczbaWierzcholkow, INT_MAX);
        vector<int> rodzic(liczbaWierzcholkow, -1);
        vector<bool> wMST(liczbaWierzcholkow, false);
        klucz[0] = 0;

        for (int licznik = 0; licznik < liczbaWierzcholkow - 1; licznik++) {
            int min = INT_MAX, u = -1;
            for (int v = 0; v < liczbaWierzcholkow; v++)
                if (!wMST[v] && klucz[v] < min)
                    min = klucz[v], u = v;
            if (u == -1) break; // graf niespójny

            wMST[u] = true;

            // Przeglądaj wszystkie krawędzie incydentne z u
            for (int e = 0; e < liczbaKrawedzi; ++e) {
                if (macierz[u][e] == -1) {
                    for (int v = 0; v < liczbaWierzcholkow; ++v) {
                        if (macierz[v][e] == 1 && !wMST[v] && wagi[e] < klucz[v]) {
                            klucz[v] = wagi[e];
                            rodzic[v] = u;
                        }
                    }
                }
                // analogicznie dla nieskierowanego grafu: sprawdź też odwrotność
                if (macierz[u][e] == 1) {
                    for (int v = 0; v < liczbaWierzcholkow; ++v) {
                        if (macierz[v][e] == -1 && !wMST[v] && wagi[e] < klucz[v]) {
                            klucz[v] = wagi[e];
                            rodzic[v] = u;
                        }
                    }
                }
            }
        }

        vector<Krawedz> mst;
        for (int v = 1; v < liczbaWierzcholkow; v++) {
            if (rodzic[v] != -1) {
                mst.push_back({rodzic[v], v, klucz[v]});
            }
        }
        return mst;
    }

    // Funkcja do wyświetlania MST
    static void wyswietlMST(const vector<Krawedz>& mst) {
        int suma = 0;
        cout << "Minimalne drzewo rozpinajace (Prima):\n";
        for (const auto& k : mst) {
            cout << k.poczatek << " - " << k.koniec << " (waga: " << k.waga << ")\n";
            suma += k.waga;
        }
        cout << "Calkowity koszt: " << suma << "\n";
    }
};

#endif // PRIMA_H
