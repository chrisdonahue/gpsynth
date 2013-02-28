#ifndef GPFUNCTIONS_H
#define GPFUNCTIONS_H

#include <cmath>
#include <string>

struct GPFunction {
    double (*function)(double, double);
    char symbol[4];
    bool isBinary;
    double identity;
};

extern GPFunction add;
extern GPFunction subtract;
extern GPFunction multiply;
extern GPFunction divide;
extern GPFunction modulo;
extern GPFunction power;
extern GPFunction sine;
extern GPFunction cosine;

extern double GPadd(double, double);
extern double GPsubtract(double, double);
extern double GPmultiply(double, double);
extern double GPdivide(double, double);
extern double GPmodulo(double, double);
extern double GPpower(double, double);
extern double GPsine(double, double);
extern double GPcosine(double, double);

#endif
