#include "convertirEntrada.hpp"
#include "includes.hpp"

#include <cctype>

std::string limpiarEspacios(const std::string& str) {
  size_t start = str.find_first_not_of(" \t\n\r\f\v");
  size_t end = str.find_last_not_of(" \t\n\r\f\v");
  if (start == std::string::npos) return "";
  return str.substr(start, end - start + 1);
}

std::vector<std::string> dividirEntrada(const std::string& entrada, char delimitador) {
  std::vector<std::string> resultado;
  std::stringstream ss(entrada);
  std::string item;
  while (std::getline(ss, item, delimitador)) {
    resultado.push_back(limpiarEspacios(item));
  }
  return resultado;
}

std::vector<int> procesarLista(const std::string& listaStr) {
  std::vector<int> resultado;
  std::stringstream ss(listaStr);
  std::string num;
  while (std::getline(ss, num, ',')) {
    num = limpiarEspacios(num);
    if (!num.empty())
      resultado.push_back(std::stoi(num));
  }
  return resultado;
}