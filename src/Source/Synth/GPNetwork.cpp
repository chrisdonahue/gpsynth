/*
  ==============================================================================

    GPNetwork.cpp
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPNetwork.h"

/*
    ==============
    PUBLIC METHODS
    ==============
*/

GPNetwork::GPNetwork(GPInfo* i, GPNode* r) {
    info = i;
    networkID = info->nextNetworkID++;
    asText = "";
    root = r;
}

GPNetwork::~GPNetwork() {
    delete &networkID;
    delete &asText;
    delete root;
}

GPNetwork* GPNetwork::getCopy() {
    return new GPNetwork(info, root->getCopy());
}

double GPNetwork::evaluate(double* t, float* f) {
    return root->evaluate(t, f);
}

std::string GPNetwork::toString() {
    if (asText.compare("") == 0) {
        asText = root->toString();
    }
    return asText;
}

void GPNetwork::mutate() {
    // IMPLEMENT LATER
}

GPNetwork* GPNetwork::reproduce(GPNetwork* partner) {
    GPNetwork* sperm = this->getCopy();
    GPNetwork* egg = partner->getCopy();
    return sperm;
}

GPNode* GPNetwork::getRoot() {
    return root;
}

/*
    ===============
    PRIVATE METHODS
    ===============
*/
void GPNetwork::swap(GPNode* one, GPNode* two) {
    // IMPLEMENT LATER
}
