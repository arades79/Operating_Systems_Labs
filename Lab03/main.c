#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

#define PI 3.14159265358979323846

float f(float x) {
    return exp(-x*x/2.0)/sqrt(2*PI);
}



float calculate_from(int a, int b, int num_samples) {
    int task_id, num_tasks;
    
    MPI_Status status;
    
    MPI_Comm_rank(MPI_COMM_WORLD,&task_id);
    MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
    
    
    
    //argv[0] is just a.exe or whatever

    float width = (float)(b-a)/(float)num_samples;
    float sample_points[num_samples];
    float sample_calculated[num_samples];
    int num_workers = num_tasks-1;
    int num_tasks_in_worker[num_workers]; //Add one to index to get worker num
    for(int i=0;i<num_workers;i++) {
        num_tasks_in_worker[i] = 0;
    }
    for(int i=0;i<num_samples;i++) {
        sample_points[i] = a + width*i;
        num_tasks_in_worker[i%num_workers]++;
    }
    
    
    if (task_id == MASTER) {
        for(int i=0;i<num_samples;i++) {
            float thing_to_send = sample_points[i];
            MPI_Send(&thing_to_send,1,MPI_FLOAT,(i%num_workers)+1,FROM_MASTER,MPI_COMM_WORLD);
        }
        float sum = 0;
        for(int i=0;i<num_samples;i++) {
            float obtained_value;
            MPI_Recv(&obtained_value,1,MPI_FLOAT,(i%num_workers)+1,FROM_WORKER,MPI_COMM_WORLD,&status);
            sum += obtained_value;
        }
        float integral_result = sum*width;
        return integral_result;
    }
    else {
        int len_points = num_tasks_in_worker[task_id-1];
        float point_to_calculate;
        float calculated;
        for(int i=0;i<len_points;i++) {
            MPI_Recv(&point_to_calculate,1,MPI_FLOAT,MASTER,FROM_MASTER,MPI_COMM_WORLD,&status);
            calculated = f(point_to_calculate);
            MPI_Send(&calculated,1,MPI_FLOAT,MASTER,FROM_WORKER,MPI_COMM_WORLD);
        }
        return 0;
    }
}



int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    int task_id, num_tasks;
    MPI_Comm_rank(MPI_COMM_WORLD,&task_id);
    MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
    if(argc == 1) {
        //test for best num samples
        int a = 1;
        int b = 100;
        FILE *fp;
        fp = fopen("num_samples_data.txt","w+");
        for(int i=1;i<50;i++) {
            int num_samples = i*100;
            float result = calculate_from(a,b,num_samples);
            if (task_id == MASTER) {
                fprintf(fp,"%d %f\n",num_samples,result);
            }
        }
    }
    else if(argc == 4) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        int num_samples = atoi(argv[3]);
        float result = calculate_from(a,b,num_samples);
        if (task_id == MASTER) {
            printf("a %d, b %d, num_samples %d: %.4f\n",a,b,num_samples,result);
        }
    }
    else {
        printf("You must call this function with arguments of leftbound, rightbound, and num_samples.\n");
        return 1;
    }
    MPI_Finalize();
    return 0;
}