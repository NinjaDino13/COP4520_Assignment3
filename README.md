# COP4520_Assignment3

## Problem 1: The Birthday Presents Party
### Compilation Instructions for Solution
To run the program, open up a command prompt window. From there enter:
```
g++ Assignment3_1.cpp -o Assignment3_1.exe
```
This will compile the code and create the .exe file for it, named Assignment1.exe. Once this has been created, enter the following in to the command line:
```
./Assignment3_1.exe
```
This will run the code within the command prompt.

## Problem 2: Atmospheric Temperature Reading Module (50 points)

You are tasked with the design of the module responsible for measuring the atmospheric temperature of the next generation Mars Rover, equipped with a multi-core CPU and 8 temperature sensors. The sensors are responsible for collecting temperature readings at regular intervals and storing them in shared memory space. The atmospheric temperature module has to compile a report at the end of every hour, comprising the top 5 highest temperatures recorded for that hour, the top 5 lowest temperatures recorded for that hour, and the 10-minute interval of time when the largest temperature difference was observed. The data storage and retrieval of the shared memory region must be carefully handled, as we do not want to delay a sensor and miss the interval of time when it is supposed to conduct temperature reading. 

Design and implement a solution using 8 threads that will offer a solution for this task. Assume that the temperature readings are taken every 1 minute. In your solution, simulate the operation of the temperature reading sensor by generating a random number from -100F to 70F at every reading. In your report, discuss the efficiency, correctness, and progress guarantee of your program.