#include "GPCore.h"

class GPNode {
	public:
		virtual float evaluate() = 0;
		virtual std::string toString() = 0;
		virtual GPNode& getCopy() = 0;
    private:
        GPNode& parent;
        GPNode& left;
        GPNode& right;
};
