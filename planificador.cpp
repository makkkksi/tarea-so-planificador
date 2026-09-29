#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <sstream>
#include <queue>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

//agrupador por variables
struct Actividad {
    string id;            
    string nombre;        
    int tiempo;            
    string deps_texto;     
    vector<int> deps;      
    vector<int> hijos;     
};
// funcion parecortar
string recortar(string s) {
        size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == string::npos) return "";   // la linea era puros espacios (o vacia)
       size_t fin = s.find_last_not_of(" \t\r\n"); //ooh q me costover el error
    return s.substr(ini, fin - ini + 1);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "uso: " << argv[0] << " plan.txt K" << endl;
    return 1;
    }
            srand(time(NULL));   // variador de semilla
                int K = atoi(argv[2]);
            if (K < 1) {
                cout << "K tiene que ser mayor a 0" << endl;
                return 1;
            }
            ifstream archivo(argv[1]);
        if (!archivo) {
                cout << "no se pudo abrir " << argv[1] << endl;
            return 1;
        }

    string linea;     vector<Actividad> actividades;
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


        Actividad a;
        a.id = id;
        a.nombre = nombre;
        a.deps_texto = deps;

        if (tiempo == "") {
            a.tiempo = 100 + rand() % 4901;   // aleatorio entre 100 y 5000
        } else {
            a.tiempo = stoi(tiempo);          // texto a numero
        }

        actividades.push_back(a);
        
    }

                // map: dado un id, dice en que posicion del vector esta
    map<string, int> posicion;
    for (int i = 0; i < (int)actividades.size(); i++) {
        if (posicion.count(actividades[i].id) > 0) {
            cout << "id repetido: " << actividades[i].id << endl;
            return 1;
        }
        posicion[actividades[i].id] = i;
    }

        for (int i = 0; i < (int)actividades.size(); i++) {
        stringstream ss(actividades[i].deps_texto);
        string dep;
        // getline con ',' corta el texto en cada coma
        while (getline(ss, dep, ',')) {
            dep = recortar(dep);
            if (dep == "") continue;

            if (posicion.count(dep) == 0) {
                cout << actividades[i].id << " depende de " << dep << " que no existe" << endl;
                return 1;
            }
            int j = posicion[dep];
            actividades[i].deps.push_back(j);    // i depende de j
            actividades[j].hijos.push_back(i);   // j tiene como hijo a i o algo asi
        }
    }
    
    // revisar que no haya ciclos (algoritmo de kahn)
    vector<int> faltan(actividades.size());   // cuantas deps le faltan a cada una
    queue<int> cola;                          // las que ya pueden "partir"

    for (int i = 0; i < (int)actividades.size(); i++) {
        faltan[i] = actividades[i].deps.size();
        if (faltan[i] == 0) cola.push(i);
    }

    int visitadas = 0;
    while (!cola.empty()) {
        int x = cola.front();
        cola.pop();
        visitadas++;
        // x "termino", a sus hijos les falta una dependencia menos
        for (int h : actividades[x].hijos) {
            faltan[h]--;
            if (faltan[h] == 0) cola.push(h);
        }
    }

    if (visitadas < (int)actividades.size()) {
        cout << "el plan tiene un ciclo, no se puede ejecutar" << endl;
        return 1;
    }

             // ahora si, a ejecutar
    for (int i = 0; i < (int)actividades.size(); i++) {
        faltan[i] = actividades[i].deps.size();
        if (faltan[i] == 0) cola.push(i);
    }

    map<int, int> vivos;   // pid -> actividad
    int hechas = 0;

    while (hechas < (int)actividades.size()) {
        while ((int)vivos.size() < K && !cola.empty()) {
            int i = cola.front();
            cola.pop();

            int pid = fork();
            if (pid < 0) {
                cout << "error en fork" << endl;
                return 1;
            }
            if (pid == 0) {
                usleep(actividades[i].tiempo * 1000);
                exit(0);
            }
            vivos[pid] = i;
            cout << "empieza " << actividades[i].nombre << " (" << vivos.size() << " corriendo)" << endl;
        }

        int status;
        int pid = waitpid(-1, &status, 0);
        int i = vivos[pid];
        vivos.erase(pid);
        hechas++;
        cout << "termina " << actividades[i].nombre << endl;

        for (int h : actividades[i].hijos) {
            faltan[h]--;
            if (faltan[h] == 0) cola.push(h);
        }
    }
    cout << "fin" << endl;
    return 0;
}