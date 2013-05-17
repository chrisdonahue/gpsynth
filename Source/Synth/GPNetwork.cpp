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
    std::vector<std::string> tokens = split(netstring, " }{)(");
    unsigned index = 0;
    root = createSubtree(tokens, &index, rng);
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
void GPNetwork::prepareToRender(float sr, unsigned blockSize, float maxTime) {
    doneRendering();
    if (!preparedToRender) {
        delete renderRoot;
    }
    root->setRenderInfo(sr, blockSize, maxTime);
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

/*
    ====================
    S-EXPRESSION PARSING
    ====================
*/

GPMutatableParam* createMutatableParam(std::vector<std::string> tokens, unsigned* currentIndex, bool ismutatable, std::string type) {
    // get tokens
    std::string tag = tokens[consume];
    std::string minstr = tokens[consume];
    std::string valstr = tokens[consume];
    std::string maxstr = tokens[consume];

    // if the param is continuous
    if (tag.compare("c") == 0) {
        float min = (float) std::atof(minstr.c_str());
        float val = (float) std::atof(valstr.c_str());
        float max = (float) std::atof(maxstr.c_str());
        return new GPMutatableParam(type, ismutatable, val, min, max);
    }
    // else if the param is discrete
    else if (tag.compare("d") == 0) {
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

GPNode* createSubtree(std::vector<std::string> tokens, unsigned* currentIndex, GPRandom* rng) {
    // fields for tokens
    std::string type = tokens[consume];
    std::string last;

    // radius around constant nodes (any nonzero value should be the same)
    // float constantRadius = 1;

    // ADSR nodes
    if (type.compare("adsr") == 0) {
        return new ADSRNode(true, createMutatableParam(tokenizer, true, "adsrdelay"), createMutatableParam(tokenizer, true, "adsrattack"), createMutatableParam(tokenizer, true, "adsrattackheight"), createMutatableParam(tokenizer, true, "adsrdecay"), createMutatableParam(tokenizer, true, "adsrsustain"), createMutatableParam(tokenizer, true, "adsrsustainheight"), createMutatableParam(tokenizer, true, "adsrrelease"), NULL);
    }
    else if (type.compare("adsr*") == 0) {
        return new ADSRNode(false, createMutatableParam(tokenizer, true, "adsrdelay"), createMutatableParam(tokenizer, true, "adsrattack"), createMutatableParam(tokenizer, true, "adsrattackheight"), createMutatableParam(tokenizer, true, "adsrdecay"), createMutatableParam(tokenizer, true, "adsrsustain"), createMutatableParam(tokenizer, true, "adsrsustainheight"), createMutatableParam(tokenizer, true, "adsrrelease"), createSubtree(tokenizer, subtreeArgs));
    }
    // constant nodes
    else if (type.compare("pi") == 0) {
        return new ConstantNode(true, true, NULL, NULL);
    }
    else if (type.compare("pi*") == 0) {
        return new ConstantNode(false, true, NULL, createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("const") == 0) {
        return new ConstantNode(true, false, createMutatableParam(tokenizer, true, "constantvalue"), NULL);
    }
    else if (type.compare("const*") == 0) {
        return new ConstantNode(false, false, createMutatableParam(tokenizer, true, "constantvalue"), createSubtree(tokenizer, subtreeArgs));
    }
    // function nodes
    else if (type.compare("+") == 0) {
        return new AddNode(createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("*") == 0) {
        return new MultiplyNode(createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("sin") == 0) {
        return new SineNode(createSubtree(tokenizer, subtreeArgs));
    }
    // LFO nodes
    else if (type.compare("lfo") == 0) {
        return new LFONode(true, createMutatableParam(tokenizer, true, "lforate"), NULL);
    }
    else if (type.compare("lfo*") == 0) {
        return new LFONode(true, createMutatableParam(tokenizer, true, "lforate"), createSubtree(tokenizer, subtreeArgs));
    }
    // mixer nodes
    else if (type.compare("if") == 0) {
        return new MixerNode(false, createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
    }
    else if (type.compare("mix") == 0) {
        return new MixerNode(true, createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs), createSubtree(tokenizer, subtreeArgs));
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
    // silence node
    else if (type.compare("silence") == 0) {
        return new SilenceNode();
    }
    // time node
    else if (type.compare("time") == 0) {
        return new TimeNode();
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
