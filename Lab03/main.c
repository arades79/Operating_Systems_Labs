#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

float f(float x) {
    return exp(-x*x/2.0)/sqrt(2*M_PI);
}
int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("You must call this function with arguments of leftbound, rightbound, and num_samples.\n");
        return 1;
    }


    int task_id, num_tasks;
    
    MPI_Status status;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&task_id);
    MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
    
    
    
    //argv[0] is just a.exe or whatever
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int num_samples = atoi(argv[3]);
    float width = (float)(b-a)/(float)num_samples;
    float sample_points[num_samples];
    float sample_calculated[num_samples];
    int num_workers = num_tasks-1;
    int num_tasks_in_worker[num_workers]; //Add one to index to get worker num
    int *worker_points[num_workers];
    for(int i=0;i<num_workers;i++) {
        num_tasks_in_worker[i] = 0;
    }
    for(int i=0;i<num_samples;i++) {
        sample_points[i] = a + width*i;
        num_tasks_in_worker[i%num_workers]++;
    }
    int tasks_index = 0;
    for(int i=0;i<num_workers;i++){
        int worker_points_individual[num_tasks_in_worker[i]];
        worker_points[i] = worker_points_individual;
        for(int j=0;j<num_tasks_in_worker[i];j++) {
            worker_points[i][j] = sample_points[tasks_index];
            tasks_index++;
        }
    }
    
    
    if (task_id == MASTER) {


        for(int i=0;i<num_workers;i++) {
            MPI_Send(&worker_points[i],num_tasks_in_worker[i],MPI_FLOAT,i+1,FROM_MASTER,MPI_COMM_WORLD);
        }
        float sum = 0;
        for(int i=0;i<num_workers;i++) {
            int *recieved_vals;
            MPI_Recv(&recieved_vals,num_tasks_in_worker[i],MPI_FLOAT,i+1,FROM_WORKER,MPI_COMM_WORLD,&status);
            for(int j=0;j<num_tasks_in_worker[i];j++) {
                sum += recieved_vals[j];
            }
        }
        float integral_result = sum*width;
        printf("Integral result: %.4f",integral_result);
    }
    else {
        float *points_to_calculate;
        int len_points = num_tasks_in_worker[task_id-1];
        MPI_Recv(&points_to_calculate,len_points,MPI_FLOAT,MASTER,FROM_MASTER,MPI_COMM_WORLD,&status);
        float result_values[len_points];
        for(int i=0;i<len_points;i++) {
            result_values[i] = f(points_to_calculate[i]);
        }
        MPI_Send(&result_values,len_points,MPI_FLOAT,MASTER,FROM_WORKER,MPI_COMM_WORLD);
    }
    
    
    
    
    MPI_Finalize();
    return 0;
}