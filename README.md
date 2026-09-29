# Tarea 1 SO - Planificador

Integrantes: (maximiliano santibañez 21.669.586-0)

## Compilar
make

## Ejecutar
./planificador plan.txt K
./planificador plan.txt K [prob_falla]

## Avance
- lee plan.text y separa cada linea en id nombre tiempo y dependencias.
- se salta lineas vacias y avisa si una linea esta mal escrita.

- guarda las actividades en un vector si no trae tiempo se le asigna uno aleatorio entre 100 y 5000 ms.
- arma el grafo
- detecta ciclos antes de ejecutar
- recortar tambien quita los /r.
- cada actividad corre en un proceso hijo, el hijo duerme su tiempo con usleep. el padre espera con waitpid y nunca hay mas de K procesos al mismo tiempo.
- fallas: con un tercer argumento opcional 0 a 100
- Con Ctrl+C se aborta todo.
## Decisiones de diseño
- limite k : el padre guarda los hijos vivos en un map, solo hace fork si hay menos k vivos
- el padre espera con waitpid, que lo deja dormido hasta que termine algun hijo. No gasta cpu esperando.
- los mensajes pasan por el padre y no directo entre hijos
- los mensajes son de 128 bytes fijos, asi el hijo sabe cuanto leer por mensaje
- despues de escribir los mensajes el padre cierra su extremo del pipe.
- Ctrl+C el handler solo pone una variable en 1 cuando el main ve la variable mata a los hijos que quedan con SIGTERM y los espera con wait para que no queden zombies.
- recortar tambien quita \r porque los archivos hechos en Windows lo traen al final de cada linea y rompia la busqueda de las deps.

## Pruebas

- plan.txt:
 ciclo.txt
 ./planificador plan.txt 3 40
  estres con 10000 actividades, generado con
  for i in $(seq 1 10000); do if [ $i -eq 1 ]; then echo "1 : t1 : 5 :"; else echo "$i : t$i : 5 : $((RANDOM % (i-1) + 1))"; fi; done > grande.txt

- /planificador grande.txt 50
