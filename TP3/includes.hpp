#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream> 


struct Proceso {
    int id;
    int burst_time;
    int prioridad;
    int llegada;
};

#endif // INCLUDES_HPP