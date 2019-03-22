#include <stdio.h>

struct Process {
    char name;
    int arrival_time;
    int service_time;

    int start_time;
    int finish_time;
    int wait_time;
    int turnaround_time;
};

struct ProcessList {
    struct Process processes[5];
};

void dprint(struct Process p) {
    printf("name%c arrival_time%i service_time%i start_time%i finish_time %i wait_time %i turnaround_time %i\n", p.name, p.arrival_time, p.service_time, p.start_time, p.finish_time, p.wait_time, p.turnaround_time);
}

int get_delay(struct Process procs[5], int time) {
    int min_delay = 10000;
    for(int i=0;i<5;i++) {
        int proc_delay = procs[i].arrival_time - time;
        if ((procs[i].start_time == -1) && (proc_delay < min_delay))
            min_delay = proc_delay;
    }
    if (min_delay < 0)
        return 0;
    return min_delay;
}

int get_next_fcfs(struct Process procs[5], int time) {
    int min_arrival_time = 10000;
    int proc_idx = -1;
    for(int i=0;i<5;i++) {
        if ((procs[i].arrival_time <= time) && (procs[i].finish_time == -1) && (procs[i].arrival_time < min_arrival_time)) {
            min_arrival_time = procs[i].arrival_time;
            proc_idx = i;
        }
    }
    return proc_idx;
}

int get_next_spn(struct Process procs[5], int time) {
    int min_length = 10000;
    int proc_idx = -1;
    for(int i=0;i<5;i++) {
        if ((procs[i].arrival_time <= time) && (procs[i].finish_time == -1) && (procs[i].service_time < min_length)) {
            min_length = procs[i].service_time;
            proc_idx = i;
        }
    }
    return proc_idx;
}

int get_next_srt(struct Process procs[5], int time) {

}
//TODO turnaround and wait time
void run(struct Process procs[5], int strat) {
    int time = 0;
    for (int i=0;i<5;i++) {
        printf("Time: %i\n", time);
        int delay = get_delay(procs, time);
        time += delay;
        int next_idx;
        if (strat == 0)
            next_idx = get_next_fcfs(procs, time);
        else if (strat == 1)
            next_idx = get_next_spn(procs, time);
        else
            next_idx = get_next_srt(procs, time);
        procs[next_idx].start_time = time;
        time += procs[next_idx].service_time;
        procs[next_idx].finish_time = time;
    }
}

struct ProcessList get_processes() {
    struct ProcessList pl;
    FILE *fp;
    fp = fopen("procs1.txt", "r");
    char buff[15];
    fgets(buff, 16, fp);
    fclose(fp);
    for(int i=0; i<15;i++) {
        int proc_num = i / 3;
        int part_num = i % 3;
        if (part_num == 0)
            pl.processes[proc_num].name = buff[i];
        else if (part_num == 1)
            pl.processes[proc_num].arrival_time = (int)buff[i] - '0';
        else if (part_num == 2)
            pl.processes[proc_num].service_time = (int)buff[i] - '0';
    }
    for(int i=0;i<5;i++) {
        pl.processes[i].start_time = -1;
        pl.processes[i].finish_time = -1;
        pl.processes[i].wait_time = -1;
        pl.processes[i].turnaround_time = -1;
    }
    return pl;
}

void print_table(struct Process procs[5]) {
    printf("Process\tArrival Time\tService Time\tStart Time\tFinish Time\tWait Time\tTurnaround Time\n");
    for(int i=0;i<5;i++)
        printf("%c\t%i\t\t%i\t\t%i\t\t%i\t\t%i\t\t%i\n", procs[i].name, procs[i].arrival_time, procs[i].service_time, procs[i].start_time, procs[i].finish_time, procs[i].wait_time, procs[i].turnaround_time);
}

int main() {
    struct ProcessList pl = get_processes();
    run(pl.processes, 1);
    print_table(pl.processes);
}
