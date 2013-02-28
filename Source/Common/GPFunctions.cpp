#include "GPFunctions.h"

double GPadd(double left, double right) {
    return left + right;
}
GPFunction add = { GPadd, "+", true, 0.0 };

double GPsubtract(double left, double right) {
    return left - right;
}
GPFunction subtract = { GPsubtract, "-", true, 0.0 };

double GPmultiply(double left, double right) {
    return left * right;
}
GPFunction multiply = { GPmultiply, "*", true, 0.0 };

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

double GPsine(double left, double right) {
    return sin(left);
}
GPFunction sine = { GPsine, "sin", false, 0.0 };

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
