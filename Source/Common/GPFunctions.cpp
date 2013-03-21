#include "GPFunctions.h"

/*
    +
*/
double GPaddCalculateRange(double* min, double* max, double zeromin, double zeromax, double onemin, double onemax) {
    *min = zeromin + onemin;
    *max = zeromax + onemax;
}
double GPadd(double left, double right) {
    return left + right;
}
GPFunction add = { GPadd, GPaddCalculateRange, "+", true, 0.0 };

/*
    *
*/
double GPmultiplyCalculateRange(double* min, double* max, double zeromin, double zeromax, double onemin, double onemax) {
    *min = zeromin * onemin;
    *max = zeromax * onemax;
}
double GPmultiply(double left, double right) {
    return left * right;
}
GPFunction multiply = { GPmultiply, GPmultiplyCalculateRange, "*", true, 0.0 };

/*
    sin
*/
double GPsineCalculateRange(double* min, double* max, double zeromin, double zeromax, double onemin, double onemax) {
    *min = -1;
    *max = 1;
}
double GPsine(double left, double right) {
    return sin(left);
}
GPFunction sine = { GPsine, GPsineCalculateRange, "sin", false, 0.0 };





/*
double GPsubtract(double left, double right) {
    return left - right;
}
GPFunction subtract = { GPsubtract, "-", true, 0.0 };
*/

/*
double GPdivide(double left, double right) {
    double denom = right;
    if (denom != 0) {
        return double(left) / denom;
    }
    else {
        return INFINITY;
    }
}
GPFunction divide = { GPdivide, "/", true, 0.0 };
*/

/*
double GPmodulo(double left, double right) {
    int mod = int(right);
    if (mod > 0) {
        return fmod(left, mod);
    }
    else {
        return left;
    }
}
GPFunction add = { GPsubtract, "-", true, 0.0 };


double GPpower(double left, double right) {
    double r;
    if (left < 0) {
        r = int(r);
    }
    else if (left == 0) {
        return 0;
    }
    return pow(left, r);
}
GPFunction add = { GPsubtract, "-", true, 0.0 };


double GPcosine(double left, double right) {
    return cos(left);
}
GPFunction add = { GPsubtract, "-", true, 0.0 };
*/
