#ifndef GPHELPERS_H
#define GPHELPERS_H

#include <cmath>

//extern void GPaddCalculateRange(double*, double*, double, double, double, double);

// http://stackoverflow.com/questions/12931115/algorithm-to-map-an-interval-to-a-smaller-interval
// output is a number m to multiply the value by and a number to add to that product
inline void continuousMapRange(double w, double x, double y, double z, double* m, double* b) {
    if ((w+0.0) == 0 && std::signbit(w) == 1) {
        w *= -1;
    }
    *m = (z - y)/(x - w);
    *b = (y - w * (*m));
}

#endif
