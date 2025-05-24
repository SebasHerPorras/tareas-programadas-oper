#include "CSCAN.hpp"

int CSCAN(const std::vector<int>& solicitudes, int posCabeza, const std::string& orden) {
    const int CILINDRO_MAX = 199;
    const int CILINDRO_MIN = 0;

    int recorrido = 0;
    int cabeza = posCabeza;

    std::vector<int> mayores, menores;
    std::vector<int> ordenRecorrido;  // <<<<< Para almacenar el orden de recorrido

    // Separar solicitudes en menores y mayores que la cabeza
    for (int s : solicitudes) {
        if (s >= cabeza)
            mayores.push_back(s);
        else
            menores.push_back(s);
    }

    // Ordenar las solicitudes en cada grupo
    std::sort(mayores.begin(), mayores.end());
    std::sort(menores.begin(), menores.end());

    if (orden == "ASC") {
        for (int s : mayores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(cabeza);
        }

        if (!menores.empty()) {
            recorrido += std::abs(cabeza - CILINDRO_MAX);
            cabeza = CILINDRO_MAX;
            ordenRecorrido.push_back(cabeza);

            recorrido += std::abs(CILINDRO_MAX - CILINDRO_MIN);
            cabeza = CILINDRO_MIN;
            ordenRecorrido.push_back(cabeza);

            for (int s : menores) {
                recorrido += std::abs(cabeza - s);
                cabeza = s;
                ordenRecorrido.push_back(cabeza);
            }
        }

    } else if (orden == "DESC") {
        std::reverse(menores.begin(), menores.end());
        for (int s : menores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(cabeza);
        }

        if (!mayores.empty()) {
            recorrido += std::abs(cabeza - CILINDRO_MIN);
            cabeza = CILINDRO_MIN;
            ordenRecorrido.push_back(cabeza);

            recorrido += std::abs(CILINDRO_MAX - CILINDRO_MIN);
            cabeza = CILINDRO_MAX;
            ordenRecorrido.push_back(cabeza);

            std::reverse(mayores.begin(), mayores.end());
            for (int s : mayores) {
                recorrido += std::abs(cabeza - s);
                cabeza = s;
                ordenRecorrido.push_back(cabeza);
            }
        }

    } else {
        std::cerr << "Dirección inválida. Usa \"ASC\" o \"DESC\"." << std::endl;
        return -1;
    }

    std::cout << "Orden de recorrido: " << posCabeza;
    for (int pos : ordenRecorrido) {
        std::cout <<  " -> " <<pos;
    }
    std::cout << std::endl;

    std::cout << "Recorrido total: " << recorrido << " cilindros." << std::endl;
    return recorrido;
}
