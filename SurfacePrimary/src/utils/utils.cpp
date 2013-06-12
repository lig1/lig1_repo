#include "utils.h"
#include "../config.h"
#include "../displayMain.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void drawLines() {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-100, 0, 0);
	glVertex3f(100, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1, -100, 0);
	glVertex3f(-1, 100, 0);
	//
	glVertex3f(0, -100, 0);
	glVertex3f(0, 100, 0);
	//
	glVertex3f(1, -100, 0);
	glVertex3f(1, 100, 0);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, -100);
	glVertex3f(0, 0, 100);

	glEnd();
	glEnable(GL_LIGHTING);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			runMode = !runMode;
			break;
		case 's':
			runMode = GL_TRUE;
			display();
			runMode = GL_FALSE;
			break;
		case 27: // Escape key
			exit(1);
		case 'r': // Reset the animation (resets everything)
			ResetAnimation();
			break;
		case '0': // Zero the rotation rates
			ZeroRotation();
			break;
		case 'f': // Shade mode toggles from flat to smooth
			ShadeModelToggle();
			break;
		case 'p': // Polygon mode toggles between fill and line
			FillModeToggle();
			break;
		case 'u': // Decrement number of U's
			LessUs();
			break;
		case 'U': // Increment number of U's
			MoreUs();
			break;
		case 'v': // Decrement number of V's
			LessVs();
			break;
		case 'V': // Increment number of V's
			MoreVs();
			break;

		case 'i':
			eyeY += 1;
			targetY += 1;
			break;
		case 'k':
			eyeY -= 1;
			targetY -= 1;
			break;
		case 'j':
			eyeX -= 1;
			targetX -= 1;
			break;
		case 'l':
			eyeX += 1;
			targetX += 1;
			break;
		case 'x':
			eyeZ -= 1;
			targetZ -= 1;
			break;
		case 'z':
			eyeZ += 1;
			targetZ += 1;
			break;
		case 'n':
			eyeX = 0;
			eyeY = 0;
			eyeZ = 0;
			targetX = 0;
			targetY = 0;
			targetZ = -100;
			break;
		case 'b':
			eyeZ *= -1;
			targetZ = 0;
			break;
	}

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	float aspectRatio;
	glViewport(0, 0, w, h); // View port uses whole window
	h = (w == 0) ? 1 : h;
	aspectRatio = (float) w / (float) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, 1.0, 30.0);
	gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			RotIncrementX = -0.5;
			break;
		case GLUT_KEY_UP:
			RotIncrementY = 0.5;
			break;
		case GLUT_KEY_RIGHT:
			RotIncrementZ = 0.5;
			break;
		case GLUT_KEY_DOWN:
			RotIncrementX = 0.0;
			RotIncrementY = 0.0;
			RotIncrementZ = 0.0;
			break;
	}
}

float genRand() {

	float f = (float) (rand() % 32767) / (float) 32767;

	return f;
}

void updateOrientation() {
	// Update the orientation, if the animation is running.
	if (runMode) {
		RotX += RotIncrementX;
		if (fabs(RotX) > 360.0) {
			RotX -= 360.0 * ((int) (((RotX / 360.0))));
		}
		RotY += RotIncrementY;
		if (fabs(RotY) > 360.0) {
			RotY -= 360.0 * ((int) (((RotY / 360.0))));
		}
		RotZ += RotIncrementZ;
		if (fabs(RotZ) > 360.0) {
			RotZ -= 360.0 * ((int) (((RotZ / 360.0))));
		}
	}

}

void ResetAnimation() {
	RotX = RotY = RotZ = RotIncrementX = RotIncrementY = RotIncrementZ = 0.0;
}

void ZeroRotation() {
	RotIncrementX = RotIncrementY = RotIncrementZ = 0.0;
}

void ShadeModelToggle() {
	if (shadeModel == GL_FLAT) {
		shadeModel = GL_SMOOTH;
	} else {
		shadeModel = GL_FLAT;
	}
}

void FillModeToggle() {
	if (polygonMode == GL_LINE) {
		polygonMode = GL_FILL;
	} else {
		polygonMode = GL_LINE;
	}
}

void MoreUs() {
	NumUs++;
}

void LessUs() {
	if (NumUs > 4) {
		NumUs--;
	}
}

void MoreVs() {
	NumVs++;
}

void LessVs() {
	if (NumVs > 4) {
		NumVs--;
	}
}

