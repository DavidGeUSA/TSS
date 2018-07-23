# Open Source EM Field FDTD Software
The purpose of this software is for doing digital experiments on electromagnetic fields, such as simulations, statistics, FFT, 3D/2D drawings, etc.  It is not intended to be a software product; rather, it uses open source to provide a framework and tools for you to experiment your own ideas and coding.

For simulating EM fields, a simple plugin system is used to dynamically load following modules: FDTD algorithms, boundary conditions, initial field values, field sources, and total field/scattered field boundary.

By using this plugin system, you can experiment on different ideas and coding of these elements in a field simulation.
For the simulation system, each plugin module is represented by an abstract C++ class. You try your ideas and coding by deriving new classes from these abstract classes.

Sample plugin modules are part of this open source project. 
Following FDTD algorithms are implemented as FDTD algorithm plugin modules:
1. Standard Yee FDTD algorithm
2. Space synchronized Yee FDTD algorithm
3. Time-Space-Synchronized FDTD algorithm for homogeneous space
4. Tine-Space-Synchronized FDTD algorithm for inhomogeneous environments

You can review source code of these sample plugin modules to see how you can develop your own plugin modules. 

To get a rough idea of what the software does, read a test report at https://github.com/Limnor/TSS/blob/master/FDTD%20Test%20Results.pdf

To know more about this software, read software specifications at https://github.com/Limnor/TSS/blob/master/EM%20field%20Software%20Spec.pdf

# Time-Space-Synchronized FDTD Simulation
I am a retired software engineer. I like to play with computer coding. Among the coding I played is Schneider’s teaching book “Understanding the FDTD Method” (https://github.com/john-b-schneider/uFDTD).

One thing I found strange was that the field simulation results provided by an FDTD method could not be used to do calculations such as field energy transfer and field divergence. I did a quick search over the internet and did not find an answer. Since I am just playing with FDTD, not using a grant to do research, I did not bother to do a thorough research but simply came up with my owner solution. My solution is a new algorithm. To my surprise the new algorithm is better than the current FDTD in many major aspects, as explained below.
1.	It uses a non-staggered grid to give simulation results in a time and space synchronized manner. Therefore it is called a Time-Space-Synchronized FDTD algorithm (TSS FDTD, or in short, TSS). I believe it is the first algorithm that can do it. Please correct me if I am wrong. This feature is my sole motivation to develop this new algorithm. It makes it possible to calculate field divergence and energy transfer. If your project requires such calculations then my algorithm is a very good option.
2.	Its estimation order for time advancement is arbitrary. By “arbitrary” I mean that choosing estimation order does not affect memory usages and calculation algorithm. For example, choosing 4-th order and choosing 40-th order, same memory and programming code is used, just different numbers of iterations is used. I believe it is also the first algorithm that can do it. Increasing estimation order greatly increases simulation accuracy.
3.	The higher order of time advancement estimation is realized via getting higher order temporal derivatives from higher order space curls, NOT via time history of fields. I believe this is also the first algorithm doing time advancement estimation in this way. Using time history of fields CANNOT increase estimation accuracy because there is not new information involved. Using higher order space curls for higher order temporal derivatives can greatly enhance estimation accuracy because new information is utilized. 
4.	Its estimation order for space curl is arbitrary but limited to the number of space grid
5.	Its computational complexity with regard to the estimation order is linear. That means increasing a little calculation amount can increase accuracy greatly. Therefore, large step sizes can be used, which in turn reduces greatly the calculation amount. The combined effect is that the TSS algorithm is much faster and much more accurate than a traditional FDTD.
6.	A modular design is used for designing a simulation system. It separates field estimation algorithms from applying boundary conditions. Thus, it is very easy to apply different boundary conditions.
7.	It does not impose adverse restrictions, limitations, conditions, etc., to its applications comparing to traditional FDTD methods. For example, it can be used for inhomogeneous material just like a traditional FDTD does. That is, given a calculation task, if a traditional FDTD can be used then TSS algorithm can also be used.

Due to very high precision TSS can achieve with higher order estimations, large space steps can be used, which in turn greatly reduces simulation time. Therefore, comparing with standard Yee FDTD, TSS can achieve very high precision in very fast simulation speed.

Test data show that a 6-th order TSS simulation used about 1/8 of Yee FDTD simulation time and produced field data 10^18 times more accurate than field data produced by Yee FDTD. See test reports at https://github.com/Limnor/TSS/blob/master/FDTD%20Test%20Results.pdf

The test reports contains task files so that you can repeat the tests and verify the test results. The full source code is here at https://github.com/Limnor/TSS/tree/master/Source%20Code. You can compile it and run the tests shown in the test report.

For details about TSS, see https://github.com/Limnor/TSS/blob/master/TSS.pdf

You are very welcome to criticize the algorithm, to provide professional guidance, to contribute to the algorithm or to the coding. You can also take its basic ideas and merge it into your own projects, just show your courtesy by presenting a reference to their origin here.


