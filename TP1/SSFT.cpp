#include "includes.hpp"



int SSFT(const std::vector<int>& solicitudes, int posCabeza) {
    int recorrido = 0;
    int cabeza = posCabeza;
    std::vector<int> pendientes = solicitudes;

    std::cout << "Orden de atención: " << cabeza;

    while (!pendientes.empty()) {
        int distanciaMinima = std::numeric_limits<int>::max();
        int indiceMasCercano = -1;

        // Buscar la solicitud más cercana
        for (size_t i = 0; i < pendientes.size(); ++i) {
            // std::abs es valor absoluto para saber la distancia que hay entre la cabeza y la solicitud
            int distancia = std::abs(pendientes[i] - cabeza);
            if (distancia < distanciaMinima) {
                distanciaMinima = distancia;
                indiceMasCercano = i;
            }
        }

        // Moverse a la solicitud más cercana
        cabeza = pendientes[indiceMasCercano];
        recorrido += distanciaMinima;
        std::cout << " -> " << cabeza;

        // Eliminar la solicitud atendida
        pendientes.erase(pendientes.begin() + indiceMasCercano);
    }
    std::cout << std::endl;
    return recorrido;
}
