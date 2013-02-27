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
    depth = -1;
    asText = "";
    root = r;
}

GPNetwork::~GPNetwork() {
    delete root;
}

GPNetwork* GPNetwork::getCopy() {
  GPNetwork* copy = new GPNetwork(root->getCopy());
  copy->traceNetwork();
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

bool GPNetwork::equals(GPNetwork* other) {
    return false;
}

/*
    ========
    MUTATION
    ========
*/

GPNode* GPNetwork::getRandomNetworkNode(GPRandom* r) {
    if (allNodes.size() == 0)
        return NULL;
    return allNodes[r->random(allNodes.size())];
}

void GPNetwork::mutate(GPNodeParams* p) {
    asText = "";
    getRandomNetworkNode(p->rng)->mutate(p);
}

void GPNetwork::traceNetwork() {
    allNodes.clear();
    // TODO: inherently encode toString and getDepth in traceLineage
    root->parent = NULL;
    root->traceLineage(&allNodes);
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
    
    // clear out info
    depth = -1;
    asText = "";
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
