#include "convertirEntrada.hpp"
#include "ShortestJob.hpp"
#include "RoundRobin.hpp"
#include "Priority.hpp"
#include "FirstComeFirstServe.hpp"

#include <fstream>
#include <iostream>
#include <memory>

void castArguments(int argc, char* argv[]);

std::string archivo;
std::vector<std::pair<std::string, int>> resultados;
int main(int argc, char* argv[]) {
    castArguments(argc, argv);

    std::ifstream file("tests/" + archivo);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        return 1;
    }

    std::string entrada;
    while (std::getline(file, entrada)) {
        try {
            // Procesar cada línea del archivo
            std::cout << "\n\nEntrada: " << entrada << std::endl;
            
            auto [frames, initial_state, algorithm, references] = procesarEntrada(entrada);

            // Mostrar información procesada
            std::cout << "Frames: " << frames << std::endl;
            
            std::cout << "Estado inicial: ";
            for (int page : initial_state) {
                std::cout << page << " ";
            }
            std::cout << std::endl;
            
            std::cout << "Algoritmo: " << algorithm << std::endl;
            
            std::cout << "Referencias: ";
            for (int ref : references) {
                if (ref < 0) {
                    std::cout << -ref << "* ";
                } else {
                    std::cout << ref << " ";
                }
            }
            std::cout << std::endl;

            // Ejecutar el algoritmo correspondiente
            
            //if (algorithm == "") {
            //    resultados.push_back({"FIFO", FIFO(references, frames, initial_state)});
            //} else if (algorithm == "Second_Chance") {
            //    resultados.push_back({"Second_Chance", Second_Chance(references, frames, initial_state)});
            //} else if (algorithm == "NRU") {
            //    resultados.push_back({"NRU", NRU(references, frames, initial_state)});
            //} else if (algorithm == "LRU") {
            //    resultados.push_back({"LRU", LRU(references, frames, initial_state)});
            //} else if (algorithm == "Clock") {
            //        resultados.push_back({"Clock", Clock(references, frames, initial_state)});
            //} else if (algorithm == "LFU") {
            //    resultados.push_back({"LFU", LFU(references, frames, initial_state)});
            //} else if (algorithm == "MFU") {
            //    resultados.push_back({"MFU", MFU(references, frames, initial_state)});
            //} else {
            //    std::cerr << "Error: Algoritmo desconocido " << algorithm << std::endl;
            //    continue;
            //}

        } catch (const std::exception& e) {
            std::cerr << "Error procesando entrada: " << e.what() << std::endl;
        }
    }
    return 0;
}

void castArguments(int argc, char* argv[]) {
    if (argc > 1) {
        archivo = argv[1];
    } else {
        archivo = "prueba_1.txt";
    }
}