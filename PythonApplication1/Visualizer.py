import glob 
import os
from pickle import TRUE
import matplotlib.pyplot as plt
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
    filename = '%s' % filename 
    print(filename)

name = input("Enter name of the data file for visualization: ")
planet_name = name

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
    enet = [float(line.split()[19]) for line in lines]

with open('../Engineer_Thesis/Simulation_History/Planets/%s_Planets.txt' % name) as f:
    lines = f.readlines()[1: ]
    names = [line.split()[0] for line in lines]
    orbits = [int(line.split()[1]) for line in lines]
    masses = [float(line.split()[2]) for line in lines]
    radiuses = [float(line.split()[3]) for line in lines]   
    pposx = [float(line.split()[4]) for line in lines]
    pposy = [float(line.split()[5]) for line in lines]
    pposz = [float(line.split()[6]) for line in lines]

#append method to the name
name += "_" + Method(meth[0]).name

print('Choose visulization mode: ')
print('0. Run Simulation only Stationary Planets: ')
print('1. Run Simulation with Orbitting Planets:')
mode = int(input("2. Plot Graphs: "))

if mode == 0:
    i = -1
    scene2 = canvas(width=1280, height=800, center=vector(0,0,0), background=color.black)
    scalefactor = 10000

    for j in range(len(names)):
        if names[j].lower() == "earth":
            earth = sphere(pos=vector(pposx[j],pposy[j],pposz[j]),radius=radiuses[j], texture=textures.earth)
        elif names[j].lower() == "moon":
            moon = sphere(pos=vector(pposx[j],pposy[j],pposz[j]),radius=radiuses[j], texture='https://vignette.wikia.nocookie.net/future/images/e/e9/Moon_map_mercator.jpg',)
        else:
            sphere(pos=vector(pposx[j],pposy[j],pposz[j]),radius=radiuses[j])
    rad = 0.03 * radiuses[0]
    ship=sphere(pos=vector(posx[0] + rad,posy[0] + rad,posz[0] + rad), radius=rad, make_trail=True)
    
    for t in time:
        i+=1
        rate(1000)
        ship.pos=vector(posx[i] + rad, posy[i] + rad, posz[i] + rad)

elif mode == 1:
    i = -1
    scene2 = canvas(width=1280, height=800, center=vector(385000000.0,0,0), background=color.black)
    scalefactor = 100000

    for j in range(len(names)):
         if orbits[j] == 1:
            planet_name += "_%s.txt" % names[j]
            with open('../Engineer_Thesis/Simulation_History/Planets/Orbits/%s' % planet_name ) as f:
                lines = f.readlines()[1: ]
                orbposx = [float(line.split()[0]) for line in lines]
                orbposz = [float(line.split()[1]) for line in lines]
            moon = sphere(pos=vector(orbposx[0],0,orbposz[0]),radius=radiuses[j], texture='https://vignette.wikia.nocookie.net/future/images/e/e9/Moon_map_mercator.jpg')
         else:
            earth = sphere(pos=vector(pposx[j],pposy[j],pposz[j]),radius=radiuses[j], texture=textures.earth) 
    
    rad = 0.02 * radiuses[0]
    ship=sphere(pos=vector(posx[0] + rad,posy[0] + rad,posz[0] + rad), radius=rad, make_trail=True)
    
    for t in time:
        i+=1
        rate(4000)
        ship.pos=vector(posx[i] + rad, posy[i] + rad, posz[i] + rad)
        moon.pos=vector(orbposx[i], 0, orbposz[i])

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
    plt.plot(time, enet, label="Total")
    plt.title("Energy in time plot")
    plt.xlabel("Time[s]")
    plt.ylabel("Energy[J]")
    plt.legend()

    plt.suptitle("%s Data plots" % name)
    plt.savefig('../Engineer_Thesis/Plots/%s.png' % name)
    plt.show()
    