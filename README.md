# Breakout
2D brick breaker game made with C++. Uses the Standard library and SFML library.
Visual Studio 2015 was used to make this project.
This project is dynamically linked with SFML.
SFML version 2.5.1 was used. 
Download SFML here: https://www.sfml-dev.org/download.php
Instructions on how to set up SFML are here: https://www.sfml-dev.org/tutorials/2.5/


For those who don't want to download SFML and follow its install tutorial; read below.


A commit on Feb 13 2019 added the SFML files into the project folder and adjusted dependency paths with macros rather than a specific path to allow proper pathing on different machines.


Set Solution platform to x86

The files in Breakout\Dependencies\bin files need to be copied into the debug folder of the project. If they are not then when you attempt to run the program with the debugger, the debugger will give an error saying sfml-system-d-2.dll was not found.
