#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


typedef enum pstate
{
    Ready,
    Running,
    Blocked,
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
    "inputs/imp1.txt",
    "inputs/imp2.txt",
    "inputs/imp3.txt",
    "inputs/imp4.txt"
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
    "Ready",
    "Running",
    "Blocked"
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
    if (strstr(s,"interrupt occured")) return Interrupt;
    if (strstr(s,"terminated")) return Terminated;
    if (strstr(s,"is created")) return Created;
    if (strstr(s,"slice expires")) return TimeExpires;
    printf("Got error with input %s",s);
    return Dispatched;
}

pstate get_initial_state(char * s)
{
    if (strstr(s, "Ready")) return Ready;
    if (strstr(s, "Running")) return Running;
    if (strstr(s, "Blocked")) return Blocked;
    printf("invalid state %s", s);
    return -1;
}

int get_pid(char* s) 
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

int main() 
{
    FILE *fp;
    char *current_line;
    int line_size;
    size_t buffer_size;
    
    for (size_t file_num = 0; file_num < 4; ++file_num)
    {
        printf("Simulation %i begin:\n", file_num);
        fp = fopen(file_paths[file_num],"r");
        line_size = getline(&current_line,&buffer_size,fp);
        printf("Initial state\n%s\n",current_line);
        while ( get_pid(current_line) != -1)
        {
            // we need the last index of the state that was just parsed, and to increment the pointer by that ammount
            // this can probably be a seperate function that does cool recursion
        }
        // split string on ';' for individual commands to feed into parser
        while ((line_size = getline(&current_line, &buffer_size, fp)) != -1)
        {
            fgets(current_line,500,fp);
            action next_action = get_action(current_line);
            int current_process = processes[get_pid(current_line)];

            switch (next_action)
            {
                case IORequest:
                    iodevice next_device = get_io_device(current_line);
                    printf("")
                    break;
            
                default:
                    break;
            }
        } 
        fclose(fp);
    }
    return 69;
}