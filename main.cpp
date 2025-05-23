#include <iostream>
#include <chrono>
#include "ZapisDoPliku.h"
#include "GenerowanieGrafu.h"
#include "Dijkstry.h"
#include "Kruskal.h"

using namespace std;
using namespace std::chrono;

ZapisDoPliku zapis("output.txt");
GenerowanieGrafu obsluga;

void menuWyswietlania() {
    int pokaz, wybor;
    cout << "\nJak chcesz wyswietlic graf?\n";
    cout << "1. Listowo\n";
    cout << "2. Macierzowo\n";
    cout << "3. Obie reprezentacje\n";
    cout << "Wybor: ";
    cin >> pokaz;

    if (pokaz == 1) {
        obsluga.wyswietlListowo();
    } else if (pokaz == 2) {
        cout << "Czy graf jest skierowany? (1 - tak, 0 - nie): ";
        cin >> wybor;
        obsluga.wyswietlMacierzowo(wybor == 1);
    } else if (pokaz == 3) {
        cout << "--- Reprezentacja listowa ---\n";
        obsluga.wyswietlListowo();
        cout << "\n--- Reprezentacja macierzowa ---\n";
        cout << "Czy graf jest skierowany? (1 - tak, 0 - nie): ";
        cin >> wybor;
        obsluga.wyswietlMacierzowo(wybor == 1);
    } else {
        cout << "Nieprawidlowy wybor.\n";
    }
}

int main() {
    // Zapis do pliku
    if (zapis.otworz()) {
        zapis.zapisWynikow();
        zapis.zamknij();
    }

    int wybor;
    cout << "Wybierz opcje:\n";
    cout << "1. Wczytaj graf z pliku\n";
    cout << "2. Wygeneruj graf losowo\n";
    cout << "Wybor: ";
    cin >> wybor;

    if (wybor == 1) {
        string nazwaPliku;
        cout << "Podaj nazwe pliku z grafem: ";
        cin >> nazwaPliku;

        if (obsluga.wczytajZPliku(nazwaPliku)) {
            cout << "Wczytano graf: " << obsluga.liczbaWierzcholkow << " wierzcholkow, "
                 << obsluga.liczbaKrawedzi << " krawedzi\n";
        } else {
            cout << "Nie udalo sie wczytac grafu z pliku :((\n";
            return 0;
        }
    } else if (wybor == 2) {
        int n;
        double gestosc;
        cout << "Podaj liczbe wierzcholkow: ";
        cin >> n;
        cout << "Podaj gestosc grafu (w %): ";
        cin >> gestosc;

        obsluga.generujLosowyGraf(n, gestosc);

        cout << "Wygenerowano graf: " << obsluga.liczbaWierzcholkow << " wierzcholkow, "
             << obsluga.liczbaKrawedzi << " krawedzi\n";
    } else {
        cout << "Nieprawidlowy wybor.\n";
        return 0;
    }

    menuWyswietlania();

    // Przygotowanie macierzy incydencji i wag na podstawie wygenerowanego grafu
    vector<vector<int>> macierz(obsluga.liczbaWierzcholkow, vector<int>(obsluga.liczbaKrawedzi, 0));
    vector<int> wagi(obsluga.liczbaKrawedzi, 0);

    for (int e = 0; e < obsluga.liczbaKrawedzi; ++e) {
        int u = obsluga.krawedzie[e].poczatek;
        int v = obsluga.krawedzie[e].koniec;
        macierz[u][e] = -1;
        macierz[v][e] = 1;
        wagi[e] = obsluga.krawedzie[e].waga;
    }

    Kruskal kruskal;

    // Pomiar czasu Kruskala LISTOWO
    auto startKruskalList = steady_clock::now();
    auto mst_list = kruskal.wykonajKruskal(obsluga.krawedzie, obsluga.liczbaWierzcholkow);
    auto endKruskalList = steady_clock::now();
    cout << "\n--- Kruskal LISTOWO ---\n";
    Kruskal::wyswietlMST(mst_list);
    cout << "Czas wykonania Kruskala (listowo): "
         << duration_cast<microseconds>(endKruskalList - startKruskalList).count()
         << " mikrosekund\n";

    // Pomiar czasu Kruskala MACIERZOWO
    auto startKruskalMac = steady_clock::now();
    auto mst_macierz = kruskal.wykonajKruskalMacierzowo(macierz, wagi, obsluga.liczbaWierzcholkow, obsluga.liczbaKrawedzi);
    auto endKruskalMac = steady_clock::now();
    cout << "\n--- Kruskal MACIERZOWO ---\n";
    Kruskal::wyswietlMST(mst_macierz);
    cout << "Czas wykonania Kruskala (macierzowo): "
         << duration_cast<microseconds>(endKruskalMac - startKruskalMac).count()
         << " mikrosekund\n";

    int start, koniec;
    cout << "Podaj wierzcholek startowy: ";
    cin >> start;
    cout << "Podaj wierzcholek koncowy: ";
    cin >> koniec;

    // Pomiar czasu Dijkstry listowo
    auto startDijkstraList = steady_clock::now();
    Dijkstry::znajdzNajkrotszaSciezkeListowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, start, koniec);
    auto endDijkstraList = steady_clock::now();
    cout << "Czas wykonania Dijkstry (listowo): "
         << duration_cast<microseconds>(endDijkstraList - startDijkstraList).count()
         << " mikrosekund\n";

    // Pomiar czasu Dijkstry macierzowo
    auto startDijkstraMac = steady_clock::now();
    Dijkstry::znajdzNajkrotszaSciezkeMacierzowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, obsluga.liczbaKrawedzi, start, koniec);
    auto endDijkstraMac = steady_clock::now();
    cout << "Czas wykonania Dijkstry (macierzowo): "
         << duration_cast<microseconds>(endDijkstraMac - startDijkstraMac).count()
         << " mikrosekund\n";

    return 0;
}
