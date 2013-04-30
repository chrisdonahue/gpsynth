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
    GPMutatableParam(std::string t, bool mutatable, double val, double min, double max) {
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

    // render as string
    void toString(bool printRange, std::stringstream& ss) {
        if (printRange) {
          ss << "{";
          if (isContinuous) {
            ss << "C: " << cminimum << " " << cvalue << " " << cmaximum;
          }
          else {
            ss << "D: " << dminimum << " " << dvalue << " " << dmaximum;
          }
          ss << "}";
        }
        else {
          if (isContinuous)
            ss << cvalue;
          else
            ss << dvalue;
        }
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
    void setCValue(double val) {
        if (val > cmaximum)
            cvalue = cmaximum;
        else if (val < cminimum)
            cvalue = cminimum;
        else
            cvalue = val;
    }

    void setCRange(double min, double max) {
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
    double getCValue() {
        assert(isContinuous);
        return cvalue;
    }

    double getCMin() {
        assert(isContinuous);
        return cminimum;
    }

    double getCMax() {
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
    double getValue() {
        if (isContinuous)
            return cvalue;
        else
            return dvalue;
    }

    double getMin() {
	if (isContinuous)
	    return cminimum;
	else
	    return dminimum;
    }

    double getMax() {
	if (isContinuous)
	    return cmaximum;
	else
	    return dmaximum;
    }

    void ephemeralRandom(GPRandom* rng) {
        if (isContinuous && isMutatable)
            cvalue = (rng->random() * (cmaximum - cminimum)) + cminimum;
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
    double cvalue;
    double cminimum;
    double cmaximum;
};

#endif
