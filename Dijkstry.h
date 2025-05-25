
#ifndef PROJEKTGRAFY_DIJKSTRY_H
#define PROJEKTGRAFY_DIJKSTRY_H


#include <iostream>
#include <vector>
#include <limits>
#include "GenerowanieGrafu.h"

class Dijkstry {
public:
    // Algorytm Dijkstry dla reprezentacji listowej (lista następników)
    static void znajdzNajkrotszaSciezkeListowo(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow, int start, int koniec) {
        // Budowa listy następników
        vector<vector<pair<int, int>>> lista(liczbaWierzcholkow);
        for (const auto& k : krawedzie)
            lista[k.poczatek].push_back({k.koniec, k.waga});

        vector<int> odleglosc(liczbaWierzcholkow, numeric_limits<int>::max());
        vector<int> poprzednik(liczbaWierzcholkow, -1);
        vector<bool> odwiedzony(liczbaWierzcholkow, false);

        odleglosc[start] = 0;

        for (int i = 0; i < liczbaWierzcholkow; ++i) {
            int u = -1;
            for (int j = 0; j < liczbaWierzcholkow; ++j)
                if (!odwiedzony[j] && (u == -1 || odleglosc[j] < odleglosc[u]))
                    u = j;

            if (odleglosc[u] == numeric_limits<int>::max())
                break;

            odwiedzony[u] = true;

            for (const auto& [v, waga] : lista[u]) {
                if (odleglosc[u] + waga < odleglosc[v]) {
                    odleglosc[v] = odleglosc[u] + waga;
                    poprzednik[v] = u;
                }
            }
        }

        wyswietlWynik(odleglosc, poprzednik, start, koniec, "listowa");
    }

    // Algorytm Dijkstry dla reprezentacji macierzowej (macierz incydencji)
    static void znajdzNajkrotszaSciezkeMacierzowo(const vector<Krawedz>& krawedzie, int liczbaWierzcholkow, int liczbaKrawedzi, int start, int koniec) {
        // Budowa macierzy incydencji
        vector<std::vector<int>> macierz(liczbaWierzcholkow, vector<int>(liczbaKrawedzi, 0));
        vector<int> wagiKrawedzi(liczbaKrawedzi, 0);

        for (int e = 0; e < liczbaKrawedzi; ++e) {
            int u = krawedzie[e].poczatek;
            int v = krawedzie[e].koniec;
            macierz[u][e] = -1; // początek
            macierz[v][e] = 1;  // koniec
            wagiKrawedzi[e] = krawedzie[e].waga;
        }

        vector<int> odleglosc(liczbaWierzcholkow, numeric_limits<int>::max());
        vector<int> poprzednik(liczbaWierzcholkow, -1);
        vector<bool> odwiedzony(liczbaWierzcholkow, false);

        odleglosc[start] = 0;

        for (int i = 0; i < liczbaWierzcholkow; ++i) {
            int u = -1;
            for (int j = 0; j < liczbaWierzcholkow; ++j)
                if (!odwiedzony[j] && (u == -1 || odleglosc[j] < odleglosc[u]))
                    u = j;

            if (odleglosc[u] == numeric_limits<int>::max())
                break;

            odwiedzony[u] = true;

            // Przeglądaj wszystkie krawędzie wychodzące z u
            for (int e = 0; e < liczbaKrawedzi; ++e) {
                if (macierz[u][e] == -1) {
                    // znajdź wierzchołek końcowy tej krawędzi
                    for (int v = 0; v < liczbaWierzcholkow; ++v) {
                        if (macierz[v][e] == 1) {
                            if (odleglosc[u] + wagiKrawedzi[e] < odleglosc[v]) {
                                odleglosc[v] = odleglosc[u] + wagiKrawedzi[e];
                                poprzednik[v] = u;
                            }
                        }
                    }
                }
            }
        }

        wyswietlWynik(odleglosc, poprzednik, start, koniec, "macierzowa");
    }

private:
    static void wyswietlWynik(const vector<int>& odleglosc, const vector<int>& poprzednik, int start, int koniec, const std::string& typ) {
        cout << "--- Dijkstry (" << typ << ") ---\n";
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


#endif //PROJEKTGRAFY_DIJKSTRY_H
