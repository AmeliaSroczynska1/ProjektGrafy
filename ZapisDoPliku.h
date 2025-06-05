#define ZAPISDOPLIKU_H


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // dla rand()
#include <ctime>    // dla time()
#include <utility>
#include <iomanip>

using namespace std;

class ZapisDoPliku {
private:
    ofstream plik;
    string nazwaPliku;

public:
    // Konstruktor
    ZapisDoPliku(string  nazwa) : nazwaPliku(std::move(nazwa)) {}

    // Tworzenie i otwieranie pliku do zapisu
    bool otworz() {
        plik.open(nazwaPliku, ios::out | ios::app);
        ;
        if (!plik.is_open()) {
            cout << "Nie udalo sie otworzyc pliku do zapisu :((\n";
            return false;
        }
        return true;
    }


    // Zamknięcie pliku
    void zamknij() {
        if (plik.is_open()) {
            plik.close();
        }
    }

    ~ZapisDoPliku() {
        zamknij();
    }

    void zapiszSrednieCzasy(int liczbaIteracji, int n, int gestosc,
                            double srDijkstraList, double srDijkstraMac,
                            double srKruskalList, double srKruskalMac,
                            double srPrimaList, double srPrimaMac,
                            double srBellmanList, double srBellmanMac) {
        if (plik.is_open()) {
            plik << fixed << setprecision(4);
            plik << "\n \n======= SREDNIE CZASY WYKONANIA ALGORYTMOW (" << liczbaIteracji << " iteracji, "
                 << n << " wierzcholkow, " << gestosc << "% gestosci) =======\n";
            plik << "Sredni czas wykonywania algorytmu Dijkstry (listowo):           " << srDijkstraList*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Dijkstry (macierzowo):        " << srDijkstraMac*0.001<< " ms\n";
            plik << "Sredni czas wykonywania algorytmu Bellmana-Forda (listowo):     " << srBellmanList*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Bellmana-Forda (macierzowo):  " << srBellmanMac*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Kruskala (listowo):           " << srKruskalList*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Kruskala (macierzowo):        " << srKruskalMac*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Prima (listowo):              " << srPrimaList*0.001 << " ms\n";
            plik << "Sredni czas wykonywania algorytmu Prima (macierzowo):           " << srPrimaMac*0.001 << " ms\n";
        } else {
            cout << "Plik nie jest otwarty :((\n";
        }
    }


};