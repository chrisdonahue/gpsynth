#include "ValueNode.h"

ValueNode::ValueNode(float *val) {
    value = val;
}

ValueNode::~ValueNode() {
    delete value;
}

ValueNode& ValueNode::getCopy() {
    return new ValueNode(value);
}

float evaluate() {
    return *value;
}

std::string toString() {
    if (value == time) {
        return "time";
    }
    else if (value == cps) {
        return "f"
    }
    else:
        return sprintf("%f", value);
}
