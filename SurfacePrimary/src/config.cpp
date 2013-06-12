#include "config.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#include <stdlib.h>
#include "displayMain.h"
#include <math.h>
#include <limits.h>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>

int windowWidth = 600;
int windowHeight = 400;

/************* LIGHTING ****************/
float ambientLight[4] =
	{0.2, 0.2, 0.2, 1.0};
float Lt0amb[4] =
	{0.1, 0.1, 0.1, 1.0};
float Lt0diff[4] =
	{0.6, 0.6, 0.6, 1.0};
float Lt0spec[4] =
	{1.0, 1.0, 1.0, 1.0};
float Lt0pos[4] =
	{1.0, 0.0, 1.0, 0.0}; // Directional light

float Lt1amb[4] =
	{0.1, 0.1, 0.1, 1.0};
float Lt1diff[4] =
	{0.6, 0.6, 0.6, 1.0};
float Lt1spec[4] =
	{1.0, 1.0, 1.0, 1.0};
float Lt1pos[4] =
	{0.0, 1.0, 1.0, 0.0}; // Directional light

/************* MATERIAL ****************/
float Noemit[4] =
	{0.0, 0.0, 0.0, 1.0};
float Matspec[4] =
	{1.0, 1.0, 1.0, 1.0};
float Matnonspec[4] =
	{0.8, 0.8, 0.8, 1.0}; //	{0.8, 0.05, 0.4, 1.0};
float Matshiny = 50.0;

/************* TEXTURE ****************/
GLuint texName[NUM_OF_TEXTURES];
size_t u2 = 1;
size_t v2 = 1;
double u3, v3;
unsigned width, height;
std::vector<unsigned char> preArr;

/************* CAMERA ****************/
float eyeX = 0;
float eyeY = 0;
float eyeZ = 0;
float targetX = 0;
float targetY = 0;
float targetZ = -100;

/************* POLYGON ****************/
int NumUs = 4;
int NumVs = 4;
GLenum shadeModel = GL_SMOOTH; // Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_FILL; // Toggles between GL_LINE and GL_FILL

/************* ANIMATION ****************/
GLenum runMode = GL_TRUE;
float RotX = 0.0f; // Rotational position around x-axis
float RotY = 0.0f; // Rotational position around y-axis
float RotZ = 0.0f; // Rotational position around z-axis
float RotIncrementX = 0.0; // Rotational increment, x-axis
float RotIncrementY = 0.0; // Rotational increment, y-axis
float RotIncrementZ = 0.0; // Rotational increment, z-axis
float RotIncFactor = 1.5; // Factor change in rot rate per key stroke