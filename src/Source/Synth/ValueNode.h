/*
  ==============================================================================

    ValueNode.h
    Created: 6 Feb 2013 11:06:13am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __VALUENODE_H_AB743438__
#define __VALUENODE_H_AB743438__

#include "GPNode.h"
#include "GPCore.h"

class ValueNode: public GPNode {
public:
    ValueNode(float* val);
    ~ValueNode();
    ValueNode& getCopy();

    float evaluate();
    std::string toString();

private:
    float* value;
};

#endif  // __VALUENODE_H_AB743438__
