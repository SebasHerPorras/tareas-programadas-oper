
#include "convertirEntrada.hpp"

std::vector<std::string> dividirEntrada(const std::string& entrada, char delimitador) {
  std::vector<std::string> resultado;
  std::stringstream ss(entrada);
  std::string token;
  while (std::getline(ss, token, delimitador)) {
    resultado.push_back(token);
  }
  return resultado;
}


std::vector<int> convertirEnteros(const std::string& entrada) {
  std::vector<int> resultado;
  std::vector<std::string> entradaString = dividirEntrada(entrada, ',');
  for (const std::string& token : entradaString) {
    resultado.push_back(stoi(token));
  }
  return resultado;
}

std::tuple<std::vector<int>, std::string, int, std::string> procesarEntrada(const std::string entrada) {
  std::vector<std::string> stringDividido = dividirEntrada(entrada, '|');
  std::vector<int> solicitudes = convertirEnteros(stringDividido[0]);

  std::string algoritmo = stringDividido[1];

  int posCabeza = 0;
  if (stringDividido.size() > 2 &&
   stringDividido[2].find("Head:") != std::string::npos) {
    posCabeza = stoi(stringDividido[2].substr(stringDividido[2].find(':') + 1));
  }

  std::string dirCabeza = "ASC";  // Valor por defecto

if (stringDividido.size() > 3) {
    std::string entrada = stringDividido[3];

    // Quitamos espacios en blanco al inicio y al final
      entrada.erase(0, entrada.find_first_not_of(' '));
      entrada.erase(entrada.find_last_not_of(' ') + 1);

    // Convertimos a mayúsculas por si viene "asc" o "desc"
      std::transform(entrada.begin(), entrada.end(), entrada.begin(), ::toupper);

    // Limpiar posibles saltos de línea o caracteres extra al final
    if (!entrada.empty() && (entrada.back() == '\n' || entrada.back() == '\r')) {
        // si es un txt hay que quitar el retorno de carro
        entrada.pop_back();  // elimina el último carácter si es '\n' o '\r'
    }
    if (entrada == "ASC" || entrada == "DESC") {
        dirCabeza = entrada;
    } else {
       std::cout << "Dirección inválida. Se usará ASC por defecto. ud puso:" << entrada << "|" << std::endl;
    }
}

return {solicitudes, algoritmo, posCabeza, dirCabeza};


}
