/*
  ==============================================================================

    FunctionNode.cpp
    Author:  cdonahue

  ==============================================================================
*/

#include "FunctionNode.h"

/*
    =========
    OVERRIDES
    =========
*/

void FunctionNode::toString(std::stringstream& ss) {
    ss << "(" << symbol;
    for (unsigned i = 0; i < arity; i++) {
        ss << " ";
        descendants[i]->toString(ss);
    }
    ss << ")";
}
