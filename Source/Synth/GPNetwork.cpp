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
    depth = -1;
    fitness = NAN;
    asText = "";
    root = r;
}

GPNetwork::~GPNetwork() {
    delete root;
}

GPNetwork* GPNetwork::getCopy() {
  GPNetwork* copy = net GPNetwork(root->getCopy());
  copy->fitness = fitness;
  copy->asText = asText;
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

std::string GPNetwork::toString() {
    if (asText.compare("") == 0) {
        asText = root->toString();
    }
    return asText;
}

int GPNetwork::getDepth() {
    if (depth == -1) {
        depth = root->getHeight(0);
    }
    return depth;
}

GPNode* GPNetwork::getRoot() {
    return root;
}

/*
    ========
    MUTATION
    ========
*/

GPNode* GPNetwork::getRandomNetworkNode(GPRandom* r) {
    return allNodes[(int) (r->random() * allNodes.size())];
}

void GPNetwork::mutate(GPNodeParams* p) {
    getRandomNetworkNode(p)->mutate(p);
}

void GPNetwork::traceNetwork() {
    allNodes.clear();
    root->parent = NULL;
    root->traceLineage(&allNodes);
}

/*
    This method replaces the subtree rooted at node old with node new's
*/
void GPNetwork::replaceSubtree(GPNode* old, GPNode* nu) {
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
            std::cerr << "Bad parent-child links detected during subtree replacement." << std::endl;
    }

    // assign nu pointers
    nu->parent = old->parent;
}

/*
    This method swaps node two into node one's spot in the network tree
*/
void GPNetwork::swap(GPNode* old, GPNode* nu) {
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
}
