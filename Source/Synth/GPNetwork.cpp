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
    minimum((-1) * std::numeric_limits<float>::infinity()), maximum(std::numeric_limits<float>::infinity()),
    preparedToRender(false),
    renderRoot(new SilenceNode()), root(r), allNodes(0), allMutatableParams(0)
{
}

GPNetwork::GPNetwork(GPRandom* rng, std::string netstring) :
    ID(-1), origin("string"), height(-1), fitness(-1),
    minimum((-1) * std::numeric_limits<float>::infinity()), maximum(std::numeric_limits<float>::infinity()),
    preparedToRender(false),
    renderRoot(new SilenceNode()), allNodes(0), allMutatableParams(0)
{
	root = createNode(netstring, rng);
}

GPNetwork::~GPNetwork() {
    if (!preparedToRender) {
        delete root;
    }
    delete renderRoot;
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

void GPNetwork::evaluateBlockPerformance(unsigned firstFrameNumber, unsigned numSamples, float* sampleTimes, unsigned numConstantVariables, float* constantVariables, float* buffer) {
    renderRoot->evaluateBlockPerformance(firstFrameNumber, numSamples, sampleTimes, numConstantVariables, constantVariables, buffer);
}


std::string GPNetwork::toString(unsigned precision) {
    std::stringstream ss;
    ss.precision(precision);
    root->toString(ss);
    return ss.str();
}

GPNode* GPNetwork::getRoot() {
    return root;
}

bool GPNetwork::equals(GPNetwork* other, unsigned precision) {
    return toString(precision).compare(other->toString(precision)) == 0;
}

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

/*
    =======
    HELPERS
    =======
*/

// renderRoot = silence and root = realroot whenever preparedToRender is false
// renderRoot = realroot and root = realroot whenever preparedToRender is true
void GPNetwork::prepareToRender(float sr, unsigned blockSize, unsigned maxFrameNumber, float maxTime) {
    doneRendering();
    if (!preparedToRender) {
        delete renderRoot;
    }
    root->setRenderInfo(sr, blockSize, maxFrameNumber, maxTime);
    root->updateMutatedParams();
    minimum = root->minimum;
    maximum = root->maximum;
    renderRoot = root;
    preparedToRender = true;
}

void GPNetwork::doneRendering() {
    if (preparedToRender) {
        root->doneRendering();
        minimum = (-1) * std::numeric_limits<float>::infinity();
        maximum = std::numeric_limits<float>::infinity();
        renderRoot = new SilenceNode();
        preparedToRender = false;
    }
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
    /*
    // assign nu parent pointer
    nu->parent = old->parent;
    */
}

void GPNetwork::ephemeralRandom(GPRandom* r) {
    root->ephemeralRandom(r);
}