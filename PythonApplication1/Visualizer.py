import matplotlib.pyplot as plt
import glob 
import os

print('Data files available for Visualization:')
path = r'../Engineer_Thesis/Simulation_History/Ships/*.txt'
files = glob.glob(path)

for f in files:
    filename = os.path.basename(f) 
    filename = filename.replace('.txt','')
    filename = '"%s"' % filename 
    print(filename)

name = input("Enter name of the data file for visualization: ")

with open('../Engineer_Thesis/Simulation_History/Ships/%s.txt' % name) as f:
    lines = f.readlines()[1: ]
    time = [float(line.split()[0]) for line in lines]
    mass = [float(line.split()[1]) for line in lines]
    fuel = [float(line.split()[2]) for line in lines]   
    fuse = [float(line.split()[3]) for line in lines]   
    posx = [float(line.split()[4]) for line in lines]
    posy = [float(line.split()[5]) for line in lines]
    posz = [float(line.split()[6]) for line in lines]
    velx = [float(line.split()[7]) for line in lines]
    vely = [float(line.split()[8]) for line in lines]
    velz = [float(line.split()[9]) for line in lines]
    engx = [float(line.split()[10]) for line in lines]
    engy = [float(line.split()[11]) for line in lines]
    engz = [float(line.split()[12]) for line in lines]
    forx = [float(line.split()[13]) for line in lines]
    fory = [float(line.split()[14]) for line in lines]
    forz = [float(line.split()[15]) for line in lines]
    kinx = [float(line.split()[16]) for line in lines]
    kiny = [float(line.split()[17]) for line in lines]
    kinz = [float(line.split()[18]) for line in lines]
    potx = [float(line.split()[19]) for line in lines]
    poty = [float(line.split()[20]) for line in lines]
    potz = [float(line.split()[21]) for line in lines]

#Mass and fuel
plt.subplot(2, 4, 1)
plt.plot(time, mass, label="mass")
plt.plot(time, fuel, label="fuel")
plt.title("Mass in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Mass[kg]")
plt.legend()

#Position
plt.subplot(2, 4, 2)
plt.plot(time, posx, label="x")
plt.plot(time, posy, label="y")
plt.plot(time, posz, label="z")
plt.title("Position in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Position[m]")
plt.legend()

#Velocity
plt.subplot(2, 4, 3)
plt.plot(time, velx, label="x")
plt.plot(time, vely, label="y")
plt.plot(time, velz, label="z")
plt.title("Velocity in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Velocity[m/s]")
plt.legend()

#Engine force
plt.subplot(2, 4, 4)
plt.plot(time, engx, label="x")
plt.plot(time, engy, label="y")
plt.plot(time, engz, label="z")
plt.title("Engine Force in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Force[N]")
plt.legend()

#Force
plt.subplot(2, 4, 5)
plt.plot(time, forx, label="x")
plt.plot(time, fory, label="y")
plt.plot(time, forz, label="z")
plt.title("Force in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Force[N]")
plt.legend()

#Kinetic Energy
plt.subplot(2, 4, 6)
plt.plot(time, kinx, label="x")
plt.plot(time, kiny, label="y")
plt.plot(time, kinz, label="z")
plt.title("Kinetic Energy in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Energy[J]")
plt.legend()

#Potential Energy
plt.subplot(2, 4, 7)
plt.plot(time, kinx, label="x")
plt.plot(time, kiny, label="y")
plt.plot(time, kinz, label="z")
plt.title("Potential Energy in time plot")
plt.xlabel("Time[s]")
plt.ylabel("Energy[J]")
plt.legend()

plt.suptitle("%s Data plots" % name)
plt.savefig('../Engineer_Thesis/Plots/%s.png' % name)
plt.show()
    