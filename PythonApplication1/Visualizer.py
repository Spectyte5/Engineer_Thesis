from site import makepath
import matplotlib.pyplot as plt
import glob 
import os
from vpython import *
from enum import Enum

class Method(Enum):
    Adams_Bashford = 0
    Euler = 1
    Midpoint = 2
    Runge_Kutta_IV = 3

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
    ekin = [float(line.split()[16]) for line in lines]
    epot = [float(line.split()[17]) for line in lines]
    meth = [int(line.split()[18]) for line in lines]


#append method to the name
name += "_" + Method(meth[0]).name

mode = int(input("Choose 1 to Plot Graphs, 0 to Run Simulation: "))

if mode == 0:
    i = -1;
    scale = 10000.0;
    rad = 50.0;
    planet=sphere(pos=vector(0,0,0),radius=6371000.0/scale, texture=textures.earth)
    ship=sphere(pos=vector(posx[0]/scale + rad,posy[0]/scale + rad,posz[0]/scale + rad), radius=rad, make_trail=True)
    
    for t in time:
        i+=1
        rate(1000)
        ship.pos=vector(posx[i]/scale + rad, posy[i]/scale + rad, posz[i]/scale + rad)

else:
    #Mass and fuel
    plt.subplot(2, 3, 1)
    plt.plot(time, mass, label="mass")
    plt.title("Mass in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Mass[kg]")
    plt.legend()

    #Position
    plt.subplot(2, 3, 2)
    plt.plot(time, posx, label="x")
    plt.plot(time, posy, label="y")
    plt.plot(time, posz, label="z")
    plt.title("Position in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Position[m]")
    plt.legend()

    #Velocity
    plt.subplot(2, 3, 3)
    plt.plot(time, velx, label="x")
    plt.plot(time, vely, label="y")
    plt.plot(time, velz, label="z")
    plt.title("Velocity in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Velocity[m/s]")
    plt.legend()

    #Engine force
    plt.subplot(2, 3, 4)
    plt.plot(time, engx, label="x")
    plt.plot(time, engy, label="y")
    plt.plot(time, engz, label="z")
    plt.title("Engine Force in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Force[N]")
    plt.legend()

    #Force
    plt.subplot(2, 3, 5)
    plt.plot(time, forx, label="x")
    plt.plot(time, fory, label="y")
    plt.plot(time, forz, label="z")
    plt.title("Force in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Force[N]")
    plt.legend()

    #Energy
    plt.subplot(2, 3, 6)
    plt.plot(time, ekin, label="Kinetic")
    plt.plot(time, epot, label="Potential")
    plt.title("Energy in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Energy[J]")
    plt.legend()

    plt.suptitle("%s Data plots" % name)
    plt.savefig('../Engineer_Thesis/Plots/%s.png' % name)
    plt.show()
    