#!/bin/sh
#SBATCH --partition debug --nodes 16 --time 10
srun --ntasks 1024 --overcommit -o ticks_32.log ./pp --synch=3 --end=32.0 --batch=4 --gvt-interval=256
