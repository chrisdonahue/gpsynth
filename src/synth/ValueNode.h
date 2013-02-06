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
