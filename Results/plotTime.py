#!/usr/bin/python
'''
Adela Habib 
PCP final project plot scrip
Date: 01/May/2018

For each test part un-comment the section for the plots
run as ./plotTime.py
'''
import matplotlib.pyplot as plt
import numpy as np

#---------uni and rank test-------------
'''
data512 = np.loadtxt("512RankTest.txt", skiprows=0)
data1024 = np.loadtxt("1024RankTest.txt", skiprows=0, delimiter=",")
data2048 = np.loadtxt("2048RankTest.txt", skiprows=0)

plt.figure(1)
plt.plot(data512[:,0], data512[:, 1], 'o-', color = "blue",mec = "blue", label="512 x 512 LPs")
plt.plot(data1024[:,0], data1024[:, 1], 'o-', color = "red",mec = "red", label="1024 x 1024 LPs")
plt.plot(data2048[:,0], data2048[:, 1], 'o-', color = "green",mec = "green", label="2048 x 2048 LPs")
plt.xlabel("MPI ranks")
plt.xticks([256, 1024, 2048, 4096, 8192])
plt.ylabel("Event rate [events/sec]")
plt.legend(loc = 'upper left', frameon=False)
plt.savefig("ranks_event_rate.png")
plt.figure(2)
plt.plot(data512[:,0], data512[:, 2], 'o-', color = "skyblue",mec = "skyblue", label="512 x 512 LPs")
plt.plot(data1024[:,0], data1024[:, 2], 'o-', color = "darkblue",mec = "darkblue", label="1024 x 1024 LPs")
plt.plot(data2048[:,0], data2048[:, 2], 'o-', color = "black",mec = "black", label="2048 x 2048 LP")
plt.xlabel("MPI ranks")
plt.xticks([256, 1024, 2048, 4096, 8192])
plt.ylabel("Execution time [seconds]")
plt.legend(loc = 'upper right', frameon=False)
plt.savefig("ranks_exe_rate.png")
plt.show()
'''
#-------Universe scaling test---------------
'''
#1 total KPs, number of ranks constant
data = np.loadtxt("UniTest.txt", skiprows=0)
plt.figure(1)
plt.plot((data[:,0]**2), data[:, 1], 'o-', color = "blue",mec = "blue")
plt.xlabel(r"Number of LPs")
plt.ylabel("Event rate [events/seconds]")
plt.savefig("LPsvsEventRate.png")
plt.figure(2)
plt.plot(np.log2(data[:,0]**2), data[:, 2], 'o-', color = "purple",mec = "purple")
plt.xlabel(r"log${_2}$(Number of LPs)")
plt.ylabel("Execution time[seconds]")
plt.savefig("LogExecvsLps.png")

#when KP per PE is constant
uniScaletime = np.loadtxt("uniTestKPPE.txt")
plt.figure(3)
plt.plot(uniScaletime[:,0], uniScaletime[:, 1], '*-', color = "orange")
plt.xticks([256, 512, 1024, 2048]) #, ['65k', '262k', '1048k', '4194k'], fontsize='small')
plt.yticks(np.linspace(0, 8e08, 8), ['1e08', '2e08', '3e08', '4e08', '5e08', '6e08', '7e08', '8e08'])
plt.xlabel(r"$\sqrt{\mathrm{Number of LPs}}$")
plt.ylabel("Event rate [events/seconds]")
plt.savefig("EventRatevsUnidimKPperPEfixed.png")
plt.show()
'''
#----------KP test---------------------
'''
data = np.loadtxt("KPTest.txt", skiprows=0)
plt.figure(1)
plt.plot((data[:,0]**2), data[:, 1], 'o-', color = "red",mec = "red")
plt.xlabel(r"Number of KPs")
plt.ylabel("Event rate [events/seconds]")
plt.savefig("KPvsEventRate.png")
plt.figure(2)
print data[:,0]**2
plt.plot((data[:,0]**2), data[:, 2], '*-', color = "green", mec="green", 
	label="Total roll backs")
plt.plot((data[:,0]**2), data[:, 3], '*-', color = "orange", mec="orange", 
        label="Primary roll backs")
plt.xlabel(r"Number of KPs")
plt.ylabel("Roll Backs")
plt.legend(loc = 'lower right', frameon =False)
plt.savefig("KPvsRollBackEvent.png")
plt.show()
'''
#---------prey and predator ratio test------------
'''
data = np.loadtxt("preyPredTest.txt", skiprows=0)
plt.plot(data[:,0], data[:, 1]/data[:,2], 'o-', color = "darkred", mec = "darkred")
plt.ylim(2.96,2.98)
print np.arange(0, 2.25, 0.25)
plt.xticks(np.arange(0, 2.5, 0.25))
plt.xlabel(r"Initial Prey/Predator")
plt.ylabel("Final Prey/Predator")
plt.savefig("PreyPredator.png")
plt.show()
'''

#--------------GVTsbatch test-------------------
'''
data = np.loadtxt("GVTsbatchTest.txt", skiprows=0)
plt.plot(data[:,0], data[:,1],'+-', color = "salmon", mec = "salmon")
plt.xlabel(r"sbatch$\times$GVT$_{Interval}$")
plt.ylabel("Execution time [seconds]")
plt.show()
'''
