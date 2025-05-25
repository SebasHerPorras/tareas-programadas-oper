#include "Second_Chance.hpp"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cmath>

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

void Second_Chance(const std::vector<int>& referencias, int marcos, const std::vector<int>& estado_inicial) {
    std::queue<int> cola_paginas;                       // Orden de llegada
    std::unordered_map<int, bool> bit_referencia;      // Bit R por página
    std::unordered_map<int, bool> bit_modificado;      // Bit M por página
    std::unordered_set<int> memoria;                   // Páginas cargadas
    int page_faults = 0;

    // Cargar estado inicial en memoria
    for (int ref : estado_inicial) {
        int pagina = std::abs(ref);
        if (pagina != 0) {
            memoria.insert(pagina);
            cola_paginas.push(pagina);
            bit_referencia[pagina] = true;
            bit_modificado[pagina] = (ref < 0);
        }
    }

    // Función para imprimir estado actual de la memoria
    auto print_memoria = [&]() {
        std::queue<int> temp = cola_paginas;
        std::cout << "   Estado actual: [ ";
        while (!temp.empty()) {
            int p = temp.front(); temp.pop();
            if (bit_modificado[p]) std::cout << "*" << p << " ";
            else std::cout << p << " ";
        }
        std::cout << "]" << std::endl;
    };

    // Procesar referencias
    for (int ref : referencias) {
        int pagina = std::abs(ref);
        bool modificada = (ref < 0);

        // Si la página ya está en memoria se actualiza el bit de referencia
        // y se ignora el fallo de página
        if (memoria.find(pagina) != memoria.end()) {
            std::cout << YELLOW << "[SC] Página " << pagina << " ya está en memoria" << RESET << std::endl;
            bit_referencia[pagina] = true;
        } else {
            // Fallo de página
            // Si no está en memoria, se cuenta como fallo de página
            // y se imprime el mensaje correspondiente
            std::cout << RED << "[SC] Fallo de página - Cargando: " << pagina;
            if (modificada) std::cout << " (modificada)";
            std::cout << RESET << std::endl;
            page_faults++;

            // Si hay que reemplazar
            if (static_cast<int>(memoria.size()) >= marcos) {
                while (true) {
                    // Reemplazar la página al frente de la cola
                    // Si el bit de referencia es 1, se le da una segunda oportunidad
                    // y se vuelve a poner al final de la cola
                    // Si el bit de referencia es 0, se reemplaza
                    // si todos son 1 se hace un ciclo y se saca la primera
                    int candidato = cola_paginas.front(); cola_paginas.pop();
                    if (bit_referencia[candidato]) {
                        bit_referencia[candidato] = false;
                        cola_paginas.push(candidato);
                    } else {
                        memoria.erase(candidato);
                        if (bit_modificado[candidato]) {
                            std::cout << MAGENTA << "   Escribiendo página " << candidato << " a swap (modificada)" << RESET << std::endl;
                        }
                        break;
                    }
                }
            }

            // Insertar nueva página en memoria
            // y actualizar los bits de referencia y modificación
            memoria.insert(pagina);
            cola_paginas.push(pagina);
            bit_referencia[pagina] = true;
            bit_modificado[pagina] = modificada;
        }

        // Si ya estaba en memoria y ahora es modificada
        // solo se manejan bits de true o false con peso 1, no se suma
        // el bit de referencia a la memoria
        if (modificada) bit_modificado[pagina] = true;

        print_memoria();
    }

    std::cout << RED << "[SC] Total de fallos de página: " << page_faults << RESET << std::endl;
}
