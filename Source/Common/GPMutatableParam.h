#ifndef GPMUTATABLEPARAM_H
#define GPMUTATABLEPARAM_H

#include <assert.h>
#include <string>

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

        // copy method
        GPMutatableParam* getCopy() {
            if (isContinuous)
                return new GPMutatableParam(type, isMutatable, cvalue, cminimum, cmaximum);
            else
                return new GPMutatableParam(type, isMutatable, dvalue, dminimum, dmaximum);
        }

        // set discrete values
        void setDValue(double val) {
            assert(!isContinuous && dminimum <= val && val <= dmaximum);
            dvalue = val;
        }

        void setDRange(double min, double max) {
            assert(!isContinuous && min <= dvalue && dvalue <= max);
            dminimum = min;
            dmaximum = max;
        }

        // set continuous values
        void setCValue(double val) {
            assert(isContinuous && cminimum <= val && val <= cmaximum);
            cvalue = val;
        }

        void setCRange(double min, double max) {
            assert(isContinuous && min <= cvalue && cvalue <= max);
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

        // combined accessor
        double getValue() {
            if (isContinuous)
                return cvalue;
            else
                return dvalue;
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
