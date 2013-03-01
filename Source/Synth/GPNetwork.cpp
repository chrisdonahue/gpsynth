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
allNodes()
{
    ID = -1;
    fitness = -1;
    root = r;
    depth = -1;
    asText = "";
}

GPNetwork::GPNetwork(std::string netstring) {
    ID = -1;
    fitness = -1;
    char* token;
    char* expr = (char*) malloc(sizeof(char) * (netstring.size() + 1));
    std::copy(netstring.begin(), netstring.end(), expr);
    expr[netstring.size()] = '\0';
    GPNode* root = createSubtree(strtok(expr, " )("));
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

void GPNetwork::mutate(GPParams* p) {
    asText = "";
    getRandomNetworkNode(p->rng)->mutate(p);
}

void GPNetwork::traceNetwork() {
    allNodes.clear();
    depth = 0;
    root->traceSubtree(&allNodes, NULL, &depth, 0);
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

    // assign nu pointers
    nu->parent = old->parent;
}

/*
    This method swaps node two into node one's spot in the network tree
*/
void GPNetwork::swap(GPNode* old, GPNode* nu) {
    /*
    // handle root case
    if (old == root)
        root = nu;
    
    // replace parent-child links
    if (old->parent != NULL) {
        if (old->parent->left == old)
            old->parent->left = nu;
        else if (old->parent->right == old)
            old->parent->right = nu;
        else
            std::cerr << "Bad parent-child links detected during node swap." << std::endl;
    }

    // assign nu pointers
    nu->parent = old->parent;
    nu->left = old->left;
    nu->right = old->right;
    */
}

// RECURSIVE CONSTRUCTION
GPNode* createSubtree(char* tokenized=strtok(NULL, " )(")) {
    std::cout << "----" << std::endl;
    char* t = tokenized; 
    std::cout << t << std::endl;
    GPNode* ret;
    if (strcmp(t, "+") == 0) {
        std::cout << "1" << std::endl;
        return new FunctionNode(add, createSubtree(), createSubtree());
    }
    else if (strcmp(t, "*") == 0) {
        std::cout << "2" << std::endl;
        return new FunctionNode(multiply, createSubtree(), createSubtree());
    }
    else if (strcmp(t, "sin") == 0) {
        std::cout << "3" << std::endl;
        return new FunctionNode(sine, createSubtree(), NULL);
    }
    else if (strcmp(t, "pi") == 0) {
        std::cout << "4" << std::endl;
        return new ValueNode(M_PI, -1);
    }
    else if (strcmp(t, "time") == 0) {
        std::cout << "5" << std::endl;
        return new ValueNode(-1, 0);
    }
    else if (strncmp(t, "v", 1) == 0) {
        std::cout << "6" << std::endl;
        return new ValueNode(-1, std::stoi(t + 1));
    }
    else {
        std::cout << "7" << std::endl;
        return new ValueNode(std::stod(t), -1);
    }
}
