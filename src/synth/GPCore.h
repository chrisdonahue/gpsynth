#include <string>
#include <vector>
#include <set>
#include "GPNetwork.h"
#include "GPNode.h"
#include "ValueNode.h"
#include "FunctionNode.h"

class GPParams {
	public:
		// parameters
		int populationSize;
		int valueRange;
		int LFORange;
		int numPartials;
		
		// chance params
		float mutationChance;
		float simplifyChance;
		float specialChance;
		float harmonyChance;
		float functionChance;
		float crossChance;
	
		// IDs
		int nextNetworkID;
		int nextGenerationID;
		
		// network containers
		std::vector<GPNetwork&> networks;
		std::vector<GPNetwork&> currentlyEvaluating;
		std::set<GPNetwork&> evaluated;
		std::set<GPNetwork&> selected;
		
		// special node values
		float *time;
		float *cps;
};

//available network functions
float add(GPNode& left, GPNode& right);
float subtract(GPNode& left, GPNode& right);
float multiply(GPNode& left, GPNode& right);
float modulo(GPNode& left, GPNode& right);
float divide(GPNode& left, GPNode& right);
float pow(GPNode& left, GPNode& right);
float sine(GPNode& left, GPNode& right);
float cosine(GPNode& left, GPNode& right);
