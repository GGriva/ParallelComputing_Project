# ParallelComputing_Project

Run the project on kratos as 
  "mpirun -np 4 ./pp --synch=3 --end=32.0 --batch=4 --gvt-interval=256"
  
Run the project on Blue Gene Q using the following in a <filename>.job file where < > 
should be changed with appropriate name:
  
#!/bin/sh
#SBATCH --partition <partition name> --nodes <num of nodes>  --time 30
  
srun --ntasks <num of tasks> --overcommit -o <outFile>.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
