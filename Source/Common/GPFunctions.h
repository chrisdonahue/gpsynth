#ifndef GPFUNCTIONS_H
#define GPFUNCTIONS_H

#include <cmath>
#include <string>

struct GPFunction {
    double (function*)(double, double);
    std::string* symbol;
    bool isBinary;
    double identity;
}

extern GPFunction add;
extern GPFunction subtract;
extern GPFunction multiply;
extern GPFunction divide;
extern GPFunction modulo;
extern GPFunction power;
extern GPFunction sine;
extern GPFunction cosine;

extern double add(double, double);
extern double subtract(double, double);
extern double multiply(double, double);
extern double modulo(double, double);
extern double divide(double, double);
extern double power(double, double);
extern double sine(double, double);
extern double cosine(double, double);

#endif
