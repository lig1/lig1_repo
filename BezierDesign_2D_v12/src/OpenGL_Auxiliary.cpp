/*       TextureExamples/texSpline.cpp  */
/*
 *  This builds the same object as closedSurface.c from out spline
 *  examples.  This time we build it twice.  On the left we build
 *  a texture spline to create spline coordinates, as well as a
 *  vertex spline to build point coordinates.  On the right we play
 *  the usual game for constructing texture coordinates on our own.
 */

#include <stdlib.h>
#include <GL/glut.h>

GLfloat circle[13][3] = { { -1.0, 0.0, 0.0 }, { -1.0, 0.5, 0.0 }, { -0.5, 1.0,
		0.0 }, { 0.0, 1.0, 0.0 }, { 0.5, 1.0, 0.0 }, { 1.0, 0.5, 0.0 }, { 1.0,
		0.0, 0.0 }, { 1.0, -0.5, 0.0 }, { 0.5, -1.0, 0.0 }, { 0.0, -1.0, 0.0 },
		{ -0.5, -1.0, 0.0 }, { -1.0, -0.5, 0.0 }, { -1.0, 0.0, 0.0 } };

#define N 10
#define SIZE 3*N+1
GLfloat controlPoints[SIZE][13][3];

GLfloat textureMap[64][64][3];
GLuint tex[2];

void assignColor(GLfloat col[3], GLfloat r, GLfloat g, GLfloat b) {
	col[0] = r;
	col[1] = g;
	col[2] = b;
}

int isEven(int x) {
	if (x % 2 == 0)
		return 1;
	else
		return 0;
}

void makeTextureMap() {
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 64; j++) {
			if (isEven(i / 8 + j / 8))
				assignColor(textureMap[i][j], 1.0, 1.0, 1.0);
			else
				assignColor(textureMap[i][j], 1.0, 0.0, 0.0);
		}
}

void makeTextures() {
	glGenTextures(2, tex);

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT,
			textureMap);

	/************************************ */
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	GLfloat planes[] = { 0.0, 0.3, 0.0, 0.0 };
	GLfloat planet[] = { 0.0, 0.0, 0.3, 0.0 };

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planes);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_FLOAT,
			textureMap);
}

void buildCircle(float radius, float height, GLfloat data[13][3]) {
	int i;

	for (i = 0; i < 13; i++) {
		data[i][0] = circle[i][0] * radius;
		data[i][1] = circle[i][1] * radius;
		data[i][2] = height;
	}
}

void buildSurfaceOfRotation() {
	buildCircle(1.0, 8.0, controlPoints[0]);
	buildCircle(1.0, 7.0, controlPoints[1]);
	buildCircle(1.5, 6.0, controlPoints[2]);
	buildCircle(1.5, 5.0, controlPoints[3]);
	buildCircle(2.0, 4.0, controlPoints[4]);
	buildCircle(2.5, 3.0, controlPoints[5]);
	buildCircle(2.5, 2.0, controlPoints[6]);
}

void display(void) {
	int row, col;
//  GLfloat patch[4][4][3];
	float texel[2][2][2] = { { { 0.0, 0.0 }, { 1.0, 0.0 } }, { { 0.0, 1.0 }, {
			1.0, 1.0 } } };

	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	buildSurfaceOfRotation();

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glPushMatrix();
	glTranslatef(0.0, -2.0, 0.0);
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 39, 4,
					&controlPoints[3 * row][3 * col][0]);
			glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2,
					&texel[0][0][0]);
			/* This second call to glMap2f builds the texture spline.  We replace the control point grid with the "texel"
			 grid at the top of this function.  This way the same parameters (say u and v) that produce points on the geometry
			 spline can also produce texture coordinates.  The arguments to glMap2 are
			 target, s_min, s_max, s_stride, s_order, t_min, t_max, t_stride, t_order
			 The  target is "GL_MAP2_TEXTURE_COORD_2.
			 Both parameter mins are 0, both maxes are 1.  Since we have 2 floats per point, s_stride is 2; since there
			 are two columns to texel we have 2*2 or 4 for t_stride.  Finally, since the texture spline values all lie in
			 a plane order 2 is fine for the texture spline. All that we are doing here is constructing a mesh over the
			 square 0 <= s, t <= 1.  */
			glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
			glEvalMesh2(GL_FILL, 0, 20, 0, 20);
		}
	}
	glPopMatrix();

	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glPushMatrix();
	glTranslatef(0.0, 4.0, 0.0);
	for (row = 0; row < 2; row++) {
		for (col = 0; col < 4; col++) {
			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 39, 4,
					&controlPoints[3 * row][3 * col][0]);
			/*      glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &texel[0][0][0]); */
			glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
			glEvalMesh2(GL_FILL, 0, 20, 0, 20);
		}
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
//	glDisable(GL_TEXTURE_GEN_T);

	glutSwapBuffers();
}

void myReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w * 1.0 / h, 0.5, 200);
	glMatrixMode(GL_MODELVIEW);
}

void myInit() {
	GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 30.0, 15.0, 10.0, 1.0 };
	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20.0, 5.0, 10.0, 0.0, 2.0, 5.0, 0.0, 0.0, 1.0);

	makeTextureMap();
	makeTextures();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

}

int main(int argc, char *argv[]) {
	int i, j, k, m, n;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("test");
	glutReshapeFunc(myReshape);
	myInit();
	glutDisplayFunc(display);

	glutMainLoop();
}
