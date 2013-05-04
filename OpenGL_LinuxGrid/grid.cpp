#include "display.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>

#define NUM_OF_ANGLES	10
#define ROT_INC			0.1
#define factor 			1

using namespace std;
using namespace glm;

void setCamera(void);
static GLfloat g_rotate = 0;
static GLfloat g_rotInc = ROT_INC;

float eyeX = 0.0;
float eyeY = 0.0;
float eyeZ = 15.0;
float lookatX = 0.0;
float lookatY = 0.0;
float lookatZ = -15.0;

int multiplierXZ = 0;
int multiplierYZ = 0;

struct Vertex
{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
};

struct VertRef
{
	VertRef( int v, int vt, int vn )
		: v(v), vt(vt), vn(vn)
	{ }
	int v, vt, vn;
};


vector<vector<Vertex>> models;

vector< Vertex > LoadOBJ( istream& in )
{
	vector< Vertex > verts;

	vector< vec4 > positions( 1, vec4( 0, 0, 0, 0 ) );
	vector< vec3 > texcoords( 1, vec3( 0, 0, 0 ) );
	vector< vec3 > normals( 1, vec3( 0, 0, 0 ) );
	string lineStr;
	while( getline( in, lineStr ) )
	{
		istringstream lineSS( lineStr );
		string lineType;
		lineSS >> lineType;

		// vertex
		if( lineType == "v" )
		{
			float x = 0, y = 0, z = 0, w = 1;
			lineSS >> x >> y >> z >> w;
			positions.push_back( vec4( x, y, z, w ) );
		}

		// texture
		if( lineType == "vt" )
		{
			float u = 0, v = 0, w = 0;
			lineSS >> u >> v >> w;
			texcoords.push_back( vec3( u, v, w ) );
		}

		// normal
		if( lineType == "vn" )
		{
			float i = 0, j = 0, k = 0;
			lineSS >> i >> j >> k;
			normals.push_back( normalize( vec3( i, j, k ) ) );
		}

		// polygon
		if( lineType == "f" )
		{
			vector< VertRef > refs;
			string refStr;
			while( lineSS >> refStr )
			{
				istringstream ref( refStr );
				string vStr, vtStr, vnStr;
				getline( ref, vStr, '/' );
				getline( ref, vtStr, '/' );
				getline( ref, vnStr, '/' );
				int v = atoi( vStr.c_str() );
				int vt = atoi( vtStr.c_str() );
				int vn = atoi( vnStr.c_str() );
				v  = (  v >= 0 ?  v : positions.size() +  v );
				vt = ( vt >= 0 ? vt : texcoords.size() + vt );
				vn = ( vn >= 0 ? vn : normals.size()   + vn );
				refs.push_back( VertRef( v, vt, vn ) );
			}

			if( refs.size() < 3 )
			{
				// error, skip
				continue;
			}

			// triangulate, assuming n>3-gons are convex and coplanar
			VertRef* p[3] = { &refs[0], NULL, NULL };
			for( size_t i = 1; i+1 < refs.size(); ++i )
			{
				p[1] = &refs[i+0];
				p[2] = &refs[i+1];

				vec3 U( positions[ p[1]->v ] - positions[ p[0]->v ] );
				vec3 V( positions[ p[2]->v ] - positions[ p[0]->v ] );
				vec3 faceNormal = normalize( cross( U, V ) );

				for( size_t j = 0; j < 3; ++j )
				{
					Vertex vert;
					vert.position = vec3( positions[ p[j]->v ] );
					vert.texcoord = vec2( texcoords[ p[j]->vt ] );
					vert.normal   = ( p[j]->vn != 0 ? normals[ p[j]->vn ] : faceNormal );
					verts.push_back( vert );
				}
			}
		}
	}

	return verts;
}

void myKey(unsigned char k, int x, int y)
{
	float resX;
	float resY;

	switch (k) {
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'r':
		eyeX -= 1.0;
		lookatX -= 1.0;

		setCamera();
		break;
	case 'l':
		eyeX += 1.0;
		lookatX += 1.0;

		setCamera();
		break;
	case 'u':
		eyeY += 1.0;
		lookatY += 1.0;

		setCamera();
		break;
	case 'd':
		eyeY -= 1.0;
		lookatY -= 1.0;

		setCamera();
		break;
	case 'i':
		eyeZ += 1.0;
		lookatZ += 1.0;

		setCamera();
		break;
	case 'o':
		eyeZ -= 1.0;
		lookatZ -= 1.0;

		setCamera();
		break;
	case 'm':
		//looks immediately left

		if (multiplierXZ != 0) {
			multiplierXZ--;
		} else {
			multiplierXZ = NUM_OF_ANGLES - 1;
		}

		lookatX = Grid::trigSinVals[multiplierXZ] + eyeX;
		lookatZ = Grid::trigCosVals[multiplierXZ] + eyeZ;

		setCamera();
		break;
	case 'n':
		//looks immediately right

		if (multiplierXZ != (NUM_OF_ANGLES - 1)) {
			multiplierXZ++;
		} else {
			multiplierXZ = 0;
		}

		lookatX = Grid::trigSinVals[multiplierXZ] + eyeX;
		lookatZ = Grid::trigCosVals[multiplierXZ] + eyeZ;

		printf("\nAngle XZ: %d",multiplierYZ*36);

		setCamera();
		break;
	case 'j':
		//looks immediately left

		if (multiplierYZ != 0) {
			multiplierYZ--;
		} else {
			multiplierYZ = 9;
		}

		lookatY = Grid::trigSinVals[multiplierYZ] + eyeY;
		lookatZ = Grid::trigCosVals[multiplierYZ] + eyeZ;

		printf("\nAngle YZ: %d",multiplierYZ*36);

		setCamera();
		break;
	case 'k':
		//looks immediately right

		if (multiplierYZ != 9) {
			multiplierYZ++;
		} else {
			multiplierYZ = 0;
		}

		lookatY = Grid::trigSinVals[multiplierYZ] + eyeY;
		lookatZ = Grid::trigCosVals[multiplierYZ] + eyeZ;

		setCamera();
		break;
	default:
		printf("Unknown keyboard command \'%c\'.\n", k);
		break;
	}
}


void setCamera(void) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(1.0, 1.0, -1.0);
	//glScalef(1.0, 1.0, -1.0);
	gluPerspective( 60.0, (GLdouble)w/(GLdouble)h, 0.1, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,lookatX, lookatY, lookatZ,0.0, 1.0, 0.0);

	glutPostRedisplay();
}

void display(void)
{
	printf("%s%g%s%g%s%g", "\nDISPLAY - Camera now positioned at ", eyeX, ",", eyeY, ",", eyeZ);
	printf("%s%g%s%g%s%g%s", "\nDISPLAY - Looking at ", lookatX, ",", lookatY, ",", lookatZ, "\n");

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0, (GLdouble)w/(GLdouble)h, 0.1, 40.0);

	gluLookAt(eyeX, eyeY, eyeZ, lookatX, lookatY, lookatZ, 0.0, 1.0, 0.0);

	/** grid */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0, 1.0, 1.0);
	glTranslatef(-1, -1, -1);
	glColor3ub( 0, 0, 180 );
	OglDisplay::drawGrid();

	/** stone */
	glColor3ub( 180, 0, 0 );
	glTranslatef(-10, -1, -1);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &model[0].position );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &model[0].texcoord );
	glNormalPointer( GL_FLOAT, sizeof(Vertex), &model[0].normal );
	glDrawArrays( GL_TRIANGLES, 0, model.size() );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	
	glPopMatrix();
	glutSwapBuffers();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); /* init projection matrix */
	gluPerspective( 60.0, (GLdouble)w/(GLdouble)h, 0.1, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0, 1.0, -1.0);
	gluLookAt(eyeX, eyeY, eyeZ,
		lookatX, lookatY, lookatZ,
		0.0, 1.0, 0.0);

	printf("%s","\nreshape");
}

void myMouse(int btn, int state, int x, int y)
{

}

void myIdleFunc(void) {

}

int main(int argc, char **argv)
{
	//from objloader
	ifstream ifile("test.obj" );
	ifstream arr[1];
	arr[0]=ifile;
	int j;
	for (j=0;j<1;j++) {
		models.push_back(LoadOBJ( ifile ));
	}

	glutInit( &argc, argv );
	Util::loadTrigValsIntoArr();
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
	glutInitWindowSize( 640, 480 );
	glutReshapeFunc(myReshape);
	glutCreateWindow( "3D GRID" );
	glutDisplayFunc( display );
	glutIdleFunc(myIdleFunc);
	glutKeyboardFunc(myKey);
	glutMouseFunc(myMouse);

	glEnable( GL_DEPTH_TEST );

	// set up "headlamp"-like light
	glShadeModel( GL_SMOOTH );
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	//glMatrixMode(GL_PROJECTION);
	//gluLookAt(-10, 0, 5, 0, 0, 5, 0.0, 1.0, 0.0);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glScalef(1.0, 1.0, -1.0);
	glTranslatef(-1,-1,-1);

	GLfloat position[] = { 0, 0, 1, 0 };
	glLightfv( GL_LIGHT0, GL_POSITION, position );

	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_LINE );

	glutMainLoop();

	return 0;
}

