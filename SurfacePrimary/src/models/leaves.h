#ifndef LEAVES_H_
#define LEAVES_H_

#define NUM_OF_LEAVES		400
#define TREE_TRUNK_TEXTURE "texture/wood.png"

struct leaf {
	float transX;
	float transY;
	float transZ;
};

extern float leafBox_StartX;
extern float leafBox_EndX;
extern float leafBox_StartY;
extern float leafBox_EndY;
extern float leafBox_StartZ;
extern float leafBox_EndZ;

extern float xSpan;
extern float ySpan;
extern float zSpan;

extern struct leaf leaves[NUM_OF_LEAVES];

void genLeaves();

#endif /* LEAVES_H_ */