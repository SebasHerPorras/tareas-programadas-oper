#include "ShortestJob.hpp"
#include <algorithm>
#include <queue>

// Asume que cada Proceso tiene un ID único
Metricas SJF(std::vector<Proceso>& procesos) {
    Metricas metricas;
    int n = procesos.size();

    // Asegurar que cada proceso tiene su ID correspondiente
    for (int i = 0; i < n; ++i) procesos[i].id = i;

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

    std::vector<int> waiting(n), turnaround(n);
    std::vector<double> throughput(n);

    while (index < n || !cola.empty()) {
        // Agregar procesos que hayan llegado hasta el tiempo actual
        while (index < n && procesos[index].llegada <= tiempo_actual) {
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
    for (int i = 0; i < n; ++i) {
        metricas.waiting_por_proceso.push_back(waiting[i]);
        metricas.turnaround_por_proceso.push_back(turnaround[i]);
        metricas.throughput_por_proceso.push_back(throughput[i]);

        suma_waiting += waiting[i];
        suma_turnaround += turnaround[i];
        suma_throughput += throughput[i];
    }

    metricas.waiting_promedio = suma_waiting / n;
    metricas.turnaround_promedio = suma_turnaround / n;
    metricas.throughput_promedio = suma_throughput / n;

    return metricas;
}
