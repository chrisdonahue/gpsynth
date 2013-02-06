#include "FunctionNode.h"

/*
    PUBLIC METHODS
*/
FunctionNode::FunctionNode(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& l, GPNode& r) {
    function = fun;
    symbol = sym;
    left = l;
    right = r;
    parent = NULL;
}

FunctionNode::~FunctionNode() {
    delete left;
    delete right;
    delete symbol;
}

FunctionNode& FunctionNode::getCopy() {
    return new FunctionNode(fun, symbol, left, right);
}

void FunctionNode::setFunction(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& rSub) {
    if (right == NULL) {
        right = rSub;
    }
    function = fun;
    symbol = sym;
}

float FunctionNode::evaluate() {
    return function(left, right);
}

std::string FunctionNode::toString() {
    if (right != NULL) {
        return sprintf("(%s %s %s)", symbol, left.toString(), right.toString());
    }
    else {
        return sprintf("(%s %s)", symbol, left.toString());
    }
}

void traceLineage() {
    // IMPLEMENT LATER
}
