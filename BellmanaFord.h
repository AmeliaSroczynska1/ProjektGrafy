#ifndef PROJEKTGRAFY_BELLMANAFORD_H
#define PROJEKTGRAFY_BELLMANAFORD_H

#include <vector>
#include <limits>
#include <iostream>
#include "GenerowanieGrafu.h"

using namespace std;

class BellmanaFord {
public:
    // Algorytm Bellmana-Forda dla reprezentacji listowej (lista krawędzi)
    static void znajdzNajkrotszaSciezkeListowo(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow, int start, int koniec) {
        vector<int> odleglosc(liczbaWierzcholkow, numeric_limits<int>::max());
        vector<int> poprzednik(liczbaWierzcholkow, -1);
        odleglosc[start] = 0;

        // Relaksacja krawędzi V-1 razy
        for (int i = 0; i < liczbaWierzcholkow - 1; ++i) {
            for (const auto& k : krawedzie) {
                if (odleglosc[k.poczatek] != numeric_limits<int>::max() &&
                    odleglosc[k.poczatek] + k.waga < odleglosc[k.koniec]) {
                    odleglosc[k.koniec] = odleglosc[k.poczatek] + k.waga;
                    poprzednik[k.koniec] = k.poczatek;
                }
            }
        }

        // Sprawdzenie obecności ujemnych cykli
        bool ujemnyCykl = false;
        for (const auto& k : krawedzie) {
            if (odleglosc[k.poczatek] != numeric_limits<int>::max() &&
                odleglosc[k.poczatek] + k.waga < odleglosc[k.koniec]) {
                ujemnyCykl = true;
                break;
            }
        }

        wyswietlWynik(odleglosc, poprzednik, start, koniec, ujemnyCykl, "listowa");
    }

    // Algorytm Bellmana-Forda dla reprezentacji macierzowej (macierz incydencji)
    static void znajdzNajkrotszaSciezkeMacierzowo(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow, int liczbaKrawedzi, int start, int koniec) {
        vector<vector<int>> macierz(liczbaWierzcholkow, vector<int>(liczbaKrawedzi, 0));
        vector<int> wagi(liczbaKrawedzi, 0);

        for (int e = 0; e < liczbaKrawedzi; ++e) {
            macierz[krawedzie[e].poczatek][e] = -1;
            macierz[krawedzie[e].koniec][e] = 1;
            wagi[e] = krawedzie[e].waga;
        }

        vector<int> odleglosc(liczbaWierzcholkow, numeric_limits<int>::max());
        vector<int> poprzednik(liczbaWierzcholkow, -1);
        odleglosc[start] = 0;

        // Relaksacja krawędzi V-1 razy
        for (int i = 0; i < liczbaWierzcholkow - 1; ++i) {
            for (int e = 0; e < liczbaKrawedzi; ++e) {
                int u = -1, v = -1;
                for (int w = 0; w < liczbaWierzcholkow; ++w) {
                    if (macierz[w][e] == -1) u = w;
                    else if (macierz[w][e] == 1) v = w;
                }
                if (u != -1 && odleglosc[u] != numeric_limits<int>::max() &&
                    odleglosc[u] + wagi[e] < odleglosc[v]) {
                    odleglosc[v] = odleglosc[u] + wagi[e];
                    poprzednik[v] = u;
                }
            }
        }

        // Sprawdzenie obecności ujemnych cykli
        bool ujemnyCykl = false;
        for (int e = 0; e < liczbaKrawedzi; ++e) {
            int u = -1, v = -1;
            for (int w = 0; w < liczbaWierzcholkow; ++w) {
                if (macierz[w][e] == -1) u = w;
                else if (macierz[w][e] == 1) v = w;
            }
            if (u != -1 && odleglosc[u] != numeric_limits<int>::max() &&
                odleglosc[u] + wagi[e] < odleglosc[v]) {
                ujemnyCykl = true;
                break;
            }
        }

        wyswietlWynik(odleglosc, poprzednik, start, koniec, ujemnyCykl, "macierzowa");
    }

private:
    static void wyswietlWynik(const vector<int>& odleglosc, const vector<int>& poprzednik,
                              int start, int koniec, bool ujemnyCykl, const string& typ) {
        cout << "\n--- Bellman-Ford (" << typ << ") ---\n";

        if (ujemnyCykl) {
            cout << "Graf zawiera ujemny cykl!\n";
            return;
        }

        if (odleglosc[koniec] == numeric_limits<int>::max()) {
            cout << "Brak sciezki z " << start << " do " << koniec << "\n";
            return;
        }

        cout << "Koszt najkrotszej sciezki: " << odleglosc[koniec] << "\n";
        vector<int> sciezka;
        for (int v = koniec; v != -1; v = poprzednik[v])
            sciezka.push_back(v);
        cout << "Sciezka: ";
        for (auto it = sciezka.rbegin(); it != sciezka.rend(); ++it)
            cout << *it << (it + 1 != sciezka.rend() ? " -> " : "\n");
    }
};

#endif //PROJEKTGRAFY_BELLMANAFORD_H
