#ifndef GPFUNCTIONS_H
#define GPFUNCTIONS_H

#include <cmath>
#include <string>

struct GPFunction {
    double (*function)(double, double);
    double (*calculateRange)(double*, double*, double, double, double, double);
    char symbol[4];
    bool isBinary;
    double identity;
};

extern GPFunction add;
extern GPFunction multiply;
extern GPFunction sine;
//extern GPFunction subtract;
//extern GPFunction divide;
//extern GPFunction modulo;
//extern GPFunction power;
//extern GPFunction cosine;

extern double GPaddCalculateRange(double*, double*, double, double, double, double);
extern double GPadd(double, double);
extern double GPmultiplyCalculateRange(double*, double*, double, double, double, double);
extern double GPmultiply(double, double);
extern double GPsineCalculateRange(double*, double*, double, double, double, double);
extern double GPsine(double, double);

//extern double GPcosine(double, double);
//extern double GPdivide(double, double);
//extern double GPmodulo(double, double);
//extern double GPpower(double, double);
//extern double GPsubtractCalculateRange(double*, double*, double, double, double, double);
//extern double GPsubtract(double, double);
/*
class GPFunction {
  GPFunction();
  ~GPFunction();
  virtual double function(double, double);
  virtual double calculateRange(double, double);
};

template<GPFunction >
function_traits
{

}

function_traits<add >
{
  int param = 0;
  string param = "+";
}

class add : GPFunction {
  double function(double left, double right) {
    return left + right;
  }
}
*/

#endif
