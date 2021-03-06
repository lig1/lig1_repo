#!/bin/bash

g++ -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -c readParse.cpp display.cpp reshape.cpp grid.cpp -pthread

g++ -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -o final readParse.o display.o reshape.o grid.o -pthread

#(exec wmctrl -k on)

(cd /gil/OpenGL_Linux/JavaSocketClient/ && exec xterm cd /gil/OpenGL_Linux/JavaSocketClient/ | javac Form2.java && exec xterm cd /gil/OpenGL_Linux/JavaSocketClient/ | java Form2 &)

./final

# git push https://github.com/lig1/lig1_repo.git master
