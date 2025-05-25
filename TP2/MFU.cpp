#include "MFU.hpp"

// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

struct FrameMFU {
  int page;
  bool modifed;
  int frecuency;
  int order_arrival;
};

int MFU(const std::vector<int>& references, int frames, const std::vector<int>& initial_state) {
  std::vector<FrameMFU> memory;
  int time = 0;
  int faults = 0;
  // Inicializar memoria con el estado inicial
  for (int page : initial_state) {
    int page_num = std::abs(page);
    bool is_modifed = (page < 0);
    if (page_num != 0) {
      memory.push_back({page_num, is_modifed, 1, time++});
    }
  }

  auto print_memory = [&]() {
    std::cout << "   Estado actual: [";
    for (const auto& f : memory) {
      std::cout << (f.modifed ? " *" : " ") << f.page;
    }
    std::cout << " ]\n";
  };

  for (int ref : references) {
    int page = std::abs(ref);
    bool is_modified = (ref < 0);
    bool on_memory = false;

    for (auto& frame : memory) {
      if (frame.page == page) {
        on_memory = true;
        frame.frecuency++;
        if (is_modified){frame.modifed = true;}
        std::cout << YELLOW << "[MFU] Página " << page << (is_modified ? " (modificada)" : "") << " ya está en memoria" << RESET << "\n";
        print_memory();
        break;
      }
    }
    if (!on_memory) {
      faults++;
      if ((int)memory.size() < frames) {
        memory.push_back({page, is_modified, 1, time++});
        std::cout << RED << "[MFU] Fallo de página - Cargando: " << page << (is_modified ? " (modificada)" : "") << RESET << "\n";
      } else {
        int index_remplazo = 0;
        for (size_t i = 1; i < memory.size(); ++i) {
          if (memory[i].frecuency > memory[index_remplazo].frecuency ||
              (memory[i].frecuency == memory[index_remplazo].frecuency &&
              memory[i].order_arrival < memory[index_remplazo].order_arrival)) {
              index_remplazo = i;
          }
        }
        if (memory[index_remplazo].modifed) {
            std::cout << MAGENTA << "   Escribiendo página " << memory[index_remplazo].page << " a swap (modificada)" << RESET << "\n";
            std::cout << RED << "[MFU] Fallo de página - Reemplazando página " << memory[index_remplazo].page
            << " por " << page << (is_modified ? " (modificada)" : "") << RESET << "\n";
            memory[index_remplazo] = {page, is_modified, 1, time++};
        }
      }
      print_memory(); 
    }    
  }
  std::cout << RED << "[MFU] Total de fallos de página: " << faults << RESET << "\n\n";
  return faults;
}