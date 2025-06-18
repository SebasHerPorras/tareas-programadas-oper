#include "Metricas.hpp"
#include <iostream>

void imprimir_metricas(const Metricas& m) {
  std::cout << "\nThroughput por proceso: ";
  for (const auto& t : m.throughput_por_proceso) std::cout << t << " ";
  std::cout << "\nThroughput promedio: " << m.throughput_promedio << std::endl;

  std::cout << "\nTurnaround por proceso: ";
  for (const auto& t : m.turnaround_por_proceso) std::cout << t << " ";
  std::cout << "\nTurnaround promedio: " << m.turnaround_promedio << std::endl;

  std::cout << "\nWaiting por proceso: ";
  for (const auto& w : m.waiting_por_proceso) std::cout << w << " ";
  std::cout << "\nWaiting promedio: " << m.waiting_promedio << std::endl;
}
