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

GPNetwork::GPNetwork(const GPParams& p, GPNode& r) {
    params = p;
    networkID = params.nextNetworkID++;
    asText = "";
    root = r;
}

GPNetwork::~GPNetwork() {
    delete networkID;
    delete string;
    delete root;
}

GPNetwork& GPNetwork::getCopy() {
    return new GPNetwork(params, root.getCopy());
}

float GPNetwork::evaluate() {
    return root.evaluate();
}

std::string GPNetwork::toString() {
    if (asText.equals("")) {
        asText = root.toString();
    }
    return asText;
}

void GPNetwork::mutate() {
    // IMPLEMENT LATER
}

GPNetwork& GPNetwork::reproduce(const GPNetwork& partner) {
    GPNetwork& sperm = root.getCopy();
    GPNetwork& egg = partner.getRoot().getCopy();
}

const GPNode& GPNetwork::getRoot() {
    return root;
}

/*
    ===============
    PRIVATE METHODS
    ===============
*/
void GPNetwork::swap(GPNode& one, GPNode& two) {
    // IMPLEMENT LATER
}
