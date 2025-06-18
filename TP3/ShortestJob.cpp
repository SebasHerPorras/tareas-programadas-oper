#include "ShortestJob.hpp"
#include <algorithm>
#include <queue>

// Asume que cada Proceso tiene un ID único
Metricas SJF(std::vector<Proceso>& procesos) {
    Metricas metricas;
    int cantidad_procesos = procesos.size();

    // Asegurar que cada proceso tiene su ID correspondiente
    for (int i = 0; i < cantidad_procesos; ++i) procesos[i].id = i;

    // Ordenar por tiempo de llegada
    std::sort(procesos.begin(), procesos.end(),
        [](const Proceso& a, const Proceso& b) {
            return a.llegada < b.llegada;
        });

    // Min-heap para elegir el proceso con menor burst_time entre los listos
    auto cmp = [](const Proceso& a, const Proceso& b) {
        return a.burst_time > b.burst_time;
    };
    std::priority_queue<Proceso, std::vector<Proceso>, decltype(cmp)> cola(cmp);

    int tiempo_actual = 0;
    int index = 0;

    std::vector<int> waiting(cantidad_procesos), turnaround(cantidad_procesos);
    std::vector<double> throughput(cantidad_procesos);

    while (index < cantidad_procesos || !cola.empty()) {
        // Agregar procesos que hayan llegado hasta el tiempo actual
        while (index < cantidad_procesos && procesos[index].llegada <= tiempo_actual) {
            cola.push(procesos[index]);
            index++;
        }

        if (!cola.empty()) {
            Proceso p = cola.top();
            cola.pop();

            int start_time = tiempo_actual;
            int end_time = start_time + p.burst_time;

            waiting[p.id] = start_time - p.llegada;
            turnaround[p.id] = end_time - p.llegada;
            throughput[p.id] = 1.0 / turnaround[p.id];

            tiempo_actual = end_time;
        } else {
            // Si no hay procesos listos, avanzar al siguiente tiempo de llegada
            tiempo_actual = procesos[index].llegada;
        }
    }

    // Guardar resultados en el struct Metricas
    double suma_waiting = 0, suma_turnaround = 0, suma_throughput = 0;
    for (int i = 0; i < cantidad_procesos; ++i) {
        metricas.waiting_por_proceso.push_back(waiting[i]);
        metricas.turnaround_por_proceso.push_back(turnaround[i]);
        metricas.throughput_por_proceso.push_back(throughput[i]);

        suma_waiting += waiting[i];
        suma_turnaround += turnaround[i];
        suma_throughput += throughput[i];
    }

    metricas.waiting_promedio = suma_waiting / cantidad_procesos;
    metricas.turnaround_promedio = suma_turnaround / cantidad_procesos;
    metricas.throughput_promedio = suma_throughput / cantidad_procesos;

    return metricas;
}
