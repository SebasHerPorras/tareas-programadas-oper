#include "FirstComeFirstServe.hpp"
#include "ShortestJob.hpp"
#include "RoundRobin.hpp"
#include "Priority.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct Proceso {
    int id;
    int burst_time;
    int prioridad;
    int llegada;
};

void parseEntrada(const std::string& entrada, int& n, std::vector<Proceso>& procesos, std::string& algoritmo);

int main(int argc, char* argv[]) {
    std::string archivo = (argc > 1) ? argv[1] : "prueba_1.txt";
    std::ifstream file("tests/" + archivo);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        return 1;
    }

    std::string entrada;
    while (std::getline(file, entrada)) {
        int n = 0;
        std::vector<Proceso> procesos;
        std::string algoritmo;
        parseEntrada(entrada, n, procesos, algoritmo);

        std::cout << "\nEntrada: " << entrada << std::endl;
        std::cout << "Algoritmo: " << algoritmo << std::endl;
        std::cout << "Procesos:\n";
        for (const auto& p : procesos) {
            std::cout << "P" << p.id << " Burst: " << p.burst_time
                      << " Prioridad: " << p.prioridad
                      << " Llegada: " << p.llegada << std::endl;
        }

        // Ejecutar el algoritmo correspondiente (debes implementar estas funciones)
        if (algoritmo == "FCFS") {
            int fcfs = FCFS(procesos);
        } else if (algoritmo == "SJF") {
            int sjf = SJF(procesos);
        } else if (algoritmo == "RoundRobin") {
            int sjf = RoundRobin(procesos, 2);
        } else if (algoritmo == "Priority") {
            int priority = Priority(procesos);
        } else if (algoritmo == "all") {
            ejecutarFCFS(procesos);
            ejecutarSJF(procesos);
            ejecutarRoundRobin(procesos, 2);
            ejecutarPriority(procesos);
        } else {
            std::cerr << "Algoritmo no reconocido: " << algoritmo << std::endl;
        }
    }
    return 0;
}

// Esta función parsea la entrada según el formato del enunciado
void parseEntrada(const std::string& entrada, int& n, std::vector<Proceso>& procesos, std::string& algoritmo) {
    std::stringstream ss(entrada);
    std::string token;
    std::vector<std::string> partes;

    while (std::getline(ss, token, '|')) {
        partes.push_back(token);
    }
    if (partes.size() < 5) throw std::runtime_error("Formato de entrada incorrecto");

    // Quitar espacios
    for (auto& s : partes) {
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    }

    n = std::stoi(partes[0]);
    std::vector<int> burst, prioridad, llegada;

    // Burst times
    std::stringstream ss_burst(partes[1]);
    while (std::getline(ss_burst, token, ',')) burst.push_back(std::stoi(token));

    // Prioridad
    std::stringstream ss_prio(partes[2]);
    while (std::getline(ss_prio, token, ',')) prioridad.push_back(std::stoi(token));
    if (prioridad.size() == 1 && prioridad[0] == 0) prioridad = std::vector<int>(n, 1);

    // Llegada
    std::stringstream ss_llegada(partes[3]);
    while (std::getline(ss_llegada, token, ',')) llegada.push_back(std::stoi(token));
    if (llegada.size() == 1 && llegada[0] == 0) llegada = std::vector<int>(n, 0);

    // Algoritmo
    algoritmo = partes[4];

// Construir procesos de forma sencilla
    for (int i = 0; i < n; ++i) {
        Proceso p;
        p.id = i + 1;
        p.burst_time = burst[i];
        p.prioridad = prioridad[i];
        p.llegada = llegada[i];
        procesos.push_back(p);
    }