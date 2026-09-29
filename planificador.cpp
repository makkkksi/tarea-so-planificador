#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// funcion parecortar
string recortar(string s) {
    size_t ini = s.find_first_not_of(" ");
    if (ini == string::npos) return "";   // la linea era puros espacios (o vacia)
    size_t fin = s.find_last_not_of(" ");
    return s.substr(ini, fin - ini + 1);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "uso: " << argv[0] << " plan.txt K" << endl;
    return 1;
    }

            ifstream archivo(argv[1]);
        if (!archivo) {
                cout << "no se pudo abrir " << argv[1] << endl;
            return 1;
        }

    string linea;
        while (getline(archivo, linea)) {
                linea = recortar(linea);
                if (linea == "") continue;   // linea vacia, se salta
        size_t p1 = linea.find(':');           // primer ':'
        size_t p2 = linea.find(':', p1 + 1);   // segundo, ybuscando despues del primero
        size_t p3 = linea.find(':', p2 + 1);   // terceroo

                if (p1 == string::npos || p2 == string::npos || p3 == string::npos) {
            cout << "linea mal escrota: " << linea << endl;
            return 1;
                }

        string id     = recortar(linea.substr(0, p1));
        string nombre = recortar(linea.substr(p1 + 1, p2 - p1 - 1));
        string tiempo = recortar(linea.substr(p2 + 1, p3 - p2 - 1));
        string deps   = recortar(linea.substr(p3 + 1));

        cout << "id=[" << id << "] nombre=[" << nombre
             << "] tiempo=[" << tiempo << "] deps=[" << deps << "]" << endl; //impresion
    }
    return 0;
}
