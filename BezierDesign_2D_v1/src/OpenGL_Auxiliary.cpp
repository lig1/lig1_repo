/*  bezsurf.c
 *  This program renders a wireframe Bezier surface,
 *  using two-dimensional evaluators.
 */
#include <stdlib.h>
#include <GL/glut.h>

bool perspective = false;
bool bezSolid = false;
bool rotateLeft = false;
bool rotateRight = false;
bool rotateStop = false;

//Modifiable values
float eyeX = 0;
float eyeY = 3;
float eyeZ = 14;

float lookatX = 0;
float lookatY = 3;
float lookatZ = 0;

float lightX = 0;
float lightY = 0;
float lightZ = 5;
float lightW = 1.0;

float rotateDeg = 180.0;
float rotateX = 0.0;
float rotateY = 0.5;
float rotateZ = 0.0;

float translateX = 3.0;
float translateY = -2.0;
float translateZ = 0.0;

GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat position[] = { lightX, lightY, lightZ, lightW };
GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

GLfloat ctrlpoints[4][4][3] = { { { 1, 0, 1 }, { 0.65, 1.5, -1 }, { 1.65, 3.0,
		-2 }, { 2.5, 4.5, -8 } }, { { 3, 0, 0 }, { 1.9, 1.5, 0 },
		{ 2.35, 3, 0 }, { 2.6, 4.5, 0 } }, { { 4, 0, 0 }, { 3.35, 1.5, 0 }, { 3,
		3, 0 }, { 4, 4.5, 0 } }, { { 5.5, 0, 0 }, { 4.2, 1.25, -1 }, { 3.2,
		2.51, -2 }, { 5, 3.4, -4 } } };

void initlights(void) {
	GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat position[] = { lightX, lightY, lightZ, lightW };
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

//	GLfloat position2[] = { 0, 0, -5, 1 };
//	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
//	glLightfv(GL_LIGHT1, GL_POSITION, position2);

//	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);

}

void display(void) {
	int i, j;
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (perspective) {
		gluPerspective(60.0, (GLdouble) (w) / (GLdouble) (h), 0.1, 40.0);
		gluLookAt(eyeX, eyeY, eyeZ, lookatX, lookatY, lookatZ, 0.0, 1.0, 0.0);
	} else {
		if (w <= h) {
			glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
					4.0 * (GLfloat) h / (GLfloat) w, -4.0, 4.0);
		} else {
			glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
					4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -4.0, 4.0);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

	glTranslatef(lightX, lightY, lightZ);
	glColor3f(0.0f, 0.0f, 1.0f); //set cube color
	glutWireCube(.75);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(translateX, translateY, translateZ);
	glRotatef(rotateDeg, rotateX, rotateY, rotateZ);

	if (bezSolid) {
		glShadeModel(GL_SMOOTH); //
		glEnable(GL_DEPTH_TEST);
			glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
			glEnable(GL_LIGHTING);

			glColorMaterial(GL_FRONT, GL_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);

		glEvalMesh2(GL_FILL, 0, 30, 0, 30);
		glDisable(GL_LIGHTING);
	} else {

		glColor3f(1.0, 1.0, 1.0);
		for (j = 0; j <= 8; j++) { //9 lines total in each dimension
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i++) //growth
				glEvalCoord2f((GLfloat) i / 30.0, (GLfloat) j / 8.0);
			glEnd();
			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= 30; i++) //growth
				glEvalCoord2f((GLfloat) j / 8.0, (GLfloat) i / 30.0);
			glEnd();
		}
	}
	glPopMatrix();
	glFlush();

	if (rotateRight) {
		rotateDeg += 0.25;
		glutPostRedisplay();
	} else if (rotateLeft) {
		rotateDeg -= 0.25;
		glutPostRedisplay();
	}
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	initlights(); /* for lighted version only */

	if (bezSolid) {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_AUTO_NORMAL); //
		//glEnable(NORMALIZE);
		glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0); //
	} else {
//		glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
		glShadeModel(GL_FLAT); //
	}
}

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (perspective) {
		gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	} else {
		if (w <= h)
			glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
					4.0 * (GLfloat) h / (GLfloat) w, -4.0, 4.0);
		else
			glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
					4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -4.0, 4.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		rotateLeft = true;
		rotateRight = false;
		glutPostRedisplay();
		break;
	case 'e':
		rotateLeft = false;
		rotateRight = true;
		glutPostRedisplay();
		break;
	case 'w':
		rotateLeft = false;
		rotateRight = false;
		rotateStop = true;
		glutPostRedisplay();
		break;
	case 'a':
		if (bezSolid) {
			bezSolid = false;
		} else {
			bezSolid = true;
		}
		init();
		glutPostRedisplay();
		break;
	case 'z': //pull camera back
		eyeZ -= 1;
		lookatZ -= 1;
		glutPostRedisplay();
		break;
	case 'x': //push camera forward
		eyeZ += 1;
		lookatZ += 1;
		glutPostRedisplay();
		break;
	case 'r':
		translateX -= 1;
		glutPostRedisplay();
		break;
	case 't':
		translateX += 1;
		glutPostRedisplay();
		break;
	case 'y':
		translateY -= 1;
		glutPostRedisplay();
		break;
	case 'u':
		translateY += 1;
		glutPostRedisplay();
		break;
	case 'i':
		translateZ -= 1;
		glutPostRedisplay();
		break;
	case 'o':
		translateZ += 1;
		glutPostRedisplay();
		break;
	case 'p':
		if (!perspective) {
			perspective = true;
		} else if (perspective) {
			perspective = false;
		}
		glutPostRedisplay();
		break;
	default:
		rotateLeft = false;
		rotateRight = false;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
