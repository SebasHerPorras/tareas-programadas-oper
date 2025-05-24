#include "NRU.hpp"
// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"
#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

struct PageEntry {
    int page_num;
    bool reference_bit;
    bool modified_bit;
};

void NRU(const std::vector<int>& references, int frames, const std::vector<int>& initial_state) {
    std::deque<PageEntry> memory;                     // Cola circular
    std::unordered_map<int, PageEntry*> page_table;   // Mapeo página -> entrada
    int page_faults = 0;

    // Inicializar memoria
    for (int p : initial_state) {
        int page = abs(p);
        if (page == 0) continue;
        bool is_modified = (p < 0);
        memory.push_back({page, true, is_modified});
        page_table[page] = &memory.back();
    }

    auto print_memory = [&]() {
        std::cout << "   Estado actual: [ ";
        for (auto& entry : memory) {
            if (entry.modified_bit)
                std::cout << "*" << entry.page_num << " ";
            else
                std::cout << entry.page_num << " ";
        }
        std::cout << "]" << std::endl;
    };

    for (int ref : references) {
        int page = abs(ref);
        bool is_modified = (ref < 0);

        if (page_table.find(page) != page_table.end()) {
            std::cout << YELLOW << "[ESC] Página " << page << " ya está en memoria" << RESET << std::endl;
            auto* entry = page_table[page];
            entry->reference_bit = true;
            if (is_modified) entry->modified_bit = true;
        } else {
            page_faults++;
            std::cout << RED << "[ESC] Fallo de página - Cargando: " << page;
            if (is_modified) std::cout << " (modificada)";
            std::cout << RESET << std::endl;

            if (memory.size() >= static_cast<size_t>(frames)) {
                // Buscar víctima por clases
                int passes = 0;
                PageEntry* victim = nullptr;

                while (!victim && passes < 4) {
                    for (size_t i = 0; i < memory.size(); ++i) {
                        PageEntry& candidate = memory.front();

                        int class_id = (candidate.reference_bit << 1) | candidate.modified_bit;
                        if (class_id == passes) {
                            victim = &candidate;
                            break;
                        } else {
                            // Dar segunda oportunidad
                            if (candidate.reference_bit) candidate.reference_bit = false;
                            memory.push_back(candidate);
                        }
                        memory.pop_front();
                    }
                    passes++;
                }

                if (victim) {
                    std::cout << MAGENTA << "   Reemplazando página " << victim->page_num;
                    if (victim->modified_bit)
                        std::cout << " (escribiendo a swap)";
                    std::cout << RESET << std::endl;

                    page_table.erase(victim->page_num);

                    // Eliminar efectivamente al victim
                    for (auto it = memory.begin(); it != memory.end(); ++it) {
                        if (it->page_num == victim->page_num) {
                            memory.erase(it);
                            break;
                        }
                    }
                }
            }

            memory.push_back({page, true, is_modified});
            page_table[page] = &memory.back();
        }

        print_memory();
    }

    std::cout << RED << "[ESC] Total de fallos de página: " << page_faults << RESET << std::endl;
}
