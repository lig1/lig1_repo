#include "lodepng.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#define NUM_OF_TEXTURES		2

//Data member declarations
size_t u2 = 1;
size_t v2 = 1;
double u3, v3;
unsigned width, height;

//Containers
std::vector<unsigned char> preArr;
static GLuint texName[NUM_OF_TEXTURES];

//Modifiable values
float eyeX = 0.5;
float eyeY = 0.5;
float eyeZ = 2.0;
float lookatX = 0.5;
float lookatY = 0.5;
float lookatZ = -15.0;

GLfloat quad1[] = { 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0 };
GLfloat quad2[] = { 1, 0, 0, 2, 0, 0, 2, 1, 0, 1, 1, 0 };

/***********************************/
/***********  UTILITY  ************/
/***********************************/
void drawQuad(int texNamePos, GLfloat arr[]) {

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texName[texNamePos]);
	glTexCoordPointer(3, GL_FLOAT, 0, arr);
	glVertexPointer(3, GL_FLOAT, 0, arr);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

std::vector<unsigned char> makePowerOfTwo(size_t& u2, size_t& v2,
		std::vector<unsigned char>& incArr,
		std::vector<unsigned char>& outArr) {

	//for cropping, the top is lowered for height, right border moves left for width
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
			for (size_t c = 0; c < 4; c++) {
				outArr[4 * u2 * y + 4 * x + c] = incArr[4 * width * y + 4 * x
						+ c];
			}

	return outArr;
}

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
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			&fullArr[0]);
}

/***********************************/
/***********    INIT    ************/
/***********************************/
void init() {
	glViewport(0, 0, 768, 768);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	/***********************************/
	/***********  TEXTURE  ************/
	/***********************************/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(NUM_OF_TEXTURES, texName);

	/***********************************/
	loadTexture("colorfade.png");
	std::vector<unsigned char> brickArr(u2 * v2 * 4);
	brickArr = makePowerOfTwo(u2, v2, preArr, brickArr);
	buildTexImg2D(brickArr, 0);

	/***********************************/
	loadTexture("wood.png");
	std::vector<unsigned char> woodArr(u2 * v2 * 4);
	woodArr = makePowerOfTwo(u2, v2, preArr, woodArr);
	buildTexImg2D(woodArr, 1);

	/***********************************/

	glEnable(GL_TEXTURE_2D);

	/***********************************/
	/***********  LIGHTING  ************/
	/***********************************/
	GLfloat left_light_pos[] = { 0.5, 0.5, 2.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);

	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

}

void display(void) {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble) (w) / (GLdouble) (h), 0.1, 40.0);
	gluLookAt(eyeX, eyeY, eyeZ, lookatX, lookatY, lookatZ, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	//	glTranslatef(-2.0, -2.0, -4.6);
	//	glScalef(7.0, 7.0, 7.0);

	drawQuad(0, quad1);
	drawQuad(1, quad2);

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 30.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(-2.0, -2.0, -4.6);
//	glScalef(7.0, 7.0, 7.0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	   glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

