# General Information

#### C++ library for numerical calculations related to basic spacecraft motion issues, taking into account influence of gravity and vehicle propulsion.

## Instalation
To use the library you need to:
1. Clone repository using:
        git clone https://github.com/Spectyte5/Engineer_Thesis
2. Make sure your compiler "sees" the libraries neccessary (which are located in the external folder):
- json for modern C++: https://github.com/tristanpenman/valijson
- valijson: https://github.com/nlohmann/json

## Operation
There are two options of using the library:
1. Provide a command line argument of type string, which is a path to your *.json* file for simulation. (examplary *.json* file can be found lower in this document)
*example:*
     
    .\Engineer_Thesis.cpp "./JSON_files/Sim1.json"

2. Run the code with your compiler of choice and then choose if you want to create a new simulation or load a *.json* from this directory:
        
   
    <Library directory>/JSON_files/

## Units
Units used are SI units:
- mass in kilograms [kg]
- position in meters [m]
- velocity in meters per second [m/s]
- force in Newtons [N]
- energy in Joule [J]
- angle in radians [rad]
- angular velocity in radians per second [rad/s]

## json file
*.json* file has one object with 4 seperate parts:
### control 
- **starttime** - *array* of three double type elemetents, time when engine will be turned on for x, y, z axis, allowing user to set each one independently.
- **endtime** - *array* of three double type elemetents, time when engine will be turned off for x, y, z axis, allowing user to set each one independently.
- **force** - *array* of three double type elemetents, magnitude and the direction of engine thrust for x, y, z axis, allowing user to set each one independently.
### data
- **ode** - integer type *number* meaning which ODE solving method should be used:
0. Adams-Bashford
1. Euler
2. Midpoint
3. Runge-Kutta IV
- **step** - double type *number* equal to timesteps used for simulation. 
- **time** - double type *number*, the final time of simulation.
### planets 
- **name** - *string* type, name of the planet
- **mass** - double type *number*, mass of the planet
- **radius** - double type *number*, radius of the planet
- **orbit** - *boolean* type , is orbitting or strationary [true/false] 
For orbitting Planets (orbit = true):
- **start_angle** - double type *number*, phase used for orbital motion (start angle)
- **orbit_radius** - double type *number*, radius of the orbit
- **ang_velocity** - double type *number*, constant angular velocity of the planet 
- **orbit_pos** - *array* of three double type elemetents, position of the center of the orbit (x,y,z)
 **NOTE:** only x,z are taken into account y value is always 0
For stationary Planets (orbit = false):
- **position** - *array* of three double type elemetents, magnitude and the direction of position vector (x,y,z)
### ship
- **fuel** - double type *number*, fuel mass of the ship 
- **fuel_usage** - double type *number*, constant ammount of fuel used when engines are turned on 
- **mass** - double type *number*, total mass of the ship
- **name** - *string* type, name of the ship
- **position** - *array* of three double type elemetents, magnitude and the direction of position vector (x,y,z)
- **velocity** - *array* of three double type elemetents, magnitude and the direction of velocity vector (x,y,z)


## Example of Json file (Ship on the Earth orbit with a constant velocity):
```json
{
  "control": [
    {
      "endtime": [
        0.0,
        0.0,
        0.0
      ],
      "force": [
        0.0,
        0.0,
        0.0
      ],
      "starttime": [
        0.0,
        0.0,
        0.0
      ]
    }
  ],
  "data": {
    "ode": 3,
    "step": 1,
    "time": 42500
  },
  "planets": [
    {
      "name": "Earth",
      "mass": 5.972e24,
      "radius": 6.378e6,
      "orbit": false,
      "position": [
        0.0,
        0.0,
        0.0
      ]
    }
  ],
  "ship": {
    "fuel": 0.0,
    "fuel_usage": 0.0,
    "mass": 3000.0,
    "name": "Sim1",
    "position": [
      12742000.0,
      0.0,
      0.0
    ],
    "velocity": [
      0.0,
      5592.27,
      0.0
    ]
  }
}
```