#include <stdio.h>

#define MAX 20  
#define QUANTUM 3
//structure to store process info
typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
} Process;

//Simple circular queue implem. Это чтобы не усложнять код указателями.
typedef struct {
    int data[MAX];
    int head;   // indx to remove
    int tail;   // indx to insert
    int size;   //current num of elem
} Queue;

void q_init(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

int q_empty(Queue *q) {
    return (q->size == 0);
}

int q_full(Queue *q) {
    return (q->size == MAX);
}

// помещаем pid в хвост очереди, add elem to the queue
void q_push(Queue *q, int pid) {
    if (q_full(q)) {
        //ignoring simple ver в реале надо обработать
        return;
    }
    q->data[q->tail] = pid;
    q->tail = (q->tail + 1) % MAX;
    q->size++;
}

// извлекаем pid из головs очереди, возвращаем -1 если пусто
//remove n return the front el
int q_pop(Queue *q) {
    if (q_empty(q)) return -1;
    int pid = q->data[q->head];
    q->head = (q->head + 1) % MAX;
    q->size--;
    return pid;
}

int main() {
    Process proc[MAX];
    int n;

    printf("Round Robin Simulation (simple)\n");
    printf("Enter number of processes (max %d): ", MAX);

    if (scanf("%d", &n) != 1) return 0;
    if (n <= 0 || n > MAX) {
        printf("Bad number of processes\n");
        return 0;
    }

    // Ввод параметров процессов.
    // Пояснение: для каждого процесса вводим время прихода (arrival)
    // и необходимое процессорное время (burst).
    //input for each process from user
    for (int i = 0; i < n; i++) {
        proc[i].pid = i;
        printf("\n--- Process P%d ---\n", i);

        printf("Arrival time: ");
        scanf("%d", &proc[i].arrival);

        printf("Burst time (CPU time needed): ");
        scanf("%d", &proc[i].burst);

        proc[i].remaining = proc[i].burst;
        proc[i].completion = -1;
    }

    // ready queue n helper arry process is already inside?
    Queue ready;
    q_init(&ready);

    // in_ready[i] = 1 означает, что процесс i уже находится в очереди ready
    // это нужно, чтобы не добавлять один и тот же процесс несколько раз
    int in_ready[MAX] = {0};

    int time = 0;        // simulat clock
    int finished = 0;    // сколько процессов завершено

    printf("\n--- Simulation trace ---\n");

    // Основной цикл: продолжаем, пока не завершены все процессы
    //main loop
    while (finished < n) {

        // 1Добавляем в очередь все процессы, у которых arrival <= time и которые еще не завершены
        //    и пока не в очереди.
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= time && proc[i].remaining > 0 && !in_ready[i]) {
                q_push(&ready, i);
                in_ready[i] = 1;
            }
        }

        // 2 Если очередь пустая — нет готовых процессов, продвигаем время на 1
        //  if no process is ready,move time forward
        if (q_empty(&ready)) {
            time++;
            continue;
        }

        // 3Берём процесс из очереди киеуе (по принципу FIFO)
        int pid = q_pop(&ready);
        in_ready[pid] = 0; //allow process to be added later

        // 4Печать для наглядности: какой процесс сейчас использует CPU
        printf("Time %2d: Running P%d (remaining before = %d)\n", 
            time, pid, proc[pid].remaining);

        // 5execute for quantum or until proces finishes
        int qtime = QUANTUM;
        if (proc[pid].remaining < qtime) qtime = proc[pid].remaining;

        proc[pid].remaining -= qtime;
        time += qtime; // advance clock

        // 5 Проверяем завершение
        if (proc[pid].remaining == 0) {
            proc[pid].completion = time;
            finished++;
            printf("         P%d finished at time %d\n", pid, time);
        } else {
            // Процесс не завершился — возвращаем его в конец ready-очереди
            //preempt back into ready queie obratno koroche
            q_push(&ready, pid);
            in_ready[pid] = 1;
            printf("         P%d preempted (remaining now = %d) -> requeued\n", 
                pid, proc[pid].remaining);
        }
    }

    // После завершения всех процессов — выводим таблицу результатов
    //summary finalnogo
    printf("\n--- Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

    double total_tat = 0;
    double total_wt = 0;

    for (int i = 0; i < n; i++) {
        int tat = proc[i].completion - proc[i].arrival; // Turnaround Time
        int wt = tat - proc[i].burst;                   // Waiting Time

        total_tat += tat;
        total_wt += wt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].pid,
               proc[i].arrival,
               proc[i].burst,
               proc[i].completion,
               tat,
               wt);
    }
    printf("\nAverage TAT = %.2f\n", total_tat / n);
    printf("Average WT  = %.2f\n", total_wt / n);

    return 0;

}
