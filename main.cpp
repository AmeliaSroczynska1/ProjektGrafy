#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "ZapisDoPliku.h"
#include "GenerowanieGrafu.h"
#include "Dijkstry.h"
#include "Kruskal.h"
#include "Prima.h"
#include "BellmanaFord.h"


using namespace std;
using namespace std::chrono;

ZapisDoPliku zapis("output.txt");
GenerowanieGrafu obsluga;

int main() {
    srand(time(0)); // Inicjalizacja generatora liczb losowych

    int n;
    int gestosc;
    mt19937 rng(static_cast<unsigned>(time(nullptr)));
    long long sumaPrimaList = 0;
    long long sumaPrimaMac = 0;
    long long sumaBellmanList = 0;
    long long sumaBellmanMac = 0;

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

    // Liczba iteracji do wykonania algorytmów
    int liczba_iteracji;
    cout << "Podaj ile razy chcesz powtorzyc pomiary (bedzie obliczona srednia czasu biorac pod uwage kazda iteracje): ";
    cin >> liczba_iteracji;

    for(int i = 0; i < liczba_iteracji; i++) {

        if(wybor == 2){
            obsluga.generujLosowyGraf(n, gestosc, rng);

            cout << "\nWygenerowano graf: " << obsluga.liczbaWierzcholkow << " wierzcholkow, "
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


        cout << "\nChcesz rozwiazac problem najkrotszej sciezki czy wyznaczyc MST?\n"
                "1 - Najkrotsza sciezka \n"
                "2 - MST\n";
        int wybor;
        cin >> wybor;
        while(wybor != 1 && wybor != 2) {
            cout << "Mozesz wybrac tylko 1 lub 2. Sprobuj ponownie: ";
            cin >> wybor;
        }

        if(wybor == 1){
            // 3. Algorytm Dijkstry wykonywany macierzowo i listowo z wyświetleniem wyników
            int start, koniec;
            cout << "\nPodaj wierzcholek startowy: ";
            cin >> start;
            cout << "Podaj wierzcholek koncowy: ";
            cin >> koniec;
            while (koniec == start){
                cout << "Wierzcholek koncowy nie moze byc taki sam jak startowy. Podaj inny wierzcholek koncowy: ";
                cin >> koniec;
            }

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

            // 4. Algorytm Bellmana-Forda wykonywany macierzowo i listowo z wyświetleniem wyników
            // Ford-Bellman LISTOWO
            auto startBellmanList = steady_clock::now();
            BellmanaFord::znajdzNajkrotszaSciezkeListowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, start, koniec);
            auto endBellmanList = steady_clock::now();
            auto czasBellmanList = duration_cast<microseconds>(endBellmanList - startBellmanList).count();
            sumaBellmanList += czasBellmanList;
            cout << "Czas wykonania Bellmana-Forda (listowo): "
                 << czasBellmanList << " mikrosekund\n";

            // Ford-Bellman MACIERZOWO
            auto startBellmanMac = steady_clock::now();
            BellmanaFord::znajdzNajkrotszaSciezkeMacierzowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, obsluga.liczbaKrawedzi, start, koniec);
            auto endBellmanMac = steady_clock::now();
            auto czasBellmanMac = duration_cast<microseconds>(endBellmanMac - startBellmanMac).count();
            sumaBellmanMac += czasBellmanMac;
            cout << "Czas wykonania Bellmana-Forda (macierzowo): "
                 << czasBellmanMac << " mikrosekund\n";
        }
        else if(wybor ==2){
            // 5. Algorytm Prima wykonywany macierzowo i listowo z wyświetleniem wyników
            Prima prima;

            // Prima LISTOWO
            auto startPrimaList = steady_clock::now();
            auto mst_prima_list = prima.wykonajPrimaListowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow);
            auto endPrimaList = steady_clock::now();
            auto czasPrimaList = duration_cast<microseconds>(endPrimaList - startPrimaList).count();
            sumaPrimaList += czasPrimaList;
            cout << "\n--- Prima (listowo) ---\n";
            Prima::wyswietlMST(mst_prima_list);
            cout << "Czas wykonania Prima (listowo): "
                 << czasPrimaList << " mikrosekund\n";

            // Prima MACIERZOWO
            auto startPrimaMac = steady_clock::now();
            auto mst_prima_mac = prima.wykonajPrimaMacierzowo(macierz, wagi, obsluga.liczbaWierzcholkow, obsluga.liczbaKrawedzi);
            auto endPrimaMac = steady_clock::now();
            auto czasPrimaMac = duration_cast<microseconds>(endPrimaMac - startPrimaMac).count();
            sumaPrimaMac += czasPrimaMac;
            cout << "\n--- Prima (macierzowo) ---\n";
            Prima::wyswietlMST(mst_prima_mac);
            cout << "Czas wykonania Prima (macierzowo): "
                 << czasPrimaMac << " mikrosekund\n";

            // 6. Algorytm Kruskala wykonywany macierzowo i listowo z wyświetleniem wyników
            Kruskal kruskal;

            // Kruskal LISTOWO
            auto startKruskalList = steady_clock::now();
            auto mst_list = kruskal.wykonajKruskal(obsluga.krawedzie, obsluga.liczbaWierzcholkow);
            auto endKruskalList = steady_clock::now();
            auto czasKruskalList = duration_cast<microseconds>(endKruskalList - startKruskalList).count();
            sumaKruskalList += czasKruskalList;
            cout << "\n--- Kruskal (listowo) ---\n";
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
            cout << "\n--- Kruskal (macierzowo) ---\n";
            Kruskal::wyswietlMST(mst_macierz);
            cout << "Czas wykonania Kruskala (macierzowo): "
                 << czasKruskalMac << " mikrosekund\n";
        }
    }

    double srBellmanList = (double)sumaBellmanList / liczba_iteracji;
    double srBellmanMac = (double)sumaBellmanMac / liczba_iteracji;
    double srPrimaList = (double)sumaPrimaList / liczba_iteracji;
    double srPrimaMac = (double)sumaPrimaMac / liczba_iteracji;
    double srDijkstraList = (double)sumaDijkstraList / liczba_iteracji;
    double srDijkstraMac = (double)sumaDijkstraMac / liczba_iteracji;
    double srKruskalList = (double)sumaKruskalList / liczba_iteracji;
    double srKruskalMac = (double)sumaKruskalMac / liczba_iteracji;

    // Zapis do pliku
    if (zapis.otworz()) {
        zapis.zapiszSrednieCzasy(liczba_iteracji, n, gestosc,
                                 srDijkstraList, srDijkstraMac,
                                 srKruskalList, srKruskalMac,
                                 srPrimaList, srPrimaMac,
                                 srBellmanList, srBellmanMac);
        zapis.zamknij();
        cout << "\nWyniki zapisano do pliku output.txt\n";
    } else {
        cout << "Nie udalo sie otworzyc pliku output.txt do zapisu wynikow.\n";
    }

    return 0;
}
