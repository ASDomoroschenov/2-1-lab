#ifndef TASK7_H
#define TASK7_H

enum ERRORS {
	INVALID_SIZE = -2,
	NO_MEMORY = -1,
	SUCCES = 0
};

typedef struct tag_coord {
	double x;
	double y;
} coord;

int get_coordinates(coord **coordinates, int count, ...);
bool is_convex(coord *coordinates, int size);
double get_polynomial(double x, int step, ...);

#endif // TASK7_H