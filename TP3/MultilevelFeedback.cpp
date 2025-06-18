#include "MultilevelFeedback.hpp"
#include <climits>
Metricas MLFQ(std::vector<Proceso>& procesos) {
  int cantidad_procesos = procesos.size();
  std::queue<int> cola_nivel1;
  std::queue<int> cola_nivel2;
  std::queue<int> cola_nivel3;
  std::vector<int> remaining_time(cantidad_procesos);
  std::vector<int> waiting(cantidad_procesos, 0);
  std::vector<int> turnaround(cantidad_procesos, 0);
  std::vector<bool> en_cola(cantidad_procesos, false);
  std::vector<bool> terminado(cantidad_procesos, false);

  for (int i = 0; i < cantidad_procesos; ++i) {
    remaining_time[i] = procesos[i].burst_time;
  }
  int tiempo_actual = 0;
  int procesos_terminados = 0;
  while (procesos_terminados < cantidad_procesos) {
    for (int i = 0; i < cantidad_procesos; ++i) {
      if(!en_cola[i] && procesos[i].llegada <= tiempo_actual) {
        cola_nivel1.push(i);
        en_cola[i] = true;
        std::cout << "Proceso: " << procesos[i].id << " ha sido agregado a la cola nivel 1, en el tiempo: " << tiempo_actual << std::endl;
      }
    }
    int indice = -1;
    int nivel = 0;
    int quantumnivel1, quantumnivel2 = 0;
    if (!cola_nivel1.empty()) {
      nivel = 1;
      quantumnivel1 = 3;
      indice = cola_nivel1.front();
      cola_nivel1.pop();
    } else if (!cola_nivel2.empty()) {
      nivel = 2;
      quantumnivel2 = 5;
      indice = cola_nivel2.front();
      cola_nivel2.pop();
    } else if(!cola_nivel3.empty()) {
      nivel = 3;
      indice = cola_nivel3.front();
      cola_nivel3.pop();
    } else {
      int siguiente_llegada = INT_MAX;
      for (int i = 0; i < cantidad_procesos; ++i) {
        if (!en_cola[i]) {
          siguiente_llegada = std::min(siguiente_llegada, procesos[i].llegada);
        }
        tiempo_actual = siguiente_llegada;
        continue;
      }
    }
    int tiempo_ejecutandose = 0;
    if (nivel ==1) {
      tiempo_ejecutandose = std::min(quantumnivel1, remaining_time[indice]);
    } else if (nivel == 2) {
      tiempo_ejecutandose = std::min(quantumnivel2, remaining_time[indice]);
    } else if (nivel == 3) {
      tiempo_ejecutandose = remaining_time[indice];
    }
    std::cout << "Proceso: " << procesos[indice].id << " se ejecuta en el nivel: " << nivel << " en el tiempo: " << tiempo_actual << std::endl;
    if (waiting[indice] == 0 && tiempo_actual > procesos[indice].llegada) {
      waiting[indice] = tiempo_actual - procesos[indice].llegada;
    }
    tiempo_actual += tiempo_ejecutandose;
    remaining_time[indice] -= tiempo_ejecutandose;

    // Agregar procesos que llegan durante la ejecución
    for (int i = 0; i < cantidad_procesos; ++i) {
      if (!en_cola[i] && procesos[i].llegada <= tiempo_actual) {
        cola_nivel1.push(i);
        en_cola[i] = true;
        std::cout << "Tiempo " << tiempo_actual << ": Proceso P" << (i+1) << " entra a nivel 1" << std::endl;
      }
    }

    // Decidir si el proceso termina o baja de nivel
    if (remaining_time[indice] == 0) {
      turnaround[indice] = tiempo_actual - procesos[indice].llegada;
      terminado[indice] = true;
      procesos_terminados++;
      std::cout << "Tiempo " << tiempo_actual << ": P" << (indice+1) << " termina en nivel " << nivel << std::endl;;
    } else {
      if (nivel == 1) {
        cola_nivel2.push(indice);
        std::cout << "Tiempo " << tiempo_actual<< ": P" << (indice+1) << " baja a nivel 2" << std::endl;
      } else if (nivel == 2) {
        cola_nivel3.push(indice);
        std::cout << "Tiempo " << tiempo_actual << ": P" << (indice+1) << " baja a nivel 3" << std::endl;
      } else {
        // FCFS, se queda en nivel 3 siempre
        cola_nivel3.push(indice);
      }
    }
  } // fin while
  Metricas metricas;
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