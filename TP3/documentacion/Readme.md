# Simulación de algoritmos de planificación de procesos

## Descripción

Para esta tarea implementamos en C++ la simulación de distintos **algoritmos de planificación de procesos**, vistos en el capítulo 5 del libro de texto. El objetivo fue comparar estos algoritmos bajo las mismas condiciones de entrada, evaluando su desempeño con base en métricas clave como **Throughput**, **Turnaround time** y **Waiting time**.  

El programa recibe por parámetro la información de varios procesos (al menos dos), incluyendo:

- Tiempo de llegada (si aplica)
- Tiempo de ráfaga o "burst time"
- Prioridad (si aplica)
- Algoritmo a analizar

Una vez ingresados estos datos, el programa simula la planificación de los procesos aplicando el algoritmo correspondiente, y genera estadísticas sobre su rendimiento.

## Algoritmos que debíamos implementar

- **First Come First Served (FCFS)**
- **Shortest Job First (SJF)**
- **Round Robin (Quantum = 2)**
- **Priority Scheduling**

## Compilación y ejecución

Desde la carpeta TP3:

```sh
make
./bin/Sistemas prueba_3.txt
Puede probar con distintos archivos de prueba
```

Créditos
Fabricio Agüero Mata - C20097
Sebastián Hernández Porras - C23770

# Anexo de Imágenes Adicionales

## Fallos de ejecución
![Fallos durante ejecución](./imagenes/caso_con_fallos.png)
## Ejecución General
![Ejecución General](./imagenes/caso_generico.png)
## Compilación funconal
![Compilación Correcta](./imagenes/correcta_compilacion.png)
## Uso de tests espeficicos
![Prueba específica de MLFQ](./imagenes/test_en_especifico.png)
## MLFQ
![Planificador MLFQ](./imagenes/MLFQ.png)

