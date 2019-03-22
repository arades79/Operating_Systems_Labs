#include <stdio.h>
#include <string.h>

struct Process {
    char name;
    int arrival_time;
    int service_time;

    int remaining_time;

    int start_time;
    int finish_time;
    int wait_time;
    int turnaround_time;
    int active_times[100];
    int len_active_times;
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
        if ((procs[i].remaining_time > 0) && (proc_delay < min_delay))
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
        if ((procs[i].arrival_time <= time) && (procs[i].remaining_time > 0) && (procs[i].arrival_time < min_arrival_time)) {
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
        if ((procs[i].arrival_time <= time) && (procs[i].remaining_time > 0) && (procs[i].remaining_time < min_length)) {
            min_length = procs[i].remaining_time;
            proc_idx = i;
        }
    }
    return proc_idx;
}

//TODO turnaround and wait time
void run(struct Process procs[5], int strat) {
    int time = 0;
    for (int i=0;i<5;i++) {
        int delay = get_delay(procs, time);
        time += delay;
        int next_idx;
        if (strat == 0)
            next_idx = get_next_fcfs(procs, time);
        else if (strat == 1)
            next_idx = get_next_spn(procs, time);
        procs[next_idx].start_time = time;
        for(int j=0;j<procs[next_idx].service_time;j++) {
            procs[next_idx].active_times[procs[next_idx].len_active_times] = j+time;
            procs[next_idx].len_active_times += 1;
        }
        time += procs[next_idx].service_time;
        procs[next_idx].remaining_time = 0;
        procs[next_idx].finish_time = time;
    }
}

int all_complete(struct Process procs[5]) {
    for(int i=0;i<5;i++)
        if (procs[i].remaining_time > 0)
            return 0;
    return 1;
}

void run_srt(struct Process procs[5]) {
    int time = 0;
    while(!all_complete(procs)) {
        int delay = get_delay(procs, time);
        time += delay;
        int next_idx = get_next_spn(procs, time);
        procs[next_idx].active_times[procs[next_idx].len_active_times] = time;
        procs[next_idx].len_active_times += 1;
        if (procs[next_idx].start_time == -1)
            procs[next_idx].start_time = time;
        time += 1;
        procs[next_idx].remaining_time -= 1;
        if (procs[next_idx].remaining_time == 0)
            procs[next_idx].finish_time = time;
    }
}

struct ProcessList get_processes(char filename[10]) {
    struct ProcessList pl;
    FILE *fp;
    fp = fopen(filename, "r");
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
        pl.processes[i].remaining_time = pl.processes[i].service_time;
        pl.processes[i].len_active_times = 0;
    }
    return pl;
}

void print_table(struct Process procs[5]) {
    printf("Process\tArrival Time\tService Time\tStart Time\tFinish Time\tWait Time\tTurnaround Time\n");
    for(int i=0;i<5;i++)
        printf("%c\t%i\t\t%i\t\t%i\t\t%i\t\t%i\t\t%i\n", procs[i].name, procs[i].arrival_time, procs[i].service_time, procs[i].start_time, procs[i].finish_time, procs[i].wait_time, procs[i].turnaround_time);
}

void print_gantt(struct Process procs[5]) {
    int max_time = 0;
    for (int i=0;i<5;i++) {
        if(procs[i].finish_time > max_time)
            max_time = procs[i].finish_time;
    }
    int active_proc[max_time];
    for(int proc_idx=0;proc_idx<5;proc_idx++) {
        for(int i=0;i<procs[proc_idx].len_active_times;i++) {
            active_proc[procs[proc_idx].active_times[i]] = proc_idx;
        }
    }
    for(int proc_idx=0;proc_idx<5;proc_idx++) {
        for(int time=0;time<=max_time;time++) {
            if(active_proc[time] == proc_idx)
                printf("%c",procs[proc_idx].name);
            else
                printf(" ");
        }
        printf("\n");
    }
}

int main() {
    printf("FCFS File 1:\n");
    struct ProcessList pl = get_processes("procs1.txt");
    run(pl.processes, 0);
    print_table(pl.processes);
    print_gantt(pl.processes);
    printf("SPN File 1:\n");
    pl = get_processes("procs1.txt");
    run(pl.processes, 1);
    print_table(pl.processes);
    print_gantt(pl.processes);
    printf("SRT File 1:\n");
    pl = get_processes("procs1.txt");
    run_srt(pl.processes);
    print_table(pl.processes);
    print_gantt(pl.processes);
    printf("FCFS File 2:\n");
    pl = get_processes("procs2.txt");
    run(pl.processes, 0);
    print_table(pl.processes);
    print_gantt(pl.processes);
    printf("SPN File 2:\n");
    pl = get_processes("procs2.txt");
    run(pl.processes, 1);
    print_table(pl.processes);
    print_gantt(pl.processes);
    printf("SRT File 2:\n");
    pl = get_processes("procs2.txt");
    run_srt(pl.processes);
    print_table(pl.processes);
    print_gantt(pl.processes);

}
