#ifndef GPHELPERS_H
#define GPHELPERS_H

#include <cmath>
#include <string>
#include <sstream>
#include <vector>

// http://stackoverflow.com/questions/12931115/algorithm-to-map-an-interval-to-a-smaller-interval
// output is a number m to multiply the value by and a number to add to that product
inline void continuousMapRange(double w, double x, double y, double z, double* m, double* b) {
	/*
    if ((w+0.0) == 0 && std::signbit(w) == 1) {
        w *= -1;
    }
	*/
    *m = (z - y)/(x - w);
    *b = (y - w * (*m));
}

inline void continuous_map_range(float w, float x, float y, float z, float* m, float* b) {
    *m = (z - y)/(x - w);
    *b = (y - w * (*m));
}

inline void intervalAdd(float* min, float* max, float a, float b, float c, float d) {
    float differences[4];
    differences[0] = a + c;
    differences[1] = a + d;
    differences[2] = b + c;
    differences[3] = b + d;

	*min = differences[0];
	*max = differences[0];

    for (unsigned i = 1; i < 4; i++) {
        if (differences[i] < *min)
            *min = differences[i];
        if (differences[i] > *max)
            *max = differences[i];
    }
}

inline void intervalSubtract(float* min, float* max, float a, float b, float c, float d) {
    float differences[4];
    differences[0] = a - c;
    differences[1] = a - d;
    differences[2] = b - c;
    differences[3] = b - d;

	*min = differences[0];
	*max = differences[0];

    for (unsigned i = 1; i < 4; i++) {
        if (differences[i] < *min)
            *min = differences[i];
        if (differences[i] > *max)
            *max = differences[i];
    }
}

inline void intervalMultiply(float* min, float* max, float a, float b, float c, float d) {
    float products[4];
    products[0] = a * c;
    products[1] = a * d;
    products[2] = b * c;
    products[3] = b * d;

	*min = products[0];
	*max = products[0];

    for (unsigned i = 1; i < 4; i++) {
        if (products[i] < *min)
            *min = products[i];
        if (products[i] > *max)
            *max = products[i];
    }
}

#endif
