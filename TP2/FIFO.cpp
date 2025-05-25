#include "FIFO.hpp"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <iostream>


// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

int FIFO(const std::vector<int>& references, int frames, const std::vector<int>& initial_state) {
    std::unordered_set<int> memory;          // Páginas actualmente en memoria
    std::queue<int> page_queue;              // Cola para orden FIFO
    int page_faults = 0;                     // Contador de fallos de página
    std::vector<bool> modified_pages(1000);  // Bit M para páginas (asumiendo páginas < 1000)

    // Inicializar memoria con el estado inicial
    for (int page : initial_state) {
        int page_num = abs(page);
        if (page_num != 0) {
            memory.insert(page_num);
            page_queue.push(page_num);
            if (page < 0) {
                modified_pages[page_num] = true;
            }
        }
    }

    // Función auxiliar para imprimir el estado actual de la memoria
    auto print_memory = [&]() {
        std::queue<int> temp = page_queue;
        std::cout << "   Estado actual: [ ";
        while (!temp.empty()) {
            int p = temp.front(); temp.pop();
            if (modified_pages[p]) {
                std::cout << "*" << p << " ";
            } else {
                std::cout << p << " ";
            }
        }
        std::cout << "]" << std::endl;
    };

    // Procesar referencias
    for (int ref : references) {
        int page = abs(ref);
        bool is_modified = (ref < 0);

        if (memory.find(page) == memory.end()) {
            page_faults++;
            std::cout << RED << "[FIFO] Fallo de página - Cargando: " << page;
            if (is_modified) std::cout << " (modificada)";
            std::cout << RESET << std::endl;

            if (memory.size() >= static_cast<size_t>(frames)) {
                int victim = page_queue.front();
                page_queue.pop();
                memory.erase(victim);

                if (modified_pages[victim]) {
                    std::cout << MAGENTA << "   Escribiendo página " << victim << " a swap (modificada)" << RESET << std::endl;
                }
            }

            memory.insert(page);
            page_queue.push(page);
        } else {
            std::cout << YELLOW << "[FIFO] Página " << page << " ya está en memoria" << RESET << std::endl;
        }

        if (is_modified) {
            modified_pages[page] = true;
        }

        print_memory();
    }

    std::cout << RED << "[FIFO] Total de fallos de página: " << page_faults << RESET << std::endl;
    return page_faults;
}