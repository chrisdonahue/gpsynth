namespace GPFunction
{
//available network functions
/*
float add(GPNode* left, GPNode* right) {
    return left->evaluate() + right->evaluate();
}
float subtract(GPNode* left, GPNode* right) {
    return left->evaluate() - right->evaluate();
}
*/
double multiply(double left, double right) {
    return left * right;
}
/*
float modulo(GPNode* left, GPNode* right) {
    int mod = int(right->evaluate());
    if (mod > 0) {
        return fmod(left->evaluate(), mod);
    }
    else {
        return left->evaluate();
    }
}
float divide(GPNode* left, GPNode* right) {
    float denom = right->evaluate();
    if (denom != 0) {
        return float(left->evaluate()) / denom;
    }
    else {
        return 0.0;
    }
}
float pow(GPNode* left, GPNode* right) {
    float l = left->evaluate();
    float r = right->evaluate();
    if (l < 0) {
        r = int(r);
    }
    return pow(l, r);
}
*/
double sine(double left, double right) {
    return sin(left);
}
/*
float cosine(GPNode* left, GPNode* right) {
    return cos(left->evaluate());
}
*/
}

/*
// parameters
int populationSize;
int valueRange;
int LFORange;
int numPartials;
 
// IDs
int nextGenerationID;

// network containers
std::vector<GPNetwork&> networks;
std::vector<GPNetwork&> currentlyEvaluating;
std::set<GPNetwork&> evaluated;
std::set<GPNetwork&> selected;
*/


