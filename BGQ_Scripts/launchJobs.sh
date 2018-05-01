#!/bin/sh

function launch()
{
	
	iRank="$1"
	iNode="$2"
	ipart="$3"
		
	cat > tmp.job <<EOF
#!/bin/sh
#SBATCH --partition $ipart  --nodes $iNode  --time 30 
srun --ntasks ${iRank}  --overcommit -o ${iNode}.log /gpfs/u/home/PCP7/PCP7adhb/scratch/Final/ROSS-build/models/ROSS-Models/PCFinal/pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256.0 
EOF
	sbatch tmp.job
}

launch 256 4 debug 
exit 0
launch 1024 16 debug
launch 2048 32 small
launch 4096 64 small
launch 8192 128 medium
launch 16384 256 medium
launch 32768 512 large






