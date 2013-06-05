/*
  ==============================================================================

    GPNetwork.cpp
    Created: 6 Feb 2013 11:05:02am
    Author:  cdonahue

  ==============================================================================
*/

#include "GPParser.h"

/*
    =========
    TOKENIZER
    =========
*/

// http://stackoverflow.com/a/236803
std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char* delims) {
    std::size_t prev = 0, pos;
    while ((pos = s.find_first_of(delims, prev)) != std::string::npos)
    {
        if (pos > prev)
            elems.push_back(s.substr(prev, pos-prev));
        prev = pos+1;
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, const char* delims) {
    std::vector<std::string> elems;
    split(s, elems, delims);
    return elems;
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

GPNode* createNode(std::vector<std::string> tokens, unsigned* currentIndex, GPRandom* rng) {
    // fields for tokens
    std::string type = tokens[consume];
    std::string last;

    // radius around constant nodes (any nonzero value should be the same)
    // float constantRadius = 1;

    // ADSR nodes
    if (type.compare("adsr") == 0) {
		GPMutatableParam* adsrdelay = createMutatableParam(tokenizer, true, "adsrdelay");
		GPMutatableParam* adsrattack = createMutatableParam(tokenizer, true, "adsrattack");
		GPMutatableParam* adsrattackheight = createMutatableParam(tokenizer, true, "adsrattackheight");
		GPMutatableParam* adsrdecay = createMutatableParam(tokenizer, true, "adsrdecay");
		GPMutatableParam* adsrsustain = createMutatableParam(tokenizer, true, "adsrsustain");
		GPMutatableParam* adsrsustainheight = createMutatableParam(tokenizer, true, "adsrsustainheight");
		GPMutatableParam* adsrrelease = createMutatableParam(tokenizer, true, "adsrrelease");
        return new ADSRNode(true, adsrdelay, adsrattack, adsrattackheight, adsrdecay, adsrsustain, adsrsustainheight, adsrrelease, NULL);
    }
    else if (type.compare("adsr*") == 0) {
		GPMutatableParam* adsrdelay = createMutatableParam(tokenizer, true, "adsrdelay");
		GPMutatableParam* adsrattack = createMutatableParam(tokenizer, true, "adsrattack");
		GPMutatableParam* adsrattackheight = createMutatableParam(tokenizer, true, "adsrattackheight");
		GPMutatableParam* adsrdecay = createMutatableParam(tokenizer, true, "adsrdecay");
		GPMutatableParam* adsrsustain = createMutatableParam(tokenizer, true, "adsrsustain");
		GPMutatableParam* adsrsustainheight = createMutatableParam(tokenizer, true, "adsrsustainheight");
		GPMutatableParam* adsrrelease = createMutatableParam(tokenizer, true, "adsrrelease");
		GPNode* signal = createNode(tokenizer, subtreeArgs);
        return new ADSRNode(false, adsrdelay, adsrattack, adsrattackheight, adsrdecay, adsrsustain, adsrsustainheight, adsrrelease, signal);
    }
    // constant nodes
    else if (type.compare("pi") == 0) {
        return new ConstantNode(true, true, NULL, NULL);
    }
    else if (type.compare("pi*") == 0) {
		GPNode* signal = createNode(tokenizer, subtreeArgs);
        return new ConstantNode(false, true, NULL, signal);
    }
    else if (type.compare("const") == 0) {
		GPMutatableParam* constantvalue = createMutatableParam(tokenizer, true, "constantvalue");
        return new ConstantNode(true, false, constantvalue, NULL);
    }
    else if (type.compare("const*") == 0) {
		GPMutatableParam* constantvalue = createMutatableParam(tokenizer, true, "constantvalue");
		GPNode* signal = createNode(tokenizer, subtreeArgs);
        return new ConstantNode(false, false, constantvalue, signal);
    }
    // function nodes
    else if (type.compare("+") == 0) {
		GPNode* signalzero = createNode(tokenizer, subtreeArgs);
		GPNode* signalone = createNode(tokenizer, subtreeArgs);
        return new AddNode(signalzero, signalone);
    }
    else if (type.compare("*") == 0) {
		GPNode* signalzero = createNode(tokenizer, subtreeArgs);
		GPNode* signalone = createNode(tokenizer, subtreeArgs);
        return new MultiplyNode(signalzero, signalone);
    }
    else if (type.compare("sin") == 0) {
		GPNode* signalzero = createNode(tokenizer, subtreeArgs);
        return new SineNode(signalzero);
    }
    // LFO nodes
    else if (type.compare("lfo") == 0) {
		GPMutatableParam* lforate = createMutatableParam(tokenizer, true, "lforate");
        return new LFONode(true, lforate, NULL);
    }
    else if (type.compare("lfo*") == 0) {
		GPMutatableParam* lforate = createMutatableParam(tokenizer, true, "lforate");
		GPNode* signal = createNode(tokenizer, subtreeArgs);
        return new LFONode(false, lforate, signal);
    }
    // mixer nodes
    else if (type.compare("if") == 0) {
		GPNode* control = createNode(tokenizer, subtreeArgs);
		GPNode* signalzero = createNode(tokenizer, subtreeArgs);
		GPNode* signalone = createNode(tokenizer, subtreeArgs);
        return new MixerNode(false, control, signalzero, signalone);
    }
    else if (type.compare("mix") == 0) {
		GPNode* control = createNode(tokenizer, subtreeArgs);
		GPNode* signalzero = createNode(tokenizer, subtreeArgs);
		GPNode* signalone = createNode(tokenizer, subtreeArgs);
        return new MixerNode(true, control, signalzero, signalone);
    }
    // noise node
    else if (type.compare("whitenoise") == 0) {
        return new NoiseNode(rng);
    }
    // oscil node
    else if (type.compare("osc") == 0) {
		GPMutatableParam* oscilvarnum = createMutatableParam(tokenizer, false, "oscilvarnum");
		GPMutatableParam* oscilpartial = createMutatableParam(tokenizer, true, "oscilpartial");
		return new OscilNode(true, oscilvarnum, oscilpartial, NULL, NULL);
    }
    else if (type.compare("fm") == 0) {
		GPMutatableParam* oscilvarnum = createMutatableParam(tokenizer, false, "oscilvarnum");
		GPMutatableParam* oscilpartial = createMutatableParam(tokenizer, true, "oscilpartial");
		GPMutatableParam* oscilindex = createMutatableParam(tokenizer, true, "oscilindex");
		GPNode* modsignal = createNode(tokenizer, subtreeArgs);
        return new OscilNode(false, oscilvarnum, oscilpartial, oscilindex, modsignal);
    }
    // silence node
    else if (type.compare("silence") == 0) {
        return new SilenceNode();
    }
    // spline node (already created)
    else if (type.compare("spline") == 0) {
        GPMutatableParam* splinetype = createMutatableParam(tokenizer, false, "splinetype");
        GPMutatableParam* splinenum = createMutatableParam(tokenizer, false, "splinenum");

        int numSplinePoints = splinenum->getDValue();
        std::vector<GPMutatableParam*> splinepoints;
        GPMutatableParam* splineinitialvalue = createMutatableParam(tokenizer, true, "splinepoint");
        splinepoints.push_back(splineinitialvalue);

        for (int i = 0; i < numSplinePoints; i++) {
            GPMutatableParam* splinelength = createMutatableParam(tokenizer, true, "splinelength");
            GPMutatableParam* splinepoint = createMutatableParam(tokenizer, true, "splinepoint");
            splinepoints.push_back(splinelength);
            splinepoints.push_back(splinepoint);
        }
        
        return new SplineNode(true, rng, false, splinetype, splinenum, splinepoints, NULL);
    }
    else if (type.compare("spline*") == 0) {
        GPMutatableParam* splinetype = createMutatableParam(tokenizer, false, "splinetype");
        GPMutatableParam* splinenum = createMutatableParam(tokenizer, true, "splinenum");

        int numSplinePoints = splinenum->getDValue();
        std::vector<GPMutatableParam*> splinepoints;
        GPMutatableParam* splineinitialvalue = createMutatableParam(tokenizer, true, "splinepoint");
        splinepoints.push_back(splineinitialvalue);

        for (int i = 0; i < numSplinePoints; i++) {
            GPMutatableParam* splinelength = createMutatableParam(tokenizer, true, "splinelength");
            GPMutatableParam* splinepoint = createMutatableParam(tokenizer, true, "splinepoint");
            splinepoints.push_back(splinelength);
            splinepoints.push_back(splinepoint);
        }
        
        return new SplineNode(false, rng, false, splinetype, splinenum, splinepoints, createNode(tokenizer, subtreeArgs));
    }
    // time node
    else if (type.compare("time") == 0) {
        return new TimeNode();
    }
    // variable node
    else if (type.compare("var") == 0) {
		GPMutatableParam* variablevarnum = createMutatableParam(tokenizer, false, "variablevarnum");
		GPMutatableParam* variablerange = createMutatableParam(tokenizer, true, "variablerange");
        return new VariableNode(variablevarnum, variablerange);
    }
	// null node
	else if (type.compare("null") == 0) {
		return NULL;
	}
	// otherwise s-expression formatting is wrong
    else {
        std::cerr << "Tried to build S-expression from improperly formatted string" << std::endl;
        return NULL;
    }
}

/*
    ===========
    EASY ACCESS
    ===========
*/

GPNode* createNode(std::string nodestring, GPRandom* rng) {
	std::vector<std::string> tokens = split(nodestring, " }{)(");
    unsigned index = 0;
	return createNode(tokens, &index, rng);
}

GPMutatableParam* createMutatableParam(std::string paramstring, std::string type, bool ismutatable) {
	std::vector<std::string> tokens = split(paramstring, " }{)(");
    unsigned index = 0;
	return createMutatableParam(tokens, &index, ismutatable, type);
}
