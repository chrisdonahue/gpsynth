#ifndef GPFUNCTION_H
#define GPFUNCTION_H

#include <cmath>

namespace GPFunction
{
    double add(double left, double right) {
        return left + right;
    }
    double subtract(double left, double right) {
        return left - right;
    }
    double multiply(double left, double right) {
        return left * right;
    }
    double modulo(double left, double right) {
        int mod = int(right);
        if (mod > 0) {
            return fmod(left, mod);
        }
        else {
            return left;
        }
    }
    double divide(double left, double right) {
        double denom = right;
        if (denom != 0) {
            return double(left) / denom;
        }
        else {
            return INFINITY;
        }
    }
    double pow(double left, double right) {
        double r;
        if (left < 0) {
            r = int(r);
        }
        else if (left == 0) {
            return 0;
        }
        return pow(left, r);
    }
    double sine(double left, double right) {
        return sin(left);
    }
    double cosine(double left, double right) {
        return cos(left);
    }
}

#endif
