#!/bin/sh
#SBATCH --partition small --nodes 64 --time 30 
srun --ntasks 4096 --overcommit -o ranks_4096.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
