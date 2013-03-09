#ifndef GPMUTATABLEPARAM_H
#define GPMUTATABLEPARAM_H

#include <assert.h>

class GPMutatableParam {
    public:
        bool isContinuous() {
            return continuous;
        };
        bool continuous;
};

class GPDiscreteParam : GPMutatableParam {
    public:
        GPDiscreteParam(int val, int min, int max) {
            assert(min <= val && val <= max);
            continuous = false;
            value = val;
            minimum = min;
            maximum = max;
        }

        void setValue(int val) {
            assert(minimum <= val && val <= maximum);
            value = val;
        }

        void setRange(int min, int max) {
            assert(min <= value && value <= max);
            minimum = min;
            maximum = max;
        }

        void setAll(int val, int min, int max) {
            assert(min <= val && val <= max);
            value = val;
            minimum = min;
            maximum = max;
        }

        int getValue() {
            return value;
        }

        int getMin() {
            return minimum;
        }

        int getMax() {
            return maximum;
        }

    private:
        int value;
        int minimum;
        int maximum;
};

class GPContinuousParam : GPMutatableParam {
    public:
        GPContinuousParam(double val, double min, double max) {
            assert(min <= val && val <= max);
            continuous = true;
            value = val;
            minimum = min;
            maximum = max;
        }

        void setValue(double val) {
            assert(minimum <= val && val <= maximum);
            value = val;
        }

        void setRange(double min, double max) {
            assert(min <= value && value <= max);
            minimum = min;
            maximum = max;
        }

        void setAll(double val, int min, double max) {
            assert(min <= val && val <= max);
            value = val;
            minimum = min;
            maximum = max;
        }

        double getValue() {
            return value;
        }

        double getMin() {
            return minimum;
        }

        double getMax() {
            return maximum;
        }

    private:
        double value;
        double minimum;
        double maximum;
};

#endif
