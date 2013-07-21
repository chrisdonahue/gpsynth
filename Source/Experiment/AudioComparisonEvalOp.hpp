/*
 *  Function maximization with real-valued GA (AudioComparison):
 */

/*!
 *  \file   AudioComparisonEvalOp.hpp
 *  \brief  Audio Spectrum Comparison
 *  \author Chris Donahue
 *  \author Marc Parizeau
 */

/*!
 *  \defgroup MaxFctFloat Function Maximization with Real-Valued GA Example
 *  \brief Function maximization with real-valued GA (MaxFctFloat):
 *    A simple real-valued GA example with Open BEAGLE.
 *
 *  \par Objective
 *  Find the maximum of the following 5D function:
 *  \f$f(x) = \frac{161.8}{(u_N^2 \sum_{k=0}^{N-1}(x_k^2 + u_k^2))}\f$
 *  with \f$x = <x_0, x_1, ..., x_{N-1}>\f$, \f$u_{k+1} = x_k + u_k\f$, \f$x_k\f$
 *  in \f$[-200,200]\f$ for all \f$k\f$, \f$N = 5\f$ and \f$u_0 = 10\f$.
 *
 *  \par Representation
 *  Vector of five floating-point numbers representing the function's five arguments
 *  \f$x_i\f$, limited in the interval \f$[-200,200]\f$.
 *
 *  \par Fitness
 *  Value of \f$f(x_1,x_2,x_3,x_4,x_5)\f$.
 *
 */

#ifndef AudioComparisonEvalOp_hpp
#define AudioComparisonEvalOp_hpp

#include "beagle/GA.hpp"
#include <vector>
#include "../Source/Synth/GPNetwork.h"
#include "GPExperiment.h"
#include <cmath>
#include "AudioComparisonParams.hpp"

class GPExperiment;

/*!
 *  \class AudioComparisonEvalOp AudioComparisonEvalOp.hpp "AudioComparisonEvalOp.hpp"
 *  \brief The individual evaluation class operator for the problem of real-valued GA
 *    function maximisation.
 *  \ingroup MaxFctFloat
 */
class AudioComparisonEvalOp : public Beagle::EvaluationOp
{

public:

	//! AudioComparisonEvalOp allocator type.
	typedef Beagle::AllocatorT<AudioComparisonEvalOp,Beagle::EvaluationOp::Alloc>
	Alloc;
	//!< AudioComparisonEvalOp handle type.
	typedef Beagle::PointerT<AudioComparisonEvalOp,Beagle::EvaluationOp::Handle>
	Handle;
	//!< AudioComparisonEvalOp bag type.
	typedef Beagle::ContainerT<AudioComparisonEvalOp,Beagle::EvaluationOp::Bag>
	Bag;

	explicit AudioComparisonEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
	        Beagle::Context& ioContext);
    virtual void registerParams(Beagle::System& ioSystem);

    GPExperiment* experiment;
    unsigned type;
    GPNetwork* candidate;
    float* candidateFramesBuffer;

protected:

};

#endif // AudioComparisonEvalOp_hpp
