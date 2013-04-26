/*
  ==============================================================================

    GPNetwork.cpp
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPNetwork.h"

/*
    ============
    CONSTRUCTION
    ============
*/

GPNetwork::GPNetwork(GPNode* r, std::string o) :
    ID(-1), origin(o), height(-1), fitness(-1),
    minimum(-INFINITY), maximum(INFINITY),
    root(r), allNodes(0), allMutatableParams(0)
{
}

GPNetwork::GPNetwork(GPParams* p, double sr, std::string netstring) :
    ID(-1), origin("string"), height(-1), fitness(-1),
    minimum(-INFINITY), maximum(INFINITY),
    allNodes(0), allMutatableParams(0)
{
    ID = -1;
    fitness = -1;
    char* expr = (char*) malloc(sizeof(char) * (netstring.size() + 1));
    std::copy(netstring.begin(), netstring.end(), expr);
    expr[netstring.size()] = '\0';
    root = createSubtree(p, sr, strtok(expr, " )("));
    free(expr);
}

GPNetwork::~GPNetwork() {
    delete root;
}

GPNetwork* GPNetwork::getCopy() {
    GPNetwork* copy = new GPNetwork(root->getCopy());
    return copy;
}

/*
    ===========
    EXAMINATION
    ===========
*/

void GPNetwork::evaluateBlock(unsigned fn, double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    double min = std::numeric_limits<double>::min();
    double max = std::numeric_limits<double>::max();
    root->evaluateBlock(fn, t, nv, v, &min, &max, n, buffer);
    //printf("min: %.2lf max: %.2lf\n", min, max);
}

void GPNetwork::evaluateBlockPerformance(unsigned fn, float* t, unsigned nv, float* v, unsigned n, float* buffer) {
	float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();
    root->evaluateBlockPerformance(fn, t, nv, v, &min, &max, n, buffer);
    //printf("min: %.2lf max: %.2lf\n", min, max);
}


std::string GPNetwork::toString(bool printRange, unsigned precision) {
    std::stringstream ss;
    ss.precision(precision);
    root->toString(printRange, ss);
    return ss.str();
}

GPNode* GPNetwork::getRoot() {
    return root;
}

bool GPNetwork::equals(GPNetwork* other) {
    return toString(true, 10).compare(other->toString(true, 10)) == 0;
}

/*
    ========
    MUTATION
    ========
*/

GPNode* GPNetwork::getRandomNetworkNode(GPRandom* r) {
    if (allNodes.size() == 0) {
        std::cerr << "Tried to get a random node of an empty or untraced network. This shouldn't happen" << std::endl;
        return NULL;
    }
    return allNodes[r->random(allNodes.size())];
}

std::vector<GPMutatableParam*>* GPNetwork::getAllMutatableParams() {
    return &allMutatableParams;
}

void GPNetwork::traceNetwork() {
    allNodes.clear();
    allMutatableParams.clear();
    root->trace(&allNodes, &allMutatableParams, NULL, &height, 0);
}

/*
    This method replaces the subtree rooted at node old with node new's
*/
void GPNetwork::replaceSubtree(GPNode* old, GPNode* nu) {
    // handle root case
    if (old == root) {
        root = nu;
    }
    else {
        // replace parent-child links
        bool replacedLink = false;
        for (unsigned i = 0; i < old->parent->arity; i++) {
            if (old->parent->descendants[i] == old) {
                old->parent->descendants[i] = nu;
                replacedLink = true;
            }
        }
        if (!replacedLink)
            std::cerr << "Bad parent-child links detected during subtree replacement." << std::endl;
    }

    // assign nu parent pointer
    nu->parent = old->parent;
}

void GPNetwork::updateMutatedParams() {
    root->updateMutatedParams();
}

void GPNetwork::ephemeralRandom(GPRandom* r) {
    root->ephemeralRandom(r);
    root->updateMutatedParams();
}

GPMutatableParam* createMutatableParam(char* tokenized=strtok(NULL, " {},")) {
    std::cout << "----" << std::endl;
    char* t = tokenized;
    std::cout << t << std::endl;
    // CONTINUOUS
    if (strcmp(t, "C:") == 0) {
        double min = std::atof(++t);
        double val = std::atof(++t);
        double max = std::atof(++t);
        return new GPMutatableParam(std::string(""), true, val, min, max);
    }
    // DISCRETE
    else if (strcmp(t, "D:") == 0) {
        int min = std::atoi(++t);
        int val = std::atoi(++t);
        int max = std::atoi(++t);
        return new GPMutatableParam(std::string(""), true, val, min, max);
    }
}

// RECURSIVE CONSTRUCTION
GPNode* createSubtree(GPParams* p, double sr, char* tokenized=strtok(NULL, " )(")) {
    //std::cout << "----" << std::endl;
    char* t = tokenized;
    double centerConstant = 1;
    //std::cout << t << std::endl;
    // FUNCTION NODES
    if (strcmp(t, "+") == 0) {
        return new FunctionNode(add, createSubtree(p, sr), createSubtree(p, sr));
    }
    else if (strcmp(t, "*") == 0) {
        return new FunctionNode(multiply, createSubtree(p, sr), createSubtree(p, sr));
    }
    else if (strcmp(t, "sin") == 0) {
        return new FunctionNode(sine, createSubtree(p, sr), NULL);
    }
    // ADSR NODES
    else if (strcmp(t, "adsr*") == 0) {
        return new ADSRNode(false, false, sr, createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createSubtree(p, sr));
    }
    else if (strcmp(t, "adsr") == 0) {
        return new ADSRNode(false, true, sr, createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), createMutatableParam(), NULL);
    }
    // TIME NODE
    else if (strcmp(t, "time") == 0) {
        return new TimeNode();
    }
    // VARIABLE NODE
    else if (strncmp(t, "v", 1) == 0) {
        return new VariableNode(std::atoi(t + 1), 0.0, 0.0);
    }
    // CONSTANT NODES
    else if (strcmp(t, "pi") == 0) {
        return new ConstantNode(new GPMutatableParam("pi", false, M_PI, M_PI - centerConstant, M_PI + centerConstant));
    }
    else {
        return new ConstantNode(createMutatableParam());
    }
}
