//
// Created by ameli on 23.05.2025.
//

#ifndef KRUSKAL_H
#define KRUSKAL_H


#include <vector>
#include <algorithm>
#include <iostream>
#include "GenerowanieGrafu.h"

using namespace std;

class Kruskal {
    vector<int> rodzic, ranga;
    void inicjalizuj(int n) {
        rodzic.resize(n);
        ranga.resize(n, 0);
        for (int i = 0; i < n; ++i) rodzic[i] = i;
    }
    int find(int v) {
        if (v != rodzic[v]) rodzic[v] = find(rodzic[v]);
        return rodzic[v];
    }
    void union_sets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (ranga[a] < ranga[b]) swap(a, b);
            rodzic[b] = a;
            if (ranga[a] == ranga[b]) ranga[a]++;
        }
    }
public:
    // Główna funkcja Kruskala
    vector<Krawedz> wykonajKruskal(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow) {
        vector<Krawedz> wynik;
        vector<Krawedz> krawedzieKopia = krawedzie;
        sort(krawedzieKopia.begin(), krawedzieKopia.end());
        inicjalizuj(liczbaWierzcholkow);

        for (const auto& k : krawedzieKopia) {
            if (find(k.poczatek) != find(k.koniec)) {
                wynik.push_back(k);
                union_sets(k.poczatek, k.koniec);
                if (wynik.size() == liczbaWierzcholkow - 1) break;
            }
        }
        return wynik;
    }

    // Funkcja do wyświetlania MST
    static void wyswietlMST(const vector<Krawedz>& mst) {
        int suma = 0;
        cout << "Minimalne drzewo rozpinajace (Kruskal):\n";
        for (const auto& k : mst) {
            cout << k.poczatek << " - " << k.koniec << " (waga: " << k.waga << ")\n";
            suma += k.waga;
        }
        cout << "Calkowity koszt: " << suma << "\n";
    }
};



#endif //KRUSKAL_H
