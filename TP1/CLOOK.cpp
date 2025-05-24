#include "CLOOK.hpp"

int CLOOK(const std::vector<int>& solicitudes, int posCabeza, const std::string& orden) {
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
        if (!menores.empty()) {
            recorrido += std::abs(cabeza - menores.front());
            cabeza = menores.front();
            for (int s : menores) {
                recorrido += std::abs(cabeza - s);
                cabeza = s;
                ordenRecorrido.push_back(s);
            }
        }
    } else if (orden == "DESC") {
        std::reverse(menores.begin(), menores.end());
        for (int s : menores) {
            recorrido += std::abs(cabeza - s);
            cabeza = s;
            ordenRecorrido.push_back(s);
        }
        if (!mayores.empty()) {
            std::reverse(mayores.begin(), mayores.end());
            recorrido += std::abs(cabeza - mayores.front());
            cabeza = mayores.front();
            for (int s : mayores) {
                recorrido += std::abs(cabeza - s);
                cabeza = s;
                ordenRecorrido.push_back(s);
            }
        }
    } else {
        std::cerr << "Direcci\u00f3n inv\u00e1lida. Usa \"ASC\" o \"DESC\"." << std::endl;
        return -1;
    }

    std::cout << "Orden de recorrido C-LOOK: ";
    for (int s : ordenRecorrido)
        std::cout << s << " ";
    std::cout << "\nRecorrido total: " << recorrido << " cilindros.\n";
    return recorrido;
}
