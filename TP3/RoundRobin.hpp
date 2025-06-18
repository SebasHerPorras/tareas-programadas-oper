#ifndef ROUND_ROBIN_HPP
#define ROUND_ROBIN_HPP

#include "includes.hpp"


Metricas RoundRobin(std::vector<Proceso>& procesos, int quantum);
std::vector<int> ordenar_por_indice(std::vector<Proceso>& procesos, int cantidad_procesos);
void agregar_procesos(std::queue<int>& cola_procesos, std::vector<Proceso>& procesos,
   int& indice, int tiempo_actual, std::vector<int>& procesos_ordenados, int cantidad_procesos);
#endif // ROUNDROBIN_HPP