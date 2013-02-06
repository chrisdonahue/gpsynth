#include "GPCore.h"

class GPNetwork {
	public:
		GPNetwork(const GPParams& p, GPNode& h);
		~GPNetwork();
		GPNetwork& getCopy();
        const GPNode& getRoot();
		
		float evaluate();
		std::string toString();
		void mutate();
		GPNetwork& reproduce(const GPNetwork& partner);

	private:
		int networkID;
		std::string asText;
		GPNode& root;
        const GPParams& params;

		void swap(GPNode& one, GPNode& two);
};
