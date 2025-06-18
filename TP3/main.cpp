#include "FirstComeFirstServe.hpp"
#include "ShortestJob.hpp"
#include "RoundRobin.hpp"
#include "Priority.hpp"
#include "convertirEntrada.hpp"
#include "Proceso.hpp"
#include "includes.hpp"

int main(int argc, char* argv[]) {
  std::string archivo = (argc > 1) ? argv[1] : "prueba_1.txt";
  std::ifstream file("tests/" + archivo);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
    return 1;
  }

  std::string entrada;
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

      // Ejecutar el algoritmo correspondiente
      if (algoritmo == "FCFS" || algoritmo == "FirstComeFirstServed") {
        int fcfs = FCFS(procesos);
      } else if (algoritmo == "SJF" || algoritmo == "ShortestJobFirst") {
        int sjf = SJF(procesos);
      } else if (algoritmo == "RoundRobin") {
        int rr = RoundRobin(procesos, 2);
      } else if (algoritmo == "Priority") {
        int prio = Priority(procesos);
      } else if (algoritmo == "all") {
        int fcfs = FCFS(procesos);
        int sjf = SJF(procesos);
        int rr = RoundRobin(procesos, 2);
        int prio = Priority(procesos);
      } else {
        std::cerr << "Algoritmo no reconocido: " << algoritmo << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "Error procesando la entrada: " << e.what() << std::endl;
    }
  }
  return 0;
}