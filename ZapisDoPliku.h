//
// Created by ameli on 22.05.2025.
//

#define ZAPISDOPLIKU_H


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // dla rand()
#include <ctime>    // dla time()
#include <utility>

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
        plik.open(nazwaPliku, ios::out);
        if (!plik.is_open()) {
            cout << "Nie udalo sie otworzyc pliku do zapisu :((\n";
            return false;
        }
        return true;
    }

    // Zapis losowej liczby do pliku
    void zapisWynikow() {
        if (plik.is_open()) {

        } else {
            cout << "Plik nie jest otwarty :((\n";
        }
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
};