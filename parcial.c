#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 10
#define LEVELS 3  

typedef struct {
    int id;
    int burst_time;
    int priority;
    int age; 
    clock_t start_time;  
    double total_cpu_time; 
} Process;

typedef struct {
    Process *queue[MAX_PROCESSES];
    int front, rear;
} Queue;

void initializeQueue(Queue *q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue *q) {
    return (q->front == -1);
}

void enqueue(Queue *q, Process *process) {
    if (q->rear == MAX_PROCESSES - 1) {
        printf("La cola está llena\n");
        exit(EXIT_FAILURE);  
    }

    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->queue[q->rear] = process;
}

Process *dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("La cola está vacía\n");
        exit(EXIT_FAILURE);
    }

    Process *process = q->queue[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;
    return process;
}

void ageProcesses(Queue *queues[], int levels) {
    for (int i = 0; i < levels; ++i) {
        if (!isEmpty(queues[i])) { 
            for (int j = queues[i]->front; j <= queues[i]->rear; ++j) {
                queues[i]->queue[j]->age++;
                if (queues[i]->queue[j]->age > 5 && i > 0) {
                    queues[i]->queue[j]->priority--;
                    Process *temp = queues[i]->queue[j];
                    enqueue(queues[i-1], temp);
                    
                    for (int k = j; k < queues[i]->rear; k++) {
                        queues[i]->queue[k] = queues[i]->queue[k+1];
                    }
                    queues[i]->rear--;
                    j--; 
                }
            }
        }
    }
}

int main() {
    Queue *queues[LEVELS];  
    for (int i = 0; i < LEVELS; i++) {
        queues[i] = malloc(sizeof(Queue));
        initializeQueue(queues[i]);
    }

    Process *process1 = malloc(sizeof(Process));
    process1->id = 1;
    process1->burst_time = 5;
    process1->priority = 0;
    process1->age = 0;
    process1->total_cpu_time = 0;

    Process *process2 = malloc(sizeof(Process));
    process2->id = 2;
    process2->burst_time = 3;
    process2->priority = 1;
    process2->age = 0;
    process2->total_cpu_time = 0;

    Process *process3 = malloc(sizeof(Process));
    process3->id = 3;
    process3->burst_time = 7;
    process3->priority = 2;
    process3->age = 0;
    process3->total_cpu_time = 0;

    enqueue(queues[process1->priority], process1);
    enqueue(queues[process2->priority], process2);
    enqueue(queues[process3->priority], process3);

    printf("Ejecutando procesos en MLFQ:\n");
    while (!isEmpty(queues[0]) || !isEmpty(queues[1]) || !isEmpty(queues[2])) {
        for (int i = 0; i < LEVELS; i++) {
            while (!isEmpty(queues[i])) {
                Process *current_process = dequeue(queues[i]);
                if (current_process == NULL) continue;  
                current_process->start_time = clock();  
                printf("Proceso %d en ejecución desde la cola %d\n", current_process->id, i);
                current_process->burst_time--;
                clock_t end_time = clock(); 
                current_process->total_cpu_time += (double)(end_time - current_process->start_time) / CLOCKS_PER_SEC;
                if (current_process->burst_time > 0) {
                    enqueue(queues[i], current_process);
                } else {
                    printf("Proceso %d completado. Tiempo total de CPU: %.6f segundos.\n", current_process->id, current_process->total_cpu_time);
                    free(current_process);
                }
            }
        }
        ageProcesses(queues, LEVELS);
    }

    for (int i = 0; i < LEVELS; i++) {  // Liberar memoria de las colas
        free(queues[i]);
    }

    return 0;
}
