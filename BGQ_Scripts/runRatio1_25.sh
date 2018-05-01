#!/bin/sh
#SBATCH --partition small --nodes 16 --time 30 
srun --ntasks 1024 --overcommit -o ratio1_25.log ./pp --synch=3 --end=256.0 --batch=4 --gvt-interval=256
