#ifndef GPMUTATABLEPARAM_H
#define GPMUTATABLEPARAM_H

#include <assert.h>
#include <string>
#include <sstream>
#include "GPRandom.h"

class GPMutatableParam {
public:
    // discrete constructor
    GPMutatableParam(std::string t, bool mutatable, int val, int min, int max) {
        assert(!mutatable || (min <= val && val <= max));
        type = t;
        isContinuous = false;
        isMutatable = mutatable;
        dvalue = val;
        dminimum = min;
        dmaximum = max;
    }

    // continuous constructor
    GPMutatableParam(std::string t, bool mutatable, float val, float min, float max) {
        assert(!mutatable || (min <= val && val <= max));
        type = t;
        isContinuous = true;
        isMutatable = mutatable;
        cvalue = val;
        cminimum = min;
        cmaximum = max;
    }

    ~GPMutatableParam() {
    }

    // copy method
    GPMutatableParam* getCopy() {
        if (isContinuous)
            return new GPMutatableParam(type, isMutatable, cvalue, cminimum, cmaximum);
        else
            return new GPMutatableParam(type, isMutatable, dvalue, dminimum, dmaximum);
    }

    // render to a string stream
    void toString(std::stringstream& ss) {
        ss << "{";
        if (isContinuous) {
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
        isMutatable = true;
    }

    // mark as unmutatable
    void setUnmutatable() {
        isMutatable = false;
    }

    // set discrete values
    void setDValue(int val) {
        if (val > dmaximum)
            dvalue = dmaximum;
        else if (val < dminimum)
            dvalue = dminimum;
        else
            dvalue = val;
    }

    void setDRange(int min, int max) {
        assert(!isContinuous && min <= max);
        // TODO: alter value to new range
        dminimum = min;
        dmaximum = max;
    }

    // set continuous values
    void setCValue(float val) {
        if (val > cmaximum)
            cvalue = cmaximum;
        else if (val < cminimum)
            cvalue = cminimum;
        else
            cvalue = val;
    }

    void setCRange(float min, float max) {
        assert(isContinuous && min <= max);
        // TODO: alter value to new range
        cminimum = min;
        cmaximum = max;
    }

    // type accessor
    std::string getType() {
        return type;
    }

    // continuous accessors
    float getCValue() {
        assert(isContinuous);
        return cvalue;
    }

    float getCMin() {
        assert(isContinuous);
        return cminimum;
    }

    float getCMax() {
        assert(isContinuous);
        return cmaximum;
    }

    // discrete accessors
    int getDValue() {
        assert(!isContinuous);
        return dvalue;
    }

    int getDMin() {
        assert(!isContinuous);
        return dminimum;
    }

    int getDMax() {
        assert(!isContinuous);
        return dmaximum;
    }

    // combined accessors
    float getValue() {
        if (isContinuous)
            return cvalue;
        else
            return (float) dvalue;
    }

    float getMin() {
		if (isContinuous)
			return cminimum;
		else
			return (float) dminimum;
    }

    float getMax() {
		if (isContinuous)
			return cmaximum;
		else
			return (float) dmaximum;
    }

    void ephemeralRandom(GPRandom* rng) {
        if (isContinuous && isMutatable)
            cvalue = ((float) rng->random() * (cmaximum - cminimum)) + cminimum;
        else if (!isContinuous && isMutatable)
            dvalue = (rng->random((dmaximum - dminimum) + 1)) + dminimum;
    }

    bool isContinuous;
    bool isMutatable;

private:
    std::string type;
    int dvalue;
    int dminimum;
    int dmaximum;
    float cvalue;
    float cminimum;
    float cmaximum;
};

#endif
