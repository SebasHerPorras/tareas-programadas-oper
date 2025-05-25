#include "Clock.hpp"

// Códigos de color ANSI
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

struct FrameClock {
  int page;
  bool modifed;
  bool reference_bit;
};

int Clock(const std::vector<int>& references, int frames, const std::vector<int>& initial_state) {
  std::vector<FrameClock> memory;
  int faults = 0;
  int pointer = 0;
  for (int pageItr : initial_state) {
    int page = std::abs(pageItr);
    bool modifed = (page < 0);
    if (page != 0) {
      memory.push_back({page, modifed, true});
    }
  }
  while ((int)memory.size() < frames) {
    memory.push_back({-1, false, false});
  }

  auto print_memory = [&]() {
    std::cout << "   Estado actual: [";
    for (const auto& f : memory) {
      if (f.page == -1) continue;
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
        frame.reference_bit = true;
        if (is_modified) { frame.modifed = true; }
        std::cout << YELLOW << "[Clock] Página " << page << (is_modified ? " (modificada)" : "") << " ya está en memoria" << RESET << "\n";
        print_memory();
        break;
      }
    }
    if (!on_memory) {
      faults++;
      while (true) {
        if (memory[pointer].page == -1) {
          // Espacio vacío
          memory[pointer] = {page, is_modified, true};
          std::cout << RED << "[Clock] Fallo de página - Cargando: " << page << (is_modified ? " (modificada)" : "") << RESET << "\n";
          pointer = (pointer + 1) % frames;
          break;
        }
        if (!memory[pointer].reference_bit) {
          // Reemplazar este marco
          if (memory[pointer].modifed) {
            std::cout << MAGENTA << "   Escribiendo página " << memory[pointer].page << " a swap (modificada)" << RESET << "\n";
          }    
          std::cout << RED << "[Clock] Fallo de página - Reemplazando página " << memory[pointer].page
          << " por " << page << (is_modified ? " (modificada)" : "") << RESET << "\n";
          memory[pointer] = {page, is_modified, true};
          pointer = (pointer + 1) % frames;
          break;
        } else {
          memory[pointer].reference_bit = false;
          pointer = (pointer + 1) % frames;
        } 
      }
      print_memory();
    }
  }
  std::cout << RED << "[Clock] Total de fallos de página: " << faults << RESET << "\n\n";
  return faults;
}