#include "controlPointLoader.h"
#include "controlPointArrs.h"
#include <GL/gl.h>

int pos = 0;

struct ctrlPtParts ctrlPtPartsArr[NUM_PARTS];

void loadControlPointsArr(void) {

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
