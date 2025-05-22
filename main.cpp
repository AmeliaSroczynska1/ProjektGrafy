#include <iostream>
#include "ZapisDoPliku.h"
#include "GenerowanieGrafu.h"
#include "Dijkstry.h"

using namespace std;

ZapisDoPliku zapis("output.txt");
GenerowanieGrafu obsluga;

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

    // DODANE MENU WYŚWIETLANIA
    int pokaz;
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
        if(wybor == 1) {
            obsluga.wyswietlMacierzowo(true);
        } else if(wybor == 0) {
            obsluga.wyswietlMacierzowo(false);
        }
        else {
            cout << "Nieprawidlowy wybor.\n";
        }
    } else if (pokaz == 3) {
        cout << "--- Reprezentacja listowa ---\n";
        obsluga.wyswietlListowo();
        cout << "\n--- Reprezentacja macierzowa ---\n";
        cout << "Czy graf jest skierowany? (1 - tak, 0 - nie): ";
        cin >> wybor;
        if(wybor == 1) {
            obsluga.wyswietlMacierzowo(true);
        } else if(wybor == 0) {
            obsluga.wyswietlMacierzowo(false);
        }
        else {
            cout << "Nieprawidlowy wybor.\n";
        }
    } else {
        cout << "Nieprawidlowy wybor.\n";
    }

    int start, koniec;
    cout << "Podaj wierzcholek startowy: ";
    cin >> start;
    cout << "Podaj wierzcholek koncowy: ";
    cin >> koniec;

    Dijkstry::znajdzNajkrotszaSciezkeListowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, start, koniec);
    Dijkstry::znajdzNajkrotszaSciezkeMacierzowo(obsluga.krawedzie, obsluga.liczbaWierzcholkow, obsluga.liczbaKrawedzi, start, koniec);


    return 0;
}
