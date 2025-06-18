#include <climits>

#include "Priority.hpp"
#include "includes.hpp"

Metricas Priority(std::vector<Proceso>& procesos) {
  int cantidad_procesos = procesos.size();
  std::vector<bool> terminado(cantidad_procesos, false);
  std::vector<int> waiting(cantidad_procesos, 0);
  std::vector<int> turnaround(cantidad_procesos, 0);
  int tiempo_actual = 0;
  int completados = 0;

  while (completados < cantidad_procesos) {
    // Buscar el proceso listo con mayor prioridad (menor número)
    int indice = -1;
    int mejor_prio = INT_MAX;
    int llegada_min = INT_MAX;
    for (int i = 0; i < cantidad_procesos; ++i) {
      if (!terminado[i] && procesos[i].llegada <= tiempo_actual) {
        if (procesos[i].prioridad < mejor_prio ||
            (procesos[i].prioridad == mejor_prio && procesos[i].llegada < llegada_min)) {
          mejor_prio = procesos[i].prioridad;
          llegada_min = procesos[i].llegada;
          indice = i;
        }
      }
    }

    if (indice == -1) {
      // No hay procesos listos, avanza el tiempo_actual al siguiente proceso que llegue
      int siguiente_llegada = INT_MAX;
      for (int i = 0; i < cantidad_procesos; ++i) {
        if (!terminado[i]) {
          siguiente_llegada = std::min(siguiente_llegada, procesos[i].llegada);
        }  
      }    
      tiempo_actual = siguiente_llegada;
      continue;
    }

    waiting[indice] = tiempo_actual - procesos[indice].llegada;
    tiempo_actual += procesos[indice].burst_time;
    turnaround[indice] = tiempo_actual - procesos[indice].llegada;
    terminado[indice] = true;
    completados++;
  }

  // Calcular métricas
  Metricas metricas;
  // Conversión de int a double para los vectores del struct
  metricas.waiting_por_proceso.assign(waiting.begin(), waiting.end());
  metricas.turnaround_por_proceso.assign(turnaround.begin(), turnaround.end());
  metricas.throughput_por_proceso.resize(cantidad_procesos);
  double suma_waiting = 0, suma_turnaround = 0;
  for (int i = 0; i < cantidad_procesos; ++i) {
    suma_waiting += waiting[i];
    suma_turnaround += turnaround[i];
    metricas.throughput_por_proceso[i] = 1.0 / turnaround[i];
  }
  metricas.waiting_promedio = suma_waiting / cantidad_procesos;
  metricas.turnaround_promedio = suma_turnaround / cantidad_procesos;
  metricas.throughput_promedio = cantidad_procesos / (double)tiempo_actual;
  return metricas;
}