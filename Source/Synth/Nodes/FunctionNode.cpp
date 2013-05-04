/*
  ==============================================================================

    FunctionNode.cpp
    Created: 6 Feb 2013 11:05:21am
    Author:  cdonahue

  ==============================================================================
*/

#include "FunctionNode.h"

/*
    =========
    OVERRIDES
    =========
*/

void FunctionNode::toString(bool printRange, std::stringstream& ss) {
    ss << "(" << symbol;
    for (unsigned i = 0; i < arity; i++) {
        ss << " ";
        descendants[i]->toString(printRange, ss);
    }
    ss << ")";
}
