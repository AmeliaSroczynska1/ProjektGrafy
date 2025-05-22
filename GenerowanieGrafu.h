#ifndef GENEROWANIE_GRAFU_H
#define GENEROWANIE_GRAFU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <set>
#include <ctime>
#include "GenerowanieGrafu.h"


using namespace std;

struct Krawedz {
    int poczatek;
    int koniec;
    int waga;
    bool operator<(const Krawedz& other) const {
        return waga < other.waga;
    }
};

class GenerowanieGrafu {
public:
    int liczbaWierzcholkow;
    int liczbaKrawedzi;
    vector<Krawedz> krawedzie;

    // Wczytuje graf z pliku o podanej nazwie
    bool wczytajZPliku(const string& nazwaPliku) {
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cout << "Nie udalo sie otworzyc pliku: " << nazwaPliku << endl;
            return false;
        }
        if (!wczytajNaglowek(plik)) {
            cout << "Blad podczas wczytywania naglowka pliku." << endl;
            plik.close();
            return false;
        }
        if (!wczytajKrawedzie(plik)) {
            cout << "Blad podczas wczytywania krawedzi." << endl;
            plik.close();
            return false;
        }
        plik.close();
        return true;
    }

    // Generuje losowy graf spójny o zadanej liczbie wierzchołków i gęstości (w %)
    void generujLosowyGraf(int wierzcholki, double gestosc, int minWaga = 1, int maxWaga = 10) {
        liczbaWierzcholkow = wierzcholki;
        int maxKrawedzi = wierzcholki * (wierzcholki - 1); // dla grafu skierowanego
        liczbaKrawedzi = static_cast<int>(gestosc / 100.0 * maxKrawedzi);

        if (liczbaKrawedzi < wierzcholki - 1)
            liczbaKrawedzi = wierzcholki - 1; // minimalna liczba krawędzi dla spójności

        krawedzie.clear();
        set<pair<int, int>> istnieje;

        mt19937 rng(static_cast<unsigned>(time(nullptr)));
        uniform_int_distribution<int> wagaDist(minWaga, maxWaga);

        // 1. Najpierw generowanie drzewa rozpinającego (żeby była spójność)
        vector<int> perm(wierzcholki);
        for (int i = 0; i < wierzcholki; ++i) perm[i] = i;
        shuffle(perm.begin(), perm.end(), rng);

        for (int i = 1; i < wierzcholki; ++i) {
            int u = perm[i];
            int v = perm[rng() % i];
            int waga = wagaDist(rng);
            krawedzie.push_back({u, v, waga});
            istnieje.insert({u, v});
        }

        // 2. Dodawanie losowych krawędzi do wymaganego poziomu gęstości
        uniform_int_distribution<int> wierzDist(0, wierzcholki - 1);
        while ((int)krawedzie.size() < liczbaKrawedzi) {
            int u = wierzDist(rng);
            int v = wierzDist(rng);
            if (u == v) continue;
            if (istnieje.count({u, v})) continue;
            int waga = wagaDist(rng);
            krawedzie.push_back({u, v, waga});
            istnieje.insert({u, v});
        }
    }

    // Wyświetlanie grafu jako listy krawędzi
    void wyswietlKrawedzie() const {
        cout << liczbaKrawedzi << " " << liczbaWierzcholkow << "\n";
        for (const auto& k : krawedzie) {
            cout << k.poczatek << " " << k.koniec << " " << k.waga << "\n";
        }
    }

    // Wyswietlenie grafu w postaci listowej (lista nastepnikow/poprzednikow)
    void wyswietlListowo() const {
        vector<vector<pair<int, int>>> lista(liczbaWierzcholkow);
        for (const auto& k : krawedzie) {
            lista[k.poczatek].push_back({k.koniec, k.waga});
        }
        for (int i = 0; i < liczbaWierzcholkow; ++i) {
            cout << i << ": ";
            for (const auto& para : lista[i]) {
                cout << "(" << para.first << ", waga: " << para.second << ") ";
            }
            cout << "\n";
        }
    }

// Wyswietlenie grafu w postaci macierzowej (macierz incydencji)
    void wyswietlMacierzowo(bool skierowany = true) const {
        // liczbaWierzcholkow x liczbaKrawedzi
        vector<vector<int>> macierz(liczbaWierzcholkow, vector<int>(liczbaKrawedzi, 0));
        for (int e = 0; e < (int)krawedzie.size(); ++e) {
            int u = krawedzie[e].poczatek;
            int v = krawedzie[e].koniec;
            if (skierowany) {
                macierz[u][e] = -1; // krawędź wychodzi z u
                macierz[v][e] = 1;  // krawędź wchodzi do v
            } else {
                macierz[u][e] = 1;
                macierz[v][e] = 1;
            }
        }
        cout << "Macierz incydencji (" << liczbaWierzcholkow << " wierzcholkow x " << liczbaKrawedzi << " krawedzi):\n";
        for (int i = 0; i < liczbaWierzcholkow; ++i) {
            for (int j = 0; j < liczbaKrawedzi; ++j) {
                cout << macierz[i][j] << " ";
            }
            cout << "\n";
        }
    }


private:
    bool wczytajNaglowek(ifstream& plik) {
        string linia;
        if (!getline(plik, linia))
            return false;
        istringstream iss(linia);
        if (!(iss >> liczbaKrawedzi >> liczbaWierzcholkow))
            return false;
        return true;
    }
    bool wczytajKrawedzie(ifstream& plik) {
        krawedzie.clear();
        string linia;
        int licznik = 0;
        while (getline(plik, linia) && licznik < liczbaKrawedzi) {
            istringstream iss(linia);
            int poczatek, koniec, waga;
            if (!(iss >> poczatek >> koniec >> waga)) {
                cout << "Blad w linii krawedzi: " << linia << endl;
                return false;
            }
            krawedzie.push_back({poczatek, koniec, waga});
            licznik++;
        }
        return licznik == liczbaKrawedzi;
    }
};

#endif // GENEROWANIE_GRAFU_H
