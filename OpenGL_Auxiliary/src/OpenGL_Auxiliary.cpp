/* Author : Vincent Tan Wai Lip
 URL : http://polymathprogrammer.com/
 */

#include <stdio.h>

int interpolate(double, double, double, double, double, double, double, double,
		double, double, double, double, double, double);

typedef struct tagVector3D {
	double x, y, z;
} Vector3D;

Vector3D pos[4];

int main(void) {
	int i, result;
	char response;
	double u, v;

	do {
		for (i = 0; i < 4; ++i) {
			pos[i].x = 0.0;
			pos[i].y = 0.0;
			pos[i].z = 0.0;
		}

		printf(
				"Enter the points, three components at a time (x y z) with a space between them.");
		printf("\nEnter the first point : ");
		scanf("%lf", &(pos[0].x));
		scanf("%lf", &(pos[0].y));
		scanf("%lf", &(pos[0].z));

		printf("\nEnter u : ");
		scanf("%lf", &u);
		printf("Enter the second point : ");
		scanf("%lf", &(pos[1].x));
		scanf("%lf", &(pos[1].y));
		scanf("%lf", &(pos[1].z));

		printf("\nEnter v : ");
		scanf("%lf", &v);
		printf("Enter the third point : ");
		scanf("%lf", &(pos[2].x));
		scanf("%lf", &(pos[2].y));
		scanf("%lf", &(pos[2].z));

		printf("\nEnter the fourth point : ");
		scanf("%lf", &(pos[3].x));
		scanf("%lf", &(pos[3].y));
		scanf("%lf", &(pos[3].z));

		/* I understand the redundancy of passing global variables into
		 the function when there's no need to. I am concentrating on the function,
		 not the program semantics. Globals were used so that there are less
		 function arguments. */
		result = interpolate(pos[0].x, pos[0].y, pos[0].z, u, pos[1].x,
				pos[1].y, pos[1].z, v, pos[2].x, pos[2].y, pos[2].z, pos[3].x,
				pos[3].y, pos[3].z);

		if (result == 1) {
			printf("\nThe calculated control points are:");
			for (i = 0; i < 4; ++i) {
				printf("\nPoint %d (%4.5f   %4.5f   %4.5f)", i + 1, pos[i].x,
						pos[i].y, pos[i].z);
			}
		} else {
			printf("\nUnable to calculate Bezier control points.");
		}

		printf("\n\nDo again? (Y/N) ");
		scanf(" %c", &response);
	} while (response == 'Y' || response == 'y');

	return 0;
}

int interpolate(double p0x, double p0y, double p0z, double u, double p1x,
		double p1y, double p1z, double v, double p2x, double p2y, double p2z,
		double p3x, double p3y, double p3z) {
	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, det = 0.0;
	Vector3D q1, q2;

	if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0) || (u >= v))
		return 0; /* failure */

	a = 3 * (1 - u) * (1 - u) * u;
	b = 3 * (1 - u) * u * u;
	c = 3 * (1 - v) * (1 - v) * v;
	d = 3 * (1 - v) * v * v;
	det = a * d - b * c;
	/* unnecessary, but just in case... */
	if (det == 0.0)
		return 0; /* failure */

	pos[0].x = p0x;
	pos[0].y = p0y;
	pos[0].z = p0z;
	pos[3].x = p3x;
	pos[3].y = p3y;
	pos[3].z = p3z;

	q1.x = p1x - ((1 - u) * (1 - u) * (1 - u) * p0x + u * u * u * p3x);
	q1.y = p1y - ((1 - u) * (1 - u) * (1 - u) * p0y + u * u * u * p3y);
	q1.z = p1z - ((1 - u) * (1 - u) * (1 - u) * p0z + u * u * u * p3z);

	q2.x = p2x - ((1 - v) * (1 - v) * (1 - v) * p0x + v * v * v * p3x);
	q2.y = p2y - ((1 - v) * (1 - v) * (1 - v) * p0y + v * v * v * p3y);
	q2.z = p2z - ((1 - v) * (1 - v) * (1 - v) * p0z + v * v * v * p3z);

	pos[1].x = d * q1.x - b * q2.x;
	pos[1].y = d * q1.y - b * q2.y;
	pos[1].z = d * q1.z - b * q2.z;
	pos[1].x /= det;
	pos[1].y /= det;
	pos[1].z /= det;

	pos[2].x = (-c) * q1.x + a * q2.x;
	pos[2].y = (-c) * q1.y + a * q2.y;
	pos[2].z = (-c) * q1.z + a * q2.z;
	pos[2].x /= det;
	pos[2].y /= det;
	pos[2].z /= det;

	return 1; /* success */
}
