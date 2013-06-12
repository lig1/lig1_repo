#include "textureUtils.h"
#include "lodepng.h"
#include "utils.h"
#include "../config.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#include <stdlib.h>
#include "../displayMain.h"
#include <math.h>
#include <limits.h>
#include <GL/glut.h>

#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
void texDimenPowerOfTwo() {
	// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
	while (u2 < width)
		u2 *= 2;
	while (v2 < height)
		v2 *= 2;
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

void buildTexImg2D(std::vector<unsigned char> fullArr, int texArrPos) {
	glBindTexture(GL_TEXTURE_2D, texName[texArrPos]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &fullArr[0]);
}

void loadTexture(const std::string& filename) {
	preArr.erase(preArr.begin(), preArr.end());
	lodepng::decode(preArr, width, height, filename);
	texDimenPowerOfTwo();
}
