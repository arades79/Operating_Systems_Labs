#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

static int do_part_2 = 0;

typedef enum pstate
{
    New,
    Ready,
    Running,
    Blocked,
    ReadySuspend,
    BlockedSuspend,
    Completed,
    NotExist
} pstate;

typedef enum action 
{
    Dispatched,
    IORequest,
    SwappedOut,
    SwappedIn,
    Interrupt,
    Terminated,
    Created,
    TimeExpires
} action;

typedef enum iodevice
{
    Disk,
    Keyboard,
    Monitor,
    Mouse,
    Printer
} iodevice;

static const char * file_paths[] = 
{
    "inputs/inp1.txt",
    "inputs/inp2.txt",
    "inputs/inp3.txt",
    "inputs/inp4.txt"
};

static const char * action_strings[] = 
{
    "Dispatched"
    "IORequest"
    "SwappedOut"
    "SwappedIn"
    "Interrupt"
    "Terminated"
    "Created"
    "TimeExpires"
};

static const char * device_strings[] = 
{
    "Disk",
    "Keyboard",
    "Monitor",
    "Mouse",
    "Printer"
};

static const char * state_strings[] = 
{
    "New",
    "Ready",
    "Running",
    "Blocked",
    "Ready/Suspend",
    "Blocked/Suspend",
    "Completed",
    "NotExist"
};

static pstate processes[] = 
{
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready,
    Ready
};

iodevice get_io_device(char *s) 
{
    if (strstr(s,"disk")) return Disk;
    if (strstr(s,"keyboard")) return Keyboard;
    if (strstr(s,"monitor")) return Monitor;
    if (strstr(s,"mouse")) return Mouse;
    if (strstr(s,"printer")) return Printer;
}

action get_action(char *s) 
{
    if (strstr(s,"is dispatched")) return Dispatched;
    if (strstr(s,"requests the")) return IORequest;
    if (strstr(s,"is swapped out")) return SwappedOut;
    if (strstr(s,"is swapped in")) return SwappedIn;
    if (strstr(s,"interrupt")) return Interrupt;
    if (strstr(s,"terminated")) return Terminated;
    if (strstr(s,"is created")) return Created;
    if (strstr(s,"slice")) return TimeExpires;
    printf("Got error with input while getting action: %s\n",s);
    return Dispatched;
}

pstate get_initial_state(char * s)
{
    if (strstr(s, "New")) return New;
    if (strstr(s, "Ready")) return Ready;
    if (strstr(s, "Ready/Suspend")) return ReadySuspend;
    if (strstr(s, "Blocked/Suspend")) return BlockedSuspend;
    if (strstr(s, "Running")) return Running;
    if (strstr(s, "Blocked")) return Blocked;
    if (strstr(s, "Completed")) return Completed;
    printf("invalid state %s", s);
    return -1;
}


int get_first_int(char* s) 
{
    //this is dumb but I don't care
    int start_index = -1;
    int end_index = -1;
    for (int i=0; end_index == -1; i++) 
    {
        if (s[i] == 0) return -1;
        if (isdigit(s[i]) && start_index == -1) start_index = i;
        if (!isdigit(s[i]) && start_index != -1) end_index = i-1;
    }
    int span = end_index - start_index;
    int sum = 0;
    for (int i=start_index;i<=end_index;i++)
    {
        int digit = s[i] - '0';
        sum += digit * pow(10.0,end_index-i);
    }
    return sum;
}
int get_pid(char* s) {
    return get_first_int(s);
}

void print_states() {
    for (int i=0;i<20;i++) {
        if(processes[i] != NotExist) {
            printf("P%d %s ",i,state_strings[processes[i]]);
        }
    }
    printf("\n");
}
    

int main() 
{
    //FILE *fp;
    // char current_line[100];
    // char sub_line[100];
    // size_t buffer_size;
    // int line_index = 0;
    // int line_index_offset = line_index;
    
    for (size_t file_num = 0; file_num < 4; ++file_num)
    {
        char current_line[500];
        char sub_line[100];
        size_t buffer_size;
        int line_index = 0;
        int line_index_offset = 0;
        
        //line_index = line_index_offset = 0;
        printf("Simulation %i begin:\n", file_num);
        
        for(int i=0;i<20;i++) {
            processes[i] = NotExist;
        }
        
        
        FILE *fp = fopen(file_paths[file_num],"r");
        if(fp == NULL) {
            printf("File was null");
            return -1;
        }
        fgets(current_line, 500, fp);
        
        //populate initial states
        while(current_line[line_index] != '\0') {
            line_index++;
            line_index_offset = line_index;
            int reached_first_space = 0;
            while((current_line[line_index] != ' ' && current_line[line_index] != '\0') || reached_first_space == 0) {
                if(current_line[line_index] == ' ') {
                    reached_first_space = 1;
                }
                sub_line[line_index-line_index_offset] = current_line[line_index];
                line_index++;
            }
            sub_line[line_index-line_index_offset] = '\0';
            processes[get_pid(sub_line)] = get_initial_state(sub_line);
        }
        
        printf("Initial states\n");
        print_states();
        
        while (fgets(current_line, 500, fp) != NULL)
        {
            if (strlen(current_line) < 3) {
                continue;
            }
            // printf("Here's currentline:%s:done",current_line);
            // printf("Got current_line: %s\n",current_line);
            int curr_time = get_first_int(current_line);
            line_index = 0;
            while(current_line[line_index] != ':') {
                line_index++;
            }
            line_index++;
            line_index_offset = line_index;
            
            int reached_end = 0;
            
            //while(current_line[line_index-1] != '.') {
            while(reached_end == 0) {
                line_index++;
                line_index_offset = line_index;
                while(current_line[line_index] != ';' && current_line[line_index] != '.') {
                    sub_line[line_index-line_index_offset] = current_line[line_index];
                    line_index++;
                }
                if(current_line[line_index] == '.') {
                    reached_end = 1;
                }
                sub_line[line_index-line_index_offset] = '.';
                sub_line[1+line_index-line_index_offset] = '\0';
                
                
                
                
                action next_action = get_action(sub_line);
                int current_process = get_pid(sub_line);
                switch (next_action)
                {
                    case Dispatched:
                        processes[current_process] = Running;
                        break;
                    case IORequest:
                        //next_device = get_io_device(sub_line); (Don't actually need to know the device)
                        processes[current_process] = Blocked;
                        break;
                    case SwappedOut:
                        if(processes[current_process] == Blocked) {
                            processes[current_process] = BlockedSuspend;
                        }
                        else {
                            processes[current_process] = ReadySuspend;
                        }
                        break;
                    case SwappedIn:
                        if(processes[current_process] == BlockedSuspend) {
                            processes[current_process] = Blocked;
                        }
                        else {
                            processes[current_process] = Ready;
                        }
                        break;
                    case Interrupt:
                        if(processes[current_process] == Blocked) {
                            processes[current_process] = Ready;
                        }
                        else {
                            processes[current_process] = ReadySuspend;
                        }
                        break;
                    case Terminated:
                        processes[current_process] = Completed;
                        if (do_part_2 == 1) {
                            for(int i=0;i<20;i++) {
                                if(processes[i] == ReadySuspend || processes[i] == BlockedSuspend) {
                                    if (processes[i] == ReadySuspend) {
                                        processes[i] = Ready;
                                    }
                                    else {
                                        processes[i] = Blocked;
                                    }
                                    break;
                                }
                            }
                        }
                        break;
                    case Created:
                        processes[current_process] = New;
                        break;
                    case TimeExpires:
                        processes[current_process] = Ready;
                        break;
                
                    default:
                        printf("Couldn't figure out what the case was for %s\n",sub_line);
                        break;
                }
            }
            //Part 2: (TODO) swap out a single process when all processes are either blocked or new
            //end of a line here
            if(do_part_2 == 1) {
                int need_to_swap_out = 1;
                for(int i=0;i<20;i++) {
                    if (processes[i] != Blocked && processes[i] != New && processes[i] != NotExist) {
                        need_to_swap_out = 0;
                    }
                }
                if(need_to_swap_out) {
                    for(int i=0;i<20;i++) {
                        if (processes[i] == Blocked) {
                            processes[i] = BlockedSuspend;
                            break;
                        }
                    }
                    for (int i=0;i<20;i++) {
                        if(processes[i]==New) {
                            processes[i] = Ready;
                            break;
                        }
                    }
                }
            }
            printf("States: ");
            print_states();
        }
        fclose(fp);
    }
    return 69;
}