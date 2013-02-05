#include "GPNode.h"

class ValueNode: public GPNode {
	public:
		ValueNode(float *value);
		~ValueNode();
		ValueNode& getCopy();
		
		void setFunction(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& rSub);
		float evaluate();
		std::string toString();
		
	private:
		float value;
		GPNode& parent;
};
