/**************************************************
 * USAGES: There are a number of keyboard commands that control
 * the animation.  They must be typed into the graphics window,
 * and are listed below:
 *
 * CONTROLLING RESOLUTION OF THE NURBS MESH
 *   Press "U" to increase the number samples in U direction.
 *   Press "u" to decrease the number samples in U direction.
 *   Press "V" to increase the number samples in V direction.
 *   Press "v" to decrease the number samples in U direction.
 *
 * CONTROLLING THE ANIMATION:
 *   Press the "a" key to toggle the animation off and on.
 *   Press the "s" key to perform a single step of the animation.
 *   The left and right arrow keys controls the
 *		rate of rotation around the y-axis.
 *   The up and down arrow keys increase and decrease the rate of
 *		rotation around the x-axis.  In order to reverse rotational
 *		direction you must zero or reset the patch ("0" or "r").
 *   Press the "r" key to reset back to initial
 *		position, with no rotation.
 *   Press "0" (zero) to zero the rotation rates.
 *
 * COMMANDS SHOWING OPENGL FEATURES:
 *   Pressing "p" toggles between wireframe and polygon mode.
 *   Pressing "f" key toggles between flat and smooth shading.
 *
 * FOR CONTROL POINTS AT INFINITY.  Change the value of the
 *   variable USE_CNTLPT_AT_INFINITY from 0 to 1 and recompile.
 *   Should be identical in functionality.
 *
 **/

//Table of Contents
//Data
/************* CONTROL POINTS ****************/
/************* POLYGON ****************/
/************* ANIMATION ****************/
/************* LIGHTING ****************/
/************* MATERIAL ****************/
/************* TEXTURE ****************/
/************* CAMERA ****************/

//Funcs
/************* UTILS ****************/
////********** CAMERA ****************/
/************* TEXTURE FUNCS ****************/
/************* DISPLAY ****************/
////********** POLYGON **********/
////********** MATERIAL **********/
////********** UPDATE ORIENTATION **********/
////********** DRAW BEZIER AND TEXTURIZE **********/
////********** DRAW LINES **********/
////********** DRAW LIGHT BOXES **********/
/************* INIT ****************/
/************* MAIN ****************/

#include "lodepng.h"
#include "Utils.h"

#include "externVarDeclaration.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#include <stdlib.h>
#include "SimpleNurbs.h"
#include <math.h>
#include <limits.h>
#include <GL/glut.h>

#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#define NUM_OF_TEXTURES		2
#define NUM_OF_LEAVES		400
#define TREE_TRUNK_TEXTURE "textures/wood.png"

float leafBox_StartX = -5;
float leafBox_EndX = 5;
float leafBox_StartY = 9;
float leafBox_EndY = 15;
float leafBox_StartZ = -5;
float leafBox_EndZ = 5;

float xSpan = leafBox_EndX - leafBox_StartX;
float ySpan = leafBox_EndY - leafBox_StartY;
float zSpan = leafBox_EndZ - leafBox_StartZ;

struct leaf {
	float transX;
	float transY;
	float transZ;
//	float transX;
//	float transX;
//	float transX;
} leaves[NUM_OF_LEAVES];

/************* POLYGON ****************/
int NumUs = 4;
int NumVs = 4;
GLenum shadeModel = GL_FLAT; // Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_LINE; // Toggles between GL_LINE and GL_FILL

/************* ANIMATION ****************/
GLenum runMode = GL_TRUE;
float RotX = 0.0f; // Rotational position around x-axis
float RotY = 0.0f; // Rotational position around y-axis
float RotZ = 0.0f; // Rotational position around z-axis
float RotIncrementX = 0.0; // Rotational increment, x-axis
float RotIncrementY = 0.0; // Rotational increment, y-axis
float RotIncrementZ = 0.0; // Rotational increment, z-axis
const float RotIncFactor = 1.5; // Factor change in rot rate per key stroke

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
static GLuint texName[NUM_OF_TEXTURES];
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

int pos = 0;
#define NUM_PARTS	16

struct ctrlPtParts {

	GLfloat *points;

	float transX;
	float transY;
	float transZ;
	float scaleX;
	float scaleY;
	float scaleZ;
} ctrlPtPartsArr[NUM_PARTS];

/************* UTILS ****************/
void loadMasterArr(void) {

	ctrlPtPartsArr[pos].points = &ctrlpoints_A[0][0][0];
	ctrlPtPartsArr[pos].transX = -3.1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_B[0][0][0];
	ctrlPtPartsArr[pos].transX = -1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_C[0][0][0];
	ctrlPtPartsArr[pos].transX = 0;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_D[0][0][0];
	ctrlPtPartsArr[pos].transX = 1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_H[0][0][0];
	ctrlPtPartsArr[pos].transX = 1;
	ctrlPtPartsArr[pos].transY = -3.4;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_G[0][0][0];
	ctrlPtPartsArr[pos].transX = 0;
	ctrlPtPartsArr[pos].transY = -3.4;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_F[0][0][0];
	ctrlPtPartsArr[pos].transX = -1;
	ctrlPtPartsArr[pos].transY = -2.6;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_E[0][0][0];
	ctrlPtPartsArr[pos].transX = -3.1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_DH[0][0][0];
	ctrlPtPartsArr[pos].transX = 1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_CH[0][0][0];
	ctrlPtPartsArr[pos].transX = 0;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_BH[0][0][0];
	ctrlPtPartsArr[pos].transX = -1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_AH[0][0][0];
	ctrlPtPartsArr[pos].transX = -3.1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_GH[0][0][0];
	ctrlPtPartsArr[pos].transX = 0;
	ctrlPtPartsArr[pos].transY = -6.8;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_HH[0][0][0];
	ctrlPtPartsArr[pos].transX = 1;
	ctrlPtPartsArr[pos].transY = -3.4;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_FH[0][0][0];
	ctrlPtPartsArr[pos].transX = -1;
	ctrlPtPartsArr[pos].transY = -2.6;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;

	ctrlPtPartsArr[pos].points = &ctrlpoints_EH[0][0][0];
	ctrlPtPartsArr[pos].transX = -3.1;
	ctrlPtPartsArr[pos].transY = 0;
	ctrlPtPartsArr[pos].transZ = 0;
	ctrlPtPartsArr[pos].scaleX = 1;
	ctrlPtPartsArr[pos].scaleY = 1;
	ctrlPtPartsArr[pos].scaleZ = 1;
	pos++;
}

void myKeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			runMode = !runMode;
			break;
		case 's':
			runMode = GL_TRUE;
			updateScene();
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

void mySpecialKeyFunc(int key, int x, int y) {
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

/************* TEXTURE FUNCS ****************/

void texDimenPowerOfTwo() {
	// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
	while (u2 < width)
		u2 *= 2;
	while (v2 < height)
		v2 *= 2;
}

void loadTexture(const std::string& filename) {
	preArr.erase(preArr.begin(), preArr.end());
	lodepng::decode(preArr, width, height, filename);
	texDimenPowerOfTwo();
}

void buildTexImg2D(std::vector<unsigned char> fullArr, int texArrPos) {
	glBindTexture(GL_TEXTURE_2D, texName[texArrPos]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &fullArr[0]);
}

std::vector<unsigned char> makePowerOfTwo(size_t& u2, size_t& v2, std::vector<unsigned char>& incArr,
		std::vector<unsigned char>& outArr) {

	//for cropping, the top is lowered for height, right border moves left for width
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
			for (size_t c = 0; c < 4; c++) {
				outArr[4 * u2 * y + 4 * x + c] = incArr[4 * width * y + 4 * x + c];
			}

	return outArr;
}

void initTexture(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D); //made it change color
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(NUM_OF_TEXTURES, texName);

	loadTexture(TREE_TRUNK_TEXTURE);
	std::vector<unsigned char> brickArr(u2 * v2 * 4);
	brickArr = makePowerOfTwo(u2, v2, preArr, brickArr);
	buildTexImg2D(brickArr, 0);
}

/************* DISPLAY UTILS ****************/
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

/************* DISPLAY ****************/
void updateScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float texel[2][2][2] =
		{
			{
				{0.0, 0.0},
				{1.0, 0.0}},
			{
				{0.0, 1.0},
				{1.0, 1.0}}};

	glPushMatrix();
	/********** UPDATE ORIENTATION **********/ //needs to be in a matrix
	updateOrientation();
	glRotatef(RotX, 1.0, 0.0, 0.0);
	glRotatef(RotY, 0.0, 1.0, 0.0);
	glRotatef(RotZ, 0.0, 0.0, 1.0);

	glScalef(0.5, 1.0, 0.5);
	glRotatef(-90, 1.0, 0.0, 0.0);

	for (int i = 0; i < NUM_PARTS; i++) {
		glPushMatrix();

		//glRotatef(-90, 1.0, 0.0, 0.0);
		glTranslatef(ctrlPtPartsArr[i].transX, ctrlPtPartsArr[i].transY, ctrlPtPartsArr[i].transZ);
		glScalef(ctrlPtPartsArr[i].scaleX, ctrlPtPartsArr[i].scaleY, ctrlPtPartsArr[i].scaleZ);
//		glRotatef(-90, 1.0, 0.0, 0.0);

		/********** POLYGON **********/
		glShadeModel(shadeModel);
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

		/********** MATERIAL **********/
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Matnonspec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Matspec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Matshiny);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);

		/********** DRAW BEZIER AND TEXTURIZE **********/
		glEnable(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texName[0]);

		glMap2f(GL_MAP2_VERTEX_4, 0, 1, 4, 4, 0, 1, 16, 4, ctrlPtPartsArr[i].points);
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &texel[0][0][0]);
		glMapGrid2f(NumUs, 0, 1, NumVs, 0, 1);
		glEvalMesh2(GL_FILL, 0, NumUs, 0, NumVs);
		glDisable(GL_TEXTURE);
		glPopMatrix();
	}
//	glPopMatrix();

	//leaves
	for (int i = 0; i < NUM_OF_LEAVES; i++) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(0, 1, 0);

		glTranslatef(leaves[i].transX, leaves[i].transZ, leaves[i].transY); //leaves[i].transZ);

//		std::cout << "***X is: " << leaves[i].transX << " - Y is: " << leaves[i].transY << "\n";

		glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(0.5, 0, 0);
		glVertex3f(0.5, 0, 0.5);
		glVertex3f(0, 0, 0.5);
		glEnd();

//		std::cout << "\n\n";
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	glPopMatrix();

	////////////////////////////////////////////////////////
//	glPushMatrix();
//	glTranslatef(1, 0, 0);
//	/********** POLYGON **********/
//	glShadeModel(shadeModel);
//	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
//
//	/********** MATERIAL **********/
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Matnonspec);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Matspec);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Matshiny);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);
//
//	/********** UPDATE ORIENTATION **********/
//	updateOrientation();
//
//	glRotatef(RotX, 1.0, 0.0, 0.0);
//	glRotatef(RotY, 0.0, 1.0, 0.0);
//
//	/********** DRAW BEZIER AND TEXTURIZE **********/
//	glEnable(GL_TEXTURE);
//	glBindTexture(GL_TEXTURE_2D, texName[0]);
//	glMap2f(GL_MAP2_VERTEX_4, 0, 1, 4, 4, 0, 1, 16, 4, &ctrlpoints_D[0][0][0]);
//	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
//			&texel[0][0][0]);
//	glMapGrid2f(NumUs, 0, 1, NumVs, 0, 1);
//	glEvalMesh2(GL_FILL, 0, NumUs, 0, NumVs);
//	glDisable(GL_TEXTURE);
//	glPopMatrix();
	////////////////////////////////////////////////////////

	/********** DRAW LINES **********/
	glPushMatrix();
	drawLines();
	glPopMatrix();

	/********** DRAW LIGHT BOXES **********/
	glPushMatrix();
	glDisable(GL_LIGHTING);

	glLoadIdentity();
	glTranslatef(1.0, 0.0, 1.0);
	glColor3f(1, 1, 1);
	glutWireCube(0.75);

	glLoadIdentity();
	glTranslatef(0.0, 1.0, 1.0);
	glColor3f(1, 1, 1);
	glutWireCube(0.75);

	glEnable(GL_LIGHTING);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}

/************* INIT ****************/
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_MAP2_VERTEX_4);

	glEnable(GL_LIGHTING); // Enable lighting calculations
	glEnable(GL_LIGHT0); // Turn on lights (unnecessary here, since also in Animate()
	glEnable(GL_LIGHT1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); // Ambient light
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Phong light backsides

	// Next line is suspicious, perhaps compensating for OpenGL bug
	glFrontFace(GL_CW); // Is needed for Bezier Patches (OpenGL bug??)

	// Light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);
	glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// Light 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, Lt1amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Lt1diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Lt1spec);
	glLightfv(GL_LIGHT1, GL_POSITION, Lt1pos);
}

void resizeWindow(int w, int h) {
	float aspectRatio;
	glViewport(0, 0, w, h); // View port uses whole window
	h = (w == 0) ? 1 : h;
	aspectRatio = (float) w / (float) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspectRatio, 1.0, 30.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glTranslatef(0.0, 0.0, -10.0);
//	glRotatef(15.0, 1.0, 0.0, 0.0);
}

void genLeaves() {

	srand(5888);

	for (int i = 0; i < NUM_OF_LEAVES; i++) {

		float randFloat = genRand();
		leaves[i].transX = (xSpan * randFloat) + leafBox_StartX;

		randFloat = genRand();
		leaves[i].transY = (ySpan * randFloat) + leafBox_StartY;

		randFloat = genRand();
		leaves[i].transZ = (zSpan * randFloat) + leafBox_StartZ;

		printf("Gen X is: %f - Gen Y is: %f - Gen Z is: %f - \n", leaves[i].transX, leaves[i].transY,
				leaves[i].transZ);
	}
}

/************* MAIN ****************/
int main(int argc, char** argv) {

	loadMasterArr();
	genLeaves();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(10, 60);
	glutInitWindowSize(620, 160);
	glutCreateWindow("Nurbs demo");

	initRendering();
	initTexture();
	resizeWindow(620, 160);

	glutKeyboardFunc(myKeyboardFunc);
	glutSpecialFunc(mySpecialKeyFunc);
	glutReshapeFunc(resizeWindow);
	glutIdleFunc(updateScene);
	glutDisplayFunc(updateScene);
	glutMainLoop();

	return 0;
}
