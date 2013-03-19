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

GPNetwork::GPNetwork(GPNode* r) :
allNodes(), allMutatableParams()
{
    ID = -1;
    fitness = -1;
    root = r;
    depth = -1;
    asText = "";
}

GPNetwork::GPNetwork(GPParams* p, std::string netstring) {
    ID = -1;
    fitness = -1;
    char* token;
    char* expr = (char*) malloc(sizeof(char) * (netstring.size() + 1));
    std::copy(netstring.begin(), netstring.end(), expr);
    expr[netstring.size()] = '\0';
    root = createSubtree(p, strtok(expr, " )("));
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

double GPNetwork::evaluate(double* t, double* v) {
    return root->evaluate(t, v);
}

void GPNetwork::evaluateBlock(double* t, unsigned nv, double* v, unsigned n, float* buffer) {
    root->evaluateBlock(t, nv, v, n, buffer);
}

int GPNetwork::getDepth() {
    return depth;
}

std::string GPNetwork::toString() {
    return asText;
}

GPNode* GPNetwork::getRoot() {
    return root;
}

bool GPNetwork::equals(GPNetwork* other) {
    return toString().compare(other->toString()) == 0;
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
    depth = 0;
    root->traceSubtree(&allNodes, &allMutatableParams, NULL, &depth, 0);
    asText = root->toString();
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
        if (old->parent->left == old)
            old->parent->left = nu;
        else if (old->parent->right == old)
            old->parent->right = nu;
        else
            std::cerr << "Bad parent-child links detected during subtree replacement." << std::endl;
    }

    // assign nu parent pointer
    nu->parent = old->parent;
}

void GPNetwork::updateMutatedParams() {
    root->updateMutatedParams();
    asText = root->toString();
}

void GPNetwork::ephemeralRandom(GPRandom* r) {
    root->ephemeralRandom(r);
}

// RECURSIVE CONSTRUCTION
GPNode* createSubtree(GPParams* p, char* tokenized=strtok(NULL, " )(")) {
    //std::cout << "----" << std::endl;
    char* t = tokenized; 
    //std::cout << t << std::endl;
    if (strcmp(t, "+") == 0) {
        return new FunctionNode(add, createSubtree(p), createSubtree(p));
    }
    else if (strcmp(t, "*") == 0) {
        return new FunctionNode(multiply, createSubtree(p), createSubtree(p));
    }
    else if (strcmp(t, "sin") == 0) {
        return new FunctionNode(sine, createSubtree(p), NULL);
    }
    else if (strcmp(t, "pi") == 0) {
        return new ConstantNode(new GPMutatableParam("pi", false, M_PI, 0.0, 0.0));
    }
    else if (strcmp(t, "time") == 0) {
        return new TimeNode();
    }
    else if (strncmp(t, "v", 1) == 0) {
        return new VariableNode(std::stoi(t + 1));
    }
    else {
        return new ConstantNode(new GPMutatableParam("constantvalue", true, std::stod(t), p->valueNodeMinimum, p->valueNodeMaximum));
    }
}
