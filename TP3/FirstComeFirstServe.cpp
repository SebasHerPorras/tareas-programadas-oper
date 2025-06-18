#include "FirstComeFirstServe.hpp"

Metricas FCFS(std::vector<Proceso>& procesos) {
    Metricas metricas;
    int cantidad_procesos = procesos.size();
    
    // Ordenar procesos por tiempo de llegada (FCFS)
    std::sort(procesos.begin(), procesos.end(), 
        [](const Proceso& a, const Proceso& b) {
            return a.llegada < b.llegada;
        });
    
    // Calcular tiempos de finalización
    int tiempo_actual = 0;
    // recordar cada sub-i de proceso contiene: llegada, burst_time, waiting, turnaround, throughput
    for (auto& p : procesos) {
        // Tiempo de espera = tiempo actual - tiempo de llegada
        int waiting = std::max(0, tiempo_actual - p.llegada);
        metricas.waiting_por_proceso.push_back(waiting);
        
        // Turnaround = waiting time + burst time
        int turnaround = waiting + p.burst_time;
        metricas.turnaround_por_proceso.push_back(turnaround);
        
        // Throughput = 1 / turnaround (para este proceso)
        metricas.throughput_por_proceso.push_back(1.0 / turnaround);
        
        // Actualizar tiempo actual
        tiempo_actual += p.burst_time;
    }
    
    // Calcular promedios
    double sum_waiting = 0, sum_turnaround = 0, sum_throughput = 0;
    for (int i = 0; i < cantidad_procesos; ++i) {
        sum_waiting += metricas.waiting_por_proceso[i];
        sum_turnaround += metricas.turnaround_por_proceso[i];
        sum_throughput += metricas.throughput_por_proceso[i];
    }
    
    metricas.waiting_promedio = sum_waiting / cantidad_procesos;
    metricas.turnaround_promedio = sum_turnaround / cantidad_procesos;
    metricas.throughput_promedio = sum_throughput / cantidad_procesos;
    
    return metricas;
}