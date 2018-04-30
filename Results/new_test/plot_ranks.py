import matplotlib.pyplot as plt
import sys

f_name = sys.argv[1]

x = []
y = []
z = []
for line in open(f_name):

	sep = line.split(",")

	x.append(float(sep[0]))
	y.append(float(sep[1]))
	z.append(float(sep[2]))

#MPI Rank plots
#plt.ylabel("Event Rate (Events per second)")
#plt.ylabel("Execution Time (seconds)")
#plt.ylabel("Event Rollbacks")
#plt.ylabel("Total Rollbacks")
#plt.ylabel("Percent Efficiency (%)")
#plt.xlabel("MPI ranks")

#Tick Tests
plt.ylabel("Average Per Cell")
#plt.ylabel("Execution Time (seconds)")
plt.xlabel("Number of Ticks")

plt.xticks(x,map(int,x))

#plt.plot(x, y, "-o")
plt.plot(x, y, "-o", label="Predator")
plt.plot(x, z, "-o", label="Prey")
plt.legend()

plt.show()



