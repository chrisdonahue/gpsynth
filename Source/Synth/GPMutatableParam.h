#ifndef GPMUTATABLEPARAM_H
#define GPMUTATABLEPARAM_H

#include <assert.h>
#include <string>
#include <sstream>
#include "../Common/GPRandom.h"

class GPMutatableParam {
public:
    // discrete constructor
    GPMutatableParam(std::string t, bool mut, int val, int min, int max) {
        assert(!mut || (min <= val && val <= max));
        type = t;
        continuous = false;
        mutatable = mut;
        dvalue = val;
        dminimum = min;
        dmaximum = max;
    }

    // continuous constructor
    GPMutatableParam(std::string t, bool mut, float val, float min, float max) {
        assert(!mut || (min <= val && val <= max));
        type = t;
        continuous = true;
        mutatable = mut;
        cvalue = val;
        cminimum = min;
        cmaximum = max;
    }

    ~GPMutatableParam() {
    }

    // copy method
    GPMutatableParam* getCopy() {
        if (continuous)
            return new GPMutatableParam(type, mutatable, cvalue, cminimum, cmaximum);
        else
            return new GPMutatableParam(type, mutatable, dvalue, dminimum, dmaximum);
    }

    // render to a string stream
    void toString(std::stringstream& ss) {
        ss << "{";
        if (continuous) {
            ss << "c " << cminimum << " " << cvalue << " " << cmaximum;
        }
        else {
            ss << "d " << dminimum << " " << dvalue << " " << dmaximum;
        }
        ss << "}";
    }

    // get mutatble param as string
    std::string toString(unsigned precision) {
        std::stringstream ss;
        ss.precision(precision);
        toString(ss);
        return ss.str();
    }

    // set type
    void setType(std::string newtype) {
        type = newtype;
    }

    // mark as mutatable
    void setMutatable() {
        mutatable = true;
    }

    // mark as unmutatable
    void setUnmutatable() {
        mutatable = false;
    }

    // set discrete data
    void setDData(int min, int val, int max) {
        assert(!continuous);
        assert(!mutatable || (min <= val && val <= max));
        dminimum = min;
        dvalue = val;
        dmaximum = max;
    }

    // set continuous values
    void setDValue(int newvalue) {
        setDData(dminimum, newvalue, dmaximum);
    }

    // set continuous data
    void setCData(float min, float val, float max) {
        assert(continuous);
        assert(!mutatable || (min <= val && val <= max));
        cminimum = min;
        cvalue = val;
        cmaximum = max;
    }

    // set continuous values
    void setCValue(float newvalue) {
        setCData(cminimum, newvalue, cmaximum);
    }

    // type accessor
    std::string getType() {
        return type;
    }

    // continuous accessors
    float getCValue() {
        assert(continuous);
        return cvalue;
    }

    float getCMin() {
        assert(continuous);
        return cminimum;
    }

    float getCMax() {
        assert(continuous);
        return cmaximum;
    }

    // discrete accessors
    int getDValue() {
        assert(!continuous);
        return dvalue;
    }

    int getDMin() {
        assert(!continuous);
        return dminimum;
    }

    int getDMax() {
        assert(!continuous);
        return dmaximum;
    }

    // combined accessors
    float getValue() {
        if (continuous)
            return cvalue;
        else
            return (float) dvalue;
    }

    float getMin() {
		if (continuous)
			return cminimum;
		else
			return (float) dminimum;
    }

    float getMax() {
		if (continuous)
			return cmaximum;
		else
			return (float) dmaximum;
    }

    bool isDiscrete() {
        return !continuous;
    }

    bool isContinuous() {
        return continuous;
    }

    bool isMutatable() {
        return mutatable;
    }

    bool isUnmutatable() {
        return !mutatable;
    }

    void ephemeralRandom(GPRandom* rng) {
        if (continuous && mutatable)
            cvalue = ((float) rng->random() * (cmaximum - cminimum)) + cminimum;
        else if (!continuous && mutatable)
            dvalue = (rng->random((dmaximum - dminimum) + 1)) + dminimum;
    }


private:
    std::string type;
    int dvalue;
    int dminimum;
    int dmaximum;
    float cvalue;
    float cminimum;
    float cmaximum;
    bool continuous;
    bool mutatable;
};

#endif
