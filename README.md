# Tarea 1 SO - Planificador

Integrantes: (maximiliano santibañez 21.669.586-0)

## Compilar
make

## Ejecutar
./planificador plan.txt K

## Avance
- lee plan.text y separa cada linea en id nombre tiempo y dependencias.
- se salta lineas vacias y avisa si una linea esta mal escrita.

- guarda las actividades en un vector si no trae tiempo se le asigna uno aleatorio entre 100 y 5000 ms.
- arma el grafo
- detecta ciclos antes de ejecutar
- recortar tambien quita los /r.
- cada actividad corre en un proceso hijo, el hijo duerme su tiempo con usleep. el padre espera con waitpid y nunca hay mas de K procesos al mismo tiempo.