/*!
 *  \file   AudioComparisonParams.hpp
 *  \brief  Audio Spectrum Comparison
 *  \author Chris Donahue
 *  \author Marc Parizeau
 */

#ifndef AudioComparisonParams_hpp
#define AudioComparisonParams_hpp

#include "beagle/GA.hpp"
#include "GPExperiment.h"
#include "../Source/Synth/GPNetwork.h"

class GPExperiment;

/*!
 *  \class AudioComparisonParams AudioComparisonParams.hpp "AudioComparisonParams.hpp"
 *  \brief The individual evaluation class operator for the problem of real-valued GA
 *    function maximisation.
 *  \ingroup MaxFctFloat
 */
class AudioComparisonParams : public Beagle::Object
{

public:

	//! AudioComparisonParams allocator type.
	typedef Beagle::AllocatorT<AudioComparisonParams,Beagle::Object::Alloc>
	Alloc;
	//!< AudioComparisonParams handle type.
	typedef Beagle::PointerT<AudioComparisonParams,Beagle::Object::Handle>
	Handle;
	//!< AudioComparisonParams bag type.
	typedef Beagle::ContainerT<AudioComparisonParams,Beagle::Object::Bag>
	Bag;

	explicit AudioComparisonParams();

    GPExperiment* experiment;
    unsigned type;
    GPNetwork* candidate;
    float* candidateFramesBuffer;

protected:

};

#endif // AudioComparisonParams_hpp
