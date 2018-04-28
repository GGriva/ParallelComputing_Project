import matplotlib.pyplot as plt
import sys

f_name = sys.argv[1]

x = []
y = []
for line in open(f_name):

	sep = line.split(",")

	x.append(float(sep[0]))
	y.append(float(sep[1]))

#plt.ylabel("Event Rate (Events per second)")
#plt.ylabel("Execution Time (seconds)")
#plt.ylabel("Event Rollbacks")
plt.ylabel("Percent Efficiency (%)")
plt.xlabel("MPI ranks")

plt.plot(x, y, "-o")

plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.show()



