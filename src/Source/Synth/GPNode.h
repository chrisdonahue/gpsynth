/*
  ==============================================================================

    GPNode.h
    Created: 6 Feb 2013 11:06:00am
    Author:  cdonahue

  ==============================================================================
*/

#ifndef __GPNODE_H_FAD1259A__
#define __GPNODE_H_FAD1259A__

#include <string>

class GPNode {
    public:
        virtual float evaluate() = 0;
        virtual std::string toString() = 0;
        virtual GPNode* getCopy() = 0;
    protected:
        GPNode* parent;
        GPNode* left;
        GPNode* right;
};

#endif  // __GPNODE_H_FAD1259A__
