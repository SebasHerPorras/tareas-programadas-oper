#ifndef METRICAS_HPP
#define METRICAS_HPP

#include <vector>

#include <iostream>

struct Metricas {
  std::vector<double> throughput_por_proceso;
  double throughput_promedio;
  std::vector<double> turnaround_por_proceso;
  double turnaround_promedio;
  std::vector<double> waiting_por_proceso;
  double waiting_promedio;
};

void imprimir_metricas(const Metricas& m);

#endif // METRICAS_HPP