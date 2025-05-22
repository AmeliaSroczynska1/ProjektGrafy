#include <iostream>
#include "ZapisDoPliku.h"

using namespace std;

ZapisDoPliku zapis("output.txt");

int main() {

    // Zapis do pliku
    if (zapis.otworz()) {
        zapis.zapisWynikow();
        zapis.zamknij();
    }

    return 0;
}
