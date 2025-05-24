#include "convertirEntrada.hpp"
#include "FCFS.hpp"
#include "SSFT.hpp"
#include "CSCAN.hpp"
#include "LOOK.hpp"
#include "CLOOK.hpp"
#include "SCAN.hpp"

#include <fstream>

void castArguments(int argc, char* argv[]);

std::string archivo;
int main(int argc, char* argv[]) {

  castArguments(argc, argv);

  std::ifstream file("tests/" + archivo);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
    return 1;
  }

  std::string entrada;
  while (std::getline(file, entrada)) {
    
    // Procesar cada línea del archivo
    std::cout << "\n" << entrada << std::endl; // Imprime la línea leída
    auto [solicitudes, algoritmo, posCabeza, dirCabeza] = procesarEntrada(entrada);

    std::cout << "Solicitudes: ";
    for (int s : solicitudes) {
      std::cout << s << " ";
    }
    std::cout << "\nAlgoritmo: " << algoritmo << "\nPosición de la cabeza: " << posCabeza << "\nDirección de la cabeza: " << dirCabeza << std::endl;

    // Usar un switch para llamar directamente a los algoritmos por su nombre
    if (algoritmo == "FCFS") {
      FCFS(solicitudes, posCabeza);
    } else if (algoritmo == "SSTF") {
      SSFT(solicitudes, posCabeza);
    } else if (algoritmo == "SCAN") {
      SCAN(solicitudes, posCabeza, dirCabeza);
    } else if (algoritmo == "CSCAN") {
      CSCAN(solicitudes, posCabeza, dirCabeza);
    } else if (algoritmo == "CLOOK") {
      CLOOK(solicitudes, posCabeza, dirCabeza);
    } else if (algoritmo == "LOOK") {
      LOOK(solicitudes, posCabeza, dirCabeza);
    } else {
      std::cerr << "Error: Algoritmo desconocido " << algoritmo << std::endl;
    }
    std::cout << std::endl;
  }
  return 0;

  
}

void castArguments(int argc, char* argv[]) {
  if (argc > 1) {
    // acá debe de venir el nombre del archivo de prueba
    archivo = argv[1];
  } else {
   archivo = "prueba_1.txt";
  }
}