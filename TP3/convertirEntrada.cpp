#include "convertirEntrada.hpp"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

// Función auxiliar para limpiar espacios
std::string limpiarEspacios(const std::string& str) {
    std::string resultado = str;
    resultado.erase(0, resultado.find_first_not_of(" \t\n\r\f\v"));
    resultado.erase(resultado.find_last_not_of(" \t\n\r\f\v") + 1);
    return resultado;
}

std::vector<std::string> dividirEntrada(const std::string& entrada, char delimitador) {
    std::vector<std::string> resultado;
    bool dentro_de_corchetes = false;
    std::string acumulado;

    for (char c : entrada) {
        if (c == '[') {
            dentro_de_corchetes = true;
            acumulado += c;
        } else if (c == ']') {
            dentro_de_corchetes = false;
            acumulado += c;
        } else if (c == delimitador && !dentro_de_corchetes) {
            if (!acumulado.empty()) {
                resultado.push_back(limpiarEspacios(acumulado));
                acumulado.clear();
            }
        } else {
            acumulado += c;
        }
    }

    if (!acumulado.empty()) {
        resultado.push_back(limpiarEspacios(acumulado));
    }

    return resultado;
}

std::vector<int> procesarLista(const std::string& listaStr) {
    std::vector<int> resultado;
    
    // Si es una lista entre corchetes
    if (listaStr.front() == '[' && listaStr.back() == ']') {
        std::string contenido = listaStr.substr(1, listaStr.size() - 2);
        std::vector<std::string> elementos = dividirEntrada(contenido, ',');
        
        for (const std::string& elem : elementos) {
            std::string elem_limpio = limpiarEspacios(elem);
            if (elem_limpio.empty()) continue;

            try {
                // Manejar páginas modificadas (con *)
                if (elem_limpio.back() == '*') {
                    std::string numStr = elem_limpio.substr(0, elem_limpio.size() - 1);
                    int num = std::stoi(numStr);
                    resultado.push_back(-num); // Valor negativo para modificadas
                } else {
                    resultado.push_back(std::stoi(elem_limpio));
                }
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Valor no numérico en lista: '" + elem_limpio + "'");
            }
        }
    } 
    // Si es un solo valor
    else {
        try {
            resultado.push_back(std::stoi(limpiarEspacios(listaStr)));
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Valor no numérico: '" + listaStr + "'");
        }
    }
    
    return resultado;
}

std::tuple<int, std::vector<int>, std::string, std::vector<int>> procesarEntrada(const std::string& entrada) {
    std::vector<std::string> partes = dividirEntrada(entrada, ',');
    
    // Validar estructura básica
    if (partes.size() != 4) {
        throw std::runtime_error("Formato incorrecto. Se espera: frames, [initial_state], algorithm, [reference_string]");
    }

    // 1. Procesar número de frames (con valor por defecto 4 si es 0)
    int frames;
    try {
        frames = std::stoi(partes[0]);
        if (frames < 0) {
            throw std::runtime_error("El número de frames no puede ser negativo");
        }
        if (frames == 0) {
            frames = 4; // Valor por defecto según especificación
        }
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Número de frames inválido: '" + partes[0] + "'");
    }

    // 2. Procesar estado inicial de memoria
    std::vector<int> initial_state = procesarLista(partes[1]);
    
    // Validar estado inicial
    if (initial_state.size() == 1 && initial_state[0] == 0) {
        // Caso especial [0] → memoria vacía
        initial_state = std::vector<int>(frames, 0);
    } else if (initial_state.size() != static_cast<size_t>(frames)) {
        throw std::runtime_error("El estado inicial debe tener " + std::to_string(frames) + 
                               " elementos (o [0] para vacío)");
    }

    // 3. Procesar algoritmo
    std::string algoritmo = limpiarEspacios(partes[2]);
    if (algoritmo.empty()) {
        throw std::runtime_error("El algoritmo no puede estar vacío");
    }

    // Lista de algoritmos válidos
    const std::vector<std::string> algoritmos_validos = {
        "FIFO", "Second_Chance", "NRU", "LRU", "Clock", "LFU", "MFU"
    };
    
    // Verificar si el algoritmo es válido
    if (std::find(algoritmos_validos.begin(), algoritmos_validos.end(), algoritmo) == algoritmos_validos.end()) {
        throw std::runtime_error("Algoritmo desconocido: '" + algoritmo + "'");
    }

    // 4. Procesar cadena de referencias
    std::vector<int> references = procesarLista(partes[3]);
    if (references.empty()) {
        throw std::runtime_error("La secuencia de referencias no puede estar vacía");
    }

    return {frames, initial_state, algoritmo, references};
}