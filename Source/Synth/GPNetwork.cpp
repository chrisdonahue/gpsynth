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

GPNetwork::GPNetwork(int id, GPNode* r) :
allNodes()
{
    ID = id;
    asText = "";
    root = r;
    traceNetwork();
}

GPNetwork::~GPNetwork() {
    delete &ID;
    delete &asText;
    delete root;
}

GPNetwork* GPNetwork::getCopy() {
    return new GPNetwork(ID, root->getCopy());
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

GPNode* GPNetwork::getRoot() {
    return root;
}

/*
    ========
    MUTATION
    ========
*/

void GPNetwork::mutateAddNode(GPNodeParams* p, GPNode* newnode) {
    return;
}

void GPNetwork::mutate(GPNodeParams* p) {
    return;
}

void GPNetwork::traceNetwork() {
    return;
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
