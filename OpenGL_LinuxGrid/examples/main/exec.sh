#!/bin/bash

gcc -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -c lightlab.c quad.c

gcc -lglut -lGL -lGLEW -lGLU -lpthread -lX11 -o final lightlab.o

#(exec wmctrl -k on)

#cd JavaSocketClient/ && exec xterm cd JavaSocketClient/ | javac Form2.java && exec xterm cd /home/gil/Desktop/OpenGL_Linux/JavaSocketClient | java Form2 &)

#./final
