/*
 *  Function maximization with real-valued GA (AudioComparison):
 */

/*!
 *  \file   AudioComparisonEvalOp.cpp
 *  \brief  Audio Spectrum Comparison
 *  \author Chris Donahue
 */

#include "beagle/GA.hpp"
#include "AudioComparisonEvalOp.hpp"

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for maximizing the function.
 */
AudioComparisonEvalOp::AudioComparisonEvalOp() :
		EvaluationOp("AudioComparisonEvalOp")
{ }

/*!
 *  \brief Register the AudioComparison parameters.
 *  \param ioSystem Evolutionary system.
 *	from AudioComparisonEvalOp.cpp
 */
void AudioComparisonEvalOp::registerParams(System& ioSystem)
{
	/*
	Beagle::EvaluationOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "Values of the available objects that can be put into the AudioComparison. ";
		lOSS << "If the object values is not specified, it will be randomly generated ";
		lOSS << "at the initialization time.";
		Register::Description lDescription(
		    "AudioComparison object values",
		    "Vector",
		    "",
		    lOSS.str()
		);
		mObjectValues = castHandleT<Vector>(
		                    ioSystem.getRegister().insertEntry("ks.object.values", new Vector(0), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "Weights of the available objects that can be put into the AudioComparison. ";
		lOSS << "If the object weights is not specified, it will be randomly generated ";
		lOSS << "at the initialization time.";
		Register::Description lDescription(
		    "AudioComparison object weights",
		    "Vector",
		    "",
		    lOSS.str()
		);
		mObjectWeights = castHandleT<Vector>(
		                     ioSystem.getRegister().insertEntry("ks.object.weights", new Vector(0), lDescription));
	}
	{
		Register::Description lDescription(
		    "Initial integer vectors sizes",
		    "UInt",
		    "0",
		    "Integer vector size of initialized individuals."
		);
		mIntVectorSize = castHandleT<UInt>(
		                     ioSystem.getRegister().insertEntry("ga.init.vectorsize", new UInt(0), lDescription));
	}
	*/
}

/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle AudioComparisonEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
    // get params
    std::vector<GPMutatableParam*>* params = candidate->getAllMutatableParams();

    // assert BEAGLE things
	Beagle_AssertM(inIndividual.size() == 1);
	GA::FloatVector::Handle lFloatVector = castHandleT<GA::FloatVector>(inIndividual[0]);
	Beagle_AssertM(lFloatVector->size() == params->size());

    // old code for reference
    /*
	double lU   = 10.0;
	double lSum = 0.0;
	for(unsigned int i=0; i<5; ++i) {
		double lXi = (*lFloatVector)[i];
		if(lXi > 200.0)  lXi = 200.0;
		if(lXi < -200.0) lXi = -200.0;
		lSum += (lXi*lXi) + (lU*lU);
		lU += lXi;
	}
	lSum += (lU*lU);
	double lF = 161.8 / lSum;
    */

    // assign new values from individual
    for (unsigned i = 0; i < params->size(); i++) {
        GPMutatableParam* param = params->at(i);
        if (param->isDiscrete()) {
            int newDValue = (int) (*lFloatVector)[i];
            param->setDValue(newDValue);
        }
        else {
            float newCValue = (float) (*lFloatVector)[i];
            param->setCValue(newCValue);
        }
    }

    // update candidate params
    candidate->updateMutatedParams();

    // evaluate the new organism using the call back method
    double fitness = (*callback) (type, candidate, candidateFramesBuffer);

    // report fitness to the system
	return new FitnessSimple(fitness);
}
