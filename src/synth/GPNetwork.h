#include "GPCore.h"

class GPNetwork {
	public:
		GPNetwork(const GPNode& h);
		GPNetwork(const GPNetwork&);
		~GPNetwork();
		GPNetwork& getCopy();
		
		float evaluate();
		std::string toString();
		void mutate();
		GPNetwork& reproduce(const GPNetwork& partner);

	private:
		int networkID;
		std::string functionString;
		GPNode head;

		void swap(GPNode& one, GPNode& two);
};
