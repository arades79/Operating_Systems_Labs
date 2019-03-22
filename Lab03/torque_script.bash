#!/bin/bash
#PBS -S /bin/bash
#PBS -o pbs_data_out.dat
#PBS -j oe
#PBS -l nodes=1:ppn=40
#PBS -M pittss@wit.edu
#PBS -m be
cd $PBS_O_WORKDIR
mpirun -np 10 -machinefile $PBS_NODEFILE ./a.out
