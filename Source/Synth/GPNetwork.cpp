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

GPNetwork::GPNetwork(GPParams* p, GPRandom* rng, double sr, std::string netstring) :
    ID(-1), origin("string"), height(-1), fitness(-1),
    minimum(-INFINITY), maximum(INFINITY),
    allNodes(0), allMutatableParams(0)
{
    std::vector<std::string> tokens = split(netstring, " }{)(");
    for (unsigned i = 0; i < tokens.size(); i++) {
        //std::cout << tokens[i] << std::endl;
    }
    unsigned index = 0;
    root = createSubtree(tokens, &index, p, rng, sr);
}

GPNetwork::~GPNetwork() {
    delete root;
}

GPNetwork* GPNetwork::getCopy(std::string neworigin) {
    GPNetwork* copy = new GPNetwork(root->getCopy(), neworigin);
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

void GPNetwork::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    root->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
    //printf("min: %.2lf max: %.2lf\n", root->minimum, root->maximum);
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

/*
    ====================
    S-EXPRESSION PARSING
    ====================
*/

// macro to "consume" a token when we observe it
#define consume (*currentIndex)++

GPMutatableParam* createMutatableParam(std::vector<std::string> tokens, unsigned* currentIndex, bool ismutatable, std::string type) {
    // get tokens
    std::string tag = tokens[consume];
    std::string minstr = tokens[consume];
    std::string valstr = tokens[consume];
    std::string maxstr = tokens[consume];

    // if the param is continuous
    if (tag.compare(0, 3, "c") == 0) {
        double min = std::atof(minstr.c_str());
        double val = std::atof(valstr.c_str());
        double max = std::atof(maxstr.c_str());
        return new GPMutatableParam(type, ismutatable, val, min, max);
    }
    // else if the param is discrete
    else if (tag.compare(0, 3, "d") == 0) {
        int min = std::atoi(minstr.c_str());
        int val = std::atoi(valstr.c_str());
        int max = std::atoi(maxstr.c_str());
        return new GPMutatableParam(type, ismutatable, val, min, max);
    }
    // else something went wrong
    else {
        std::cerr << "Tried to create mutatable param from incorrectly formatted string" << std::endl;
        return NULL;
    }
}

// macro for standard structure of vector of tokens/index
#define tokenizer tokens, currentIndex
// macro for calling createSubtree easily
#define subtreeArgs p, rng, sr

GPNode* createSubtree(std::vector<std::string> tokens, unsigned* currentIndex, GPParams* p, GPRandom* rng, double sr) {
    // fields for tokens
    std::string type = tokens[consume];
    std::string last;

    std::cout << type << std::endl;

    // radius around constant nodes (any nonzero value should be the same)
    double constantRadius = 1;

    // ADSR nodes
    if (type.compare("adsr") == 0) {
        return new ADSRNode(true, true, sr, createMutatableParam(tokenizer, true, "adsrdelay"), createMutatableParam(tokenizer, true, "adsrattack"), createMutatableParam(tokenizer, true, "adsrattackheight"), createMutatableParam(tokenizer, true, "adsrdecay"), createMutatableParam(tokenizer, true, "adsrsustain"), createMutatableParam(tokenizer, true, "adsrsustainheight"), createMutatableParam(tokenizer, true, "adsrrelease"), NULL);
    }
    else if (type.compare("adsr*") == 0) {
        return new ADSRNode(false, true, sr, createMutatableParam(tokenizer, true, "adsrdelay"), createMutatableParam(tokenizer, true, "adsrattack"), createMutatableParam(tokenizer, true, "adsrattackheight"), createMutatableParam(tokenizer, true, "adsrdecay"), createMutatableParam(tokenizer, true, "adsrsustain"), createMutatableParam(tokenizer, true, "adsrsustainheight"), createMutatableParam(tokenizer, true, "adsrrelease"), createSubtree(tokenizer, subtreeArgs));
    }
    // constant nodes
    else if (type.compare("pi") == 0) {
        return new ConstantNode(true, new GPMutatableParam("pi", false, M_PI, M_PI - constantRadius, M_PI + constantRadius), NULL);
    }
    else if (type.compare("pi*") == 0) {
        return new ConstantNode(false, new GPMutatableParam("pi", false, M_PI, M_PI - constantRadius, M_PI + constantRadius), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("const") == 0) {
        return new ConstantNode(true, createMutatableParam(tokenizer, true, "constantvalue"), NULL);
    }
    else if (type.compare("const*") == 0) {
        return new ConstantNode(false, createMutatableParam(tokenizer, true, "constantvalue"), createSubtree(tokenizer, subtreeArgs));
    }
    // function nodes
    else if (type.compare("+") == 0) {
        return new FunctionNode(add, createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("*") == 0) {
        return new FunctionNode(multiply, createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("sin") == 0) {
        return new FunctionNode(sine, createSubtree(tokenizer, subtreeArgs), NULL);
    }
    // time node
    else if (type.compare("time") == 0) {
        return new TimeNode(createMutatableParam(tokenizer, true, "timemax"));
    }
    // noise node
    else if (type.compare("whitenoise") == 0) {
        return new NoiseNode(rng);
    }
    // oscil node
    else if (type.compare("osc") == 0) {
        return new OscilNode(true, createMutatableParam(tokenizer, false, "oscilvarnum"), createMutatableParam(tokenizer, true, "oscilpartial"), NULL, NULL);
    }
    else if (type.compare("fm") == 0) {
        return new OscilNode(false, createMutatableParam(tokenizer, false, "oscilvarnum"), createMutatableParam(tokenizer, true, "oscilpartial"), createMutatableParam(tokenizer, true, "oscilindex"), createSubtree(tokenizer, subtreeArgs));
    }
    // variable node
    else if (type.compare("var") == 0) {
        return new VariableNode(createMutatableParam(tokenizer, false, "variablevarnum"), createMutatableParam(tokenizer, true, "variablerange"));
    }
    else {
        std::cerr << "Tried to build S-expression from improperly formatted string" << std::endl;
        return NULL;
    }
}
