#include "LOOK.hpp"

int LOOK(const std::vector<int>& solicitudes, int posCabeza, const std::string& orden) {
    int recorrido = 0;
    int cabeza = posCabeza;
    std::vector<int> mayores, menores, ordenRecorrido;

    for (int s : solicitudes) {
        if (s >= cabeza)
            mayores.push_back(s);
        else
            menores.push_back(s);
    }

    std::sort(mayores.begin(), mayores.end());
    std::sort(menores.begin(), menores.end());

    if (orden == "ASC") {
        for (int s : mayores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(s);
        }
        std::reverse(menores.begin(), menores.end());
        for (int s : menores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(s);
        }
    } else if (orden == "DESC") {
        std::reverse(menores.begin(), menores.end());
        for (int s : menores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(s);
        }
        for (int s : mayores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(s);
        }
    } else {
        std::cerr << "Direcci\u00f3n inv\u00e1lida. Usa \"ASC\" o \"DESC\"." << std::endl;
        return -1;
    }

    std::cout << "Orden de recorrido LOOK: ";
    for (int s : ordenRecorrido)
        std::cout << s << " ";
    std::cout << "\nRecorrido total: " << recorrido << " cilindros.\n";
    return recorrido;
}
