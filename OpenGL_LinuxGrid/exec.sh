#!/bin/bash

g++ -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -c grid.cpp -pthread

g++ -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -o final grid.o -pthread

#(exec wmctrl -k on)

#cd JavaSocketClient/ && exec xterm cd JavaSocketClient/ | javac Form2.java && exec xterm cd /home/gil/Desktop/OpenGL_Linux/JavaSocketClient | java Form2 &)

#./final