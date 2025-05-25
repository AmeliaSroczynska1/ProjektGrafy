#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "ZapisDoPliku.h"
#include "GenerowanieGrafu.h"
#include "Dijkstry.h"
#include "Kruskal.h"
#include <iomanip>


using namespace std;
using namespace std::chrono;

ZapisDoPliku zapis("output.txt");
GenerowanieGrafu obsluga;

int main() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych

    int n;
    int gestosc;
    mt19937 rng(static_cast<unsigned>(time(nullptr)));

    // 1. Wczytaj dane z pliku lub wygeneruj graf losowo
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
        cout << "Podaj liczbe wierzcholkow: ";
        cin >> n;
        cout << "Podaj gestosc grafu (w %): ";
        cin >> gestosc;
    } else {
        cout << "Nieprawidlowy wybor.\n";
        return 0;
    }

    // Zmienne do sumowania czasów
    long long sumaDijkstraList = 0;
    long long sumaDijkstraMac = 0;
    long long sumaKruskalList = 0;
    long long sumaKruskalMac = 0;
    int liczba_iteracji;

    cout << "Podaj liczbe iteracji do testowania algorytmow: ";
    cin >> liczba_iteracji;

    for(int i = 0; i < liczba_iteracji; i++) {

        cout << "\n \n \n--- Iteracja " << i + 1 << " ---\n";

        if(wybor == 2){
            obsluga.generujLosowyGraf(n, gestosc, rng);

            cout << "Wygenerowano graf: " << obsluga.liczbaWierzcholkow << " wierzcholkow, "
                 << obsluga.liczbaKrawedzi << " krawedzi\n";
        }

        //2. Wyświetl graf listowo i macierzowo na ekranie
        cout << "\n--- Reprezentacja listowa ---\n";
        obsluga.wyswietlListowo();
        cout << "\n--- Reprezentacja macierzowa ---\n";
        obsluga.wyswietlMacierzowo(true);

        // Przygotowanie macierzy incydencji i wag
        vector<vector<int>> macierz(obsluga.liczbaWierzcholkow, vector<int>(obsluga.liczbaKrawedzi, 0));
        vector<int> wagi(obsluga.liczbaKrawedzi, 0);

        for (int e = 0; e < obsluga.liczbaKrawedzi; ++e) {
            int u = obsluga.krawedzie[e].poczatek;
            int v = obsluga.krawedzie[e].koniec;
            macierz[u][e] = -1;
            macierz[v][e] = 1;
            wagi[e] = obsluga.krawedzie[e].waga;
        }

        // 3. Algorytm Dijkstry wykonywany macierzowo i listowo z wyświetleniem wyników
        int start, koniec;
        start = rand() % obsluga.liczbaWierzcholkow;
        do {
            koniec = rand() % obsluga.liczbaWierzcholkow;
        } while (koniec == start);

        cout << "\nWylosowany wierzcholek startowy do algorytmu Dijkstry: " << start << endl;
        cout << "Wylosowany wierzcholek koncowy do algorytmu Dijkstry: " << koniec << endl;

        // Dijkstra LISTOWO
        auto startDijkstraList = steady_clock::now();
        Dijkstry::znajdzNajkrotszaSciezkeListowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, start, koniec);
        auto endDijkstraList = steady_clock::now();
        auto czasDijkstraList = duration_cast<microseconds>(endDijkstraList - startDijkstraList).count();
        sumaDijkstraList += czasDijkstraList;
        cout << "Czas wykonania Dijkstry (listowo): "
             << czasDijkstraList << " mikrosekund\n \n";

        // Dijkstra MACIERZOWO
        auto startDijkstraMac = steady_clock::now();
        Dijkstry::znajdzNajkrotszaSciezkeMacierzowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow,
                                                    obsluga.liczbaKrawedzi, start, koniec);
        auto endDijkstraMac = steady_clock::now();
        auto czasDijkstraMac = duration_cast<microseconds>(endDijkstraMac - startDijkstraMac).count();
        sumaDijkstraMac += czasDijkstraMac;
        cout << "Czas wykonania Dijkstry (macierzowo): "
             << czasDijkstraMac << " mikrosekund\n";

        // 4. Algorytm Kruskala wykonywany macierzowo i listowo z wyświetleniem wyników
        Kruskal kruskal;

        // Kruskal LISTOWO
        auto startKruskalList = steady_clock::now();
        auto mst_list = kruskal.wykonajKruskal(obsluga.krawedzie, obsluga.liczbaWierzcholkow);
        auto endKruskalList = steady_clock::now();
        auto czasKruskalList = duration_cast<microseconds>(endKruskalList - startKruskalList).count();
        sumaKruskalList += czasKruskalList;
        cout << "\n--- Kruskal LISTOWO ---\n";
        Kruskal::wyswietlMST(mst_list);
        cout << "Czas wykonania Kruskala (listowo): "
             << czasKruskalList << " mikrosekund\n";

        // Kruskal MACIERZOWO
        auto startKruskalMac = steady_clock::now();
        auto mst_macierz = kruskal.wykonajKruskalMacierzowo(macierz, wagi, obsluga.liczbaWierzcholkow,
                                                            obsluga.liczbaKrawedzi);
        auto endKruskalMac = steady_clock::now();
        auto czasKruskalMac = duration_cast<microseconds>(endKruskalMac - startKruskalMac).count();
        sumaKruskalMac += czasKruskalMac;
        cout << "\n--- Kruskal MACIERZOWO ---\n";
        Kruskal::wyswietlMST(mst_macierz);
        cout << "Czas wykonania Kruskala (macierzowo): "
             << czasKruskalMac << " mikrosekund\n";
    }

    double srDijkstraList = (double)sumaDijkstraList / liczba_iteracji;
    double srDijkstraMac = (double)sumaDijkstraMac / liczba_iteracji;
    double srKruskalList = (double)sumaKruskalList / liczba_iteracji;
    double srKruskalMac = (double)sumaKruskalMac / liczba_iteracji;

    // Zapis do pliku
    if (zapis.otworz()) {
        zapis.zapiszSrednieCzasy(liczba_iteracji, n, gestosc,
                                 srDijkstraList, srDijkstraMac,
                                 srKruskalList, srKruskalMac);
        zapis.zamknij();
    } else {
        cout << "Nie udalo sie otworzyc pliku output.txt do zapisu wynikow.\n";
    }

    return 0;
}
