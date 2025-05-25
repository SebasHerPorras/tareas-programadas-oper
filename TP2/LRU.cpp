#include "LRU.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <unordered_set>

// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

int LRU(const std::vector<int>& references, int frames, const std::vector<int>& initial_state) {
    std::list<int> page_list;                             // Lista de uso reciente (MRU al frente, LRU al final)
    std::unordered_map<int, std::list<int>::iterator> page_map;  // Mapa para acceso O(1) a posiciones en la lista
    std::unordered_set<int> memory;                       // Páginas en memoria
    std::vector<bool> modified_pages(1000);               // Bit M
    int page_faults = 0;

    // Inicializar con el estado inicial
    for (int page : initial_state) {
        int page_num = abs(page);
        if (page_num != 0) {
            page_list.push_front(page_num);
            page_map[page_num] = page_list.begin();
            memory.insert(page_num);
            if (page < 0) {
                modified_pages[page_num] = true;
            }
        }
    }

    // Función auxiliar para imprimir el estado actual de la memoria
    auto print_memory = [&]() {
        std::cout << "   Estado actual: [ ";
        for (int page : page_list) {
            if (modified_pages[page]) {
                std::cout << "*" << page << " ";
            } else {
                std::cout << page << " ";
            }
        }
        std::cout << "]" << std::endl;
    };

    // Procesar referencias
    for (int ref : references) {
        int page = abs(ref);
        bool is_modified = (ref < 0);

        // si falla se actualiza el bit de referencia
        // y se imprime el mensaje correspondiente

        if (memory.find(page) == memory.end()) {
            // Fallo de página
            page_faults++;
            std::cout << RED << "[LRU] Fallo de página - Cargando: " << page;
            if (is_modified) std::cout << " (modificada)";
            std::cout << RESET << std::endl;

            if (memory.size() >= static_cast<size_t>(frames)) {
                int lru_page = page_list.back();
                page_list.pop_back();
                page_map.erase(lru_page);
                memory.erase(lru_page);

                // Si la página a reemplazar está modificada, se simula escritura en swap
                // (en este caso solo se imprime un mensaje)
                if (modified_pages[lru_page]) {
                    std::cout << MAGENTA << "   Escribiendo página " << lru_page << " a swap (modificada)" << RESET << std::endl;
                }
            }

            page_list.push_front(page);
            page_map[page] = page_list.begin();
            memory.insert(page);
        } else {
            std::cout << YELLOW << "[LRU] Página " << page << " ya está en memoria" << RESET << std::endl;
            // Mover al frente (más recientemente usada)
            page_list.erase(page_map[page]);
            page_list.push_front(page);
            page_map[page] = page_list.begin();
        }

        // marcar la página como modificada si corresponde
        if (is_modified) {
            modified_pages[page] = true;
        }

        print_memory();
    }

    std::cout << RED << "[LRU] Total de fallos de página: " << page_faults << RESET << std::endl;
    return page_faults;
}
