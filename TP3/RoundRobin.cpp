#include "RoundRobin.hpp"

Metricas RoundRobin(std::vector<Proceso>& procesos, int quantum){
  int cantidad_procesos = procesos.size();
  // Vector para el tiempo que queda le queda a cada proceso
  std::vector<int> remaining_time(cantidad_procesos);
  // Para ver si el proceso ya terminó
  std::vector<bool> terminado(cantidad_procesos, false);
  // Una cola para ir poniendo procesos
  std::queue<int> cola_procesos;
  // Se guarda la info para  luego imprimir
  Metricas metricas;
  metricas.turnaround_por_proceso.resize(cantidad_procesos);
  metricas.waiting_por_proceso.resize(cantidad_procesos);
  metricas.throughput_por_proceso.resize(cantidad_procesos);
  // Ordeno los procesos por tiempo de llegada pero por índice
  std::vector<int> procesos_ordenados(cantidad_procesos);
  procesos_ordenados = ordenar_por_indice(procesos, cantidad_procesos);
  for (int i = 0; i < cantidad_procesos; ++i) {
    remaining_time[i] = procesos[i].burst_time;
  }
  int tiempo_actual = 0;
  int procesos_terminados = 0;
  int indice = 0;
  while(procesos_terminados < cantidad_procesos) {
    agregar_procesos(cola_procesos, procesos, indice, tiempo_actual, procesos_ordenados, cantidad_procesos);
    // Simula el paso del tiempo en el que no hay llegado un proceso
    if (cola_procesos.empty()) {
      if (indice < cantidad_procesos) {
        tiempo_actual = procesos[procesos_ordenados[indice]].llegada;
      }
      continue;
    }
    // Se simula la ejecución de un proceoso
    if (!cola_procesos.empty()) {
      int proceso_actual = cola_procesos.front();
      cola_procesos.pop();
      int tiempo_ejecutandose = std::min(quantum, remaining_time[proceso_actual]);
      tiempo_actual += tiempo_ejecutandose;
      remaining_time[proceso_actual] -= tiempo_ejecutandose;
      agregar_procesos(cola_procesos, procesos, indice, tiempo_actual, procesos_ordenados, cantidad_procesos);
      if (remaining_time[proceso_actual] > 0) {
        cola_procesos.push(proceso_actual);
      } else if (!terminado[proceso_actual]) {
        terminado[proceso_actual] = true;
        procesos_terminados++;
        int turnaround = tiempo_actual - procesos[proceso_actual].llegada;
        int waiting = turnaround - procesos[proceso_actual].burst_time;
        metricas.turnaround_por_proceso[proceso_actual] = turnaround;
        metricas.waiting_por_proceso[proceso_actual] = waiting;
      }
    }
  }
  // Calcular throughput por proceso
  for (int i = 0; i < cantidad_procesos; ++i) {
    if (metricas.turnaround_por_proceso[i] > 0) {
        metricas.throughput_por_proceso[i] = 1.0 / metricas.turnaround_por_proceso[i];
    } else {
        metricas.throughput_por_proceso[i] = 0; 
    }
  }
  // Calcular promedios para devolver las métricas
  double suma_turnaround = 0;
  double suma_waiting = 0;
  for (int i = 0; i < cantidad_procesos; ++i) {
    suma_turnaround += metricas.turnaround_por_proceso[i];
    suma_waiting += metricas.waiting_por_proceso[i];
  }
  metricas.turnaround_promedio = suma_turnaround / cantidad_procesos;
  metricas.waiting_promedio = suma_waiting / cantidad_procesos;
  metricas.throughput_promedio = static_cast<double>(cantidad_procesos) / tiempo_actual;
  // Retornar el struct con todos los resultados
  return metricas;
}

/*
  Funnción para ordenar los procesos por tiempo de llegada
  pero mapenadolo por el indice
*/
std::vector<int> ordenar_por_indice(std::vector<Proceso>& procesos, int cantidad_procesos) {
  std::vector<int> procesos_ordenados(cantidad_procesos);
  for (int i = 0; i < cantidad_procesos; ++i) {
    procesos_ordenados[i] = i;
  }
  std::sort(procesos_ordenados.begin(), procesos_ordenados.end(), [&](int a, int b) {
    return procesos[a].llegada < procesos[b].llegada;
  });
  return procesos_ordenados;
}

/*
  Funnción para agregar los procesos que ya llegaron a la cola de procesos
  y que no se han agregado aún. Se usa un índice para saber qué proceso
  es el siguiente en llegar.
*/
void agregar_procesos(std::queue<int>& cola_procesos, std::vector<Proceso>& procesos,
   int& indice, int tiempo_actual, std::vector<int>& procesos_ordenados, int cantidad_procesos) {
  // Agrega los procesos que ya llegaron a la cola de procesos
  while (indice <  cantidad_procesos && procesos[procesos_ordenados[indice]].llegada <= tiempo_actual) {
    cola_procesos.push(procesos_ordenados[indice]);
    indice++;
  }}


