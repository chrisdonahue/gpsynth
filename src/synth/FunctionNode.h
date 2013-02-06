#include "GPNode.h"

class FunctionNode: public GPNode {
	public:
		FunctionNode(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& l, GPNode& r);
		~FunctionNode();
		FunctionNode& getCopy();
		
		void setFunction(void (*fun)(GPNode&, GPNode&), std::string sym, GPNode& rSub);
		float evaluate();
		std::string toString();
        void traceLineage();

	private:
		void (*function)(GPNode&, GPNode&);
		std::string symbol;
};
