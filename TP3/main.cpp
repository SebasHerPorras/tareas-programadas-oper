#include "convertirEntrada.hpp"
#include "Proceso.hpp"
#include "includes.hpp"
#include "FirstComeFirstServe.hpp"
#include "ShortestJob.hpp"
#include "RoundRobin.hpp"
#include "Priority.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct ResultadoAlgoritmo {
  std::string nombreAlgoritmo;
  Metricas metricas;
};

int main(int argc, char* argv[]) {
  std::string archivo = (argc > 1) ? argv[1] : "prueba_1.txt";
  std::ifstream file("tests/" + archivo);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
    return 1;
  }

  std::string entrada;
  std::vector<ResultadoAlgoritmo> metricas_por_algoritmo;
  while (std::getline(file, entrada)) {
    try {
      std::vector<std::string> partes = dividirEntrada(entrada, '|');
      if (partes.size() < 5) throw std::runtime_error("Formato de entrada incorrecto");

      int n = std::stoi(partes[0]);
      std::vector<int> burst = procesarLista(partes[1]);
      std::vector<int> prioridad = procesarLista(partes[2]);
      std::vector<int> llegada = procesarLista(partes[3]);
      std::string algoritmo = partes[4];

      if (prioridad.size() == 1 && prioridad[0] == 0) prioridad = std::vector<int>(n, 1);
      if (llegada.size() == 1 && llegada[0] == 0) llegada = std::vector<int>(n, 0);

      std::vector<Proceso> procesos;
      for (int i = 0; i < n; ++i) {
        Proceso p;
        p.id = i + 1;
        p.burst_time = burst[i];
        p.prioridad = prioridad[i];
        p.llegada = llegada[i];
        procesos.push_back(p);
      }

      std::cout << "\nEntrada: " << entrada << std::endl;
      std::cout << "Algoritmo: " << algoritmo << std::endl;
      std::cout << "Procesos:\n";
      for (const auto& p : procesos) {
        std::cout << "P" << p.id << " Burst: " << p.burst_time
                  << " Prioridad: " << p.prioridad
                  << " Llegada: " << p.llegada << std::endl;
      }

      // // Ejecutar el algoritmo correspondiente
      if (algoritmo == "FCFS" || algoritmo == "FirstComeFirstServed") {
        Metricas fcfs = FCFS(procesos);
        metricas_por_algoritmo.push_back({"First Come First Serve", fcfs});
      } else if (algoritmo == "SJF" || algoritmo == "ShortestJobFirst") {
        Metricas sjf = SJF(procesos);
        metricas_por_algoritmo.push_back({"Shortest Job First", sjf});
      } else if (algoritmo == "RoundRobin") {
        Metricas roundrobin = RoundRobin(procesos, 2);
        metricas_por_algoritmo.push_back({"Round Robin", roundrobin});
      } else if (algoritmo == "Priority") {
        Metricas prio = Priority(procesos);
        metricas_por_algoritmo.push_back({"Priority", prio});
      } else if (algoritmo == "all") {
        Metricas fcfs = FCFS(procesos);
        Metricas sjf = SJF(procesos);
        Metricas rr = RoundRobin(procesos, 2);
        Metricas prio = Priority(procesos);
      } else {
        std::cerr << "Algoritmo no reconocido: " << algoritmo << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "Error procesando la entrada: " << e.what() << std::endl;
    }
  }
  file.close();
  std::cout << "\nProcesamiento completado." << std::endl;
  // Imprimir métricas de todos los algoritmos
  for (const auto& metricas : metricas_por_algoritmo) {
    std::cout << "\nMétricas del algoritmo: " << metricas.nombreAlgoritmo << std::endl;
    imprimir_metricas(metricas.metricas);
    std::cout << std::endl;
  }
  std::cout << "\nFin del programa." << std::endl;


  return 0;
}