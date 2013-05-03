#ifndef GPHELPERS_H
#define GPHELPERS_H

#include <cmath>
#include <string>
#include <sstream>
#include <vector>

//extern void GPaddCalculateRange(double*, double*, double, double, double, double);

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

inline void intervalMultiply(float* min, float* max, float zeromin, float zeromax, float onemin, float onemax) {
    float products[4];
    products[0] = zeromin * onemin;
    products[1] = zeromin * onemax;
    products[2] = zeromax * onemin;
    products[3] = zeromax * onemax;

	*min = products[0];
	*max = products[0];

    for (char i = 1; i < 4; i++) {
        if (products[i] < *min)
            *min = products[i];
        if (products[i] > *max)
            *max = products[i];
    }
}

// http://stackoverflow.com/a/236803
inline std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char* delims) {
    std::size_t prev = 0, pos;
    while ((pos = s.find_first_of(delims, prev)) != std::string::npos)
    {
        if (pos > prev)
            elems.push_back(s.substr(prev, pos-prev));
        prev = pos+1;
    }
    return elems;
}

inline std::vector<std::string> split(const std::string &s, const char* delims) {
    std::vector<std::string> elems;
    split(s, elems, delims);
    return elems;
}

#endif
