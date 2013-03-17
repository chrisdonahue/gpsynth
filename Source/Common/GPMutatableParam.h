#ifndef GPMUTATABLEPARAM_H
#define GPMUTATABLEPARAM_H

#include <assert.h>

class GPMutatableParam {
    public:
        virtual GPMutatableParam* getCopy() = 0;
};

class GPMutatableParam {
    public:
        // discrete constructor
        GPMutatableParam(char* t, bool mutatable, int val, int min, int max) {
            assert(min <= val && val <= max);
            type = t;
            isContinuous = false;
            isMutatable = mutatable;
            dvalue = val;
            dminimum = min;
            dmaximum = max;
        }

        // continuous constructor
        GPMutatableParam(char* t, bool mutatable, double val, double min, double max) {
            assert(min <= val && val <= max);
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
        char* getType() {
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
        char* type;
        int dvalue;
        int dminimum;
        int dmaximum;
        double cvalue;
        double cminimum;
        double cmaximum;
};

class GPDiscreteParam : GPMutatableParam {
    public:
        GPDiscreteParam(int* val, int min, int max) {
            assert(min <= val && val <= max);
            continuous = false;
            value = val;
            minimum = min;
            maximum = max;
        }

        getCopy() {
            
        }

        void setValue(int val) {
            assert(minimum <= val && val <= maximum);
            *value = val;
        }

        void setRange(int min, int max) {
            assert(min <= value && value <= max);
            minimum = min;
            maximum = max;
        }

        void setAll(int val, int min, int max) {
            assert(min <= val && val <= max);
            *value = val;
            minimum = min;
            maximum = max;
        }

        int getValue() {
            return *value;
        }

        int getMin() {
            return minimum;
        }

        int getMax() {
            return maximum;
        }

    private:
        char* type;
        int* value;
        int minimum;
        int maximum;
};

#endif
