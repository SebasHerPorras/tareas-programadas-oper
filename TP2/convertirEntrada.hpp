#ifndef CONVERTIRENTRADA_HPP
#define CONVERTIRENTRADA_HPP

#include <vector>
#include <string>
#include <tuple>

std::vector<std::string> dividirEntrada(const std::string& entrada, char delimitador);
std::vector<int> procesarLista(const std::string& listaStr);
std::tuple<int, std::vector<int>, std::string, std::vector<int>> procesarEntrada(const std::string& entrada);

#endif // CONVERTIRENTRADA_HPP