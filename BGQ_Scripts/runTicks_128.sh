#!/bin/sh
#SBATCH --partition debug --nodes 16 --time 1
srun --ntasks 1024 --overcommit -o ticks_128.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
