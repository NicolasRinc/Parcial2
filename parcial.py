import time

MAX_PROCESSES = 10
TIME_QUANTUM = 2
WAIT_THRESHOLD = 5

class Process:
    def __init__(self, _id, burst_time, priority):
        self.id = _id
        self.burst_time = burst_time
        self.priority = priority
        self.wait_time = 0

class Queue:
    def __init__(self):
        self.queue = []
    
    def is_empty(self):
        return len(self.queue) == 0
    
    def enqueue(self, process):
        self.queue.append(process)
    
    def dequeue(self):
        if not self.is_empty():
            return self.queue.pop(0)
        else:
            print("La cola está vacía")
            exit()

def execute_process(process, execution_time):
    print(f"Proceso {process.id} en ejecución durante {time} segundos")
    time.sleep(execution_time)

def adjust_priority(process):
    if process.wait_time >= WAIT_THRESHOLD:
        process.priority -= 1
        process.wait_time = 0
        print(f"Se ha aumentado la prioridad del proceso {process.id}")

def main():
    high_priority_queue = Queue()
    low_priority_queue = Queue()

    process1 = Process(1, 5, 1)
    process2 = Process(2, 3, 2)
    process3 = Process(3, 7, 1)

    high_priority_queue.enqueue(process1)
    low_priority_queue.enqueue(process2)
    high_priority_queue.enqueue(process3)

    print("Ejecutando procesos:")
    while not (high_priority_queue.is_empty() and low_priority_queue.is_empty()):
        if not high_priority_queue.is_empty():
            current_process = high_priority_queue.dequeue()
            execute_process(current_process, TIME_QUANTUM)
            current_process.burst_time -= TIME_QUANTUM
            current_process.wait_time += TIME_QUANTUM
            adjust_priority(current_process)
            if current_process.burst_time > 0:
                high_priority_queue.enqueue(current_process)
            else:
                del current_process

        if not low_priority_queue.is_empty():
            current_process = low_priority_queue.dequeue()
            execute_process(current_process, TIME_QUANTUM)
            current_process.burst_time -= TIME_QUANTUM
            current_process.wait_time += TIME_QUANTUM
            adjust_priority(current_process)
            if current_process.burst_time > 0:
                low_priority_queue.enqueue(current_process)
            else:
                del current_process

if __name__ == "__main__":
    main()
