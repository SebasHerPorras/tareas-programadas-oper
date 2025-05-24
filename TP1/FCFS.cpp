#include "FCFS.hpp"

int FCFS(const std::vector<int>& solicitudes, int posCabeza) {
  int posActual = posCabeza;
  int recorrido = 0;
  for(int solicitud : solicitudes) {
    recorrido += abs(solicitud - posActual);
    posActual = solicitud;
  }
  return recorrido;
}