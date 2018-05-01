#!/bin/sh
#SBATCH --partition small --nodes 4 --time 30 
srun --ntasks 256 --overcommit -o ranks_256.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
