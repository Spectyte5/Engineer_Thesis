import matplotlib.pyplot as plt
import glob 
import os

print('Data files available for Visualization:')
path = r'../Engineer_Thesis/Simulation_History/*.txt'
files = glob.glob(path)
for f in files:
    filename = os.path.basename(f) 
    filename = filename.replace('.txt','')
    print(filename)

name = input("Enter name of the data file for visualization: ")

with open('../Engineer_Thesis/Simulation_History/%s.txt' % name) as f:
    lines = f.readlines()
    time = [float(line.split()[0]) for line in lines]
    mass = [float(line.split()[1]) for line in lines]
    fuel = [float(line.split()[2]) for line in lines]   
    posx = [float(line.split()[3]) for line in lines]
    posy = [float(line.split()[4]) for line in lines]
    posz = [float(line.split()[5]) for line in lines]
    velx = [float(line.split()[6]) for line in lines]
    vely = [float(line.split()[7]) for line in lines]
    velz = [float(line.split()[8]) for line in lines]
    forx = [float(line.split()[9]) for line in lines]
    fory = [float(line.split()[10]) for line in lines]
    forz = [float(line.split()[11]) for line in lines]

#Mass and fuel
plt.subplot(2, 2, 1)
plt.plot(time, mass, label="mass")
plt.plot(time, fuel, label="fuel")
plt.title("Mass in time plot")
plt.xlabel("Time [s]")
plt.ylabel("Mass [kg]")
plt.legend()

#Position
plt.subplot(2, 2, 2)
plt.plot(time, posx, label="x")
plt.plot(time, posy, label="y")
plt.plot(time, posz, label="z")
plt.title("Position in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Position [m]")
plt.legend()

#Velocity
plt.subplot(2, 2, 3)
plt.plot(time, velx, label="x")
plt.plot(time, vely, label="y")
plt.plot(time, velz, label="z")
plt.title("Velocity in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Velocity [m/s]")
plt.legend()

#Force
plt.subplot(2, 2, 4)
plt.plot(time, forx, label="x")
plt.plot(time, fory, label="y")
plt.plot(time, forz, label="z")
plt.title("Force in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Force [N]")
plt.legend()

plt.suptitle("%s Data plots" % name)
plt.savefig('../Engineer_Thesis/Plots/%s.png' % name)
plt.show()
    