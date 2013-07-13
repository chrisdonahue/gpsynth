/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/GA/src/CrossoverBlendESVecOp.cpp
 *  \brief  Source code of class GA::CrossoverBlendFltVec.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>
#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a ES blend crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
Beagle::GA::CrossoverBlendESVecOp::CrossoverBlendESVecOp(std::string inMatingPbName,
        std::string inName) :
		CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the ES blend crossover operator.
 *  \param ioSystem System of the evolution.
 */
void Beagle::GA::CrossoverBlendESVecOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
		    "Individual blend crossover prob.",
		    "Double",
		    "0.3",
		    "ES blend crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lDescription));
	}
	CrossoverOp::registerParams(ioSystem);
	{
		std::ostringstream lOSS;
		lOSS << "ES vector maximum values. This can be a scalar, which limit the values for all ES, ";
		lOSS << "or a vector which limit the values individually. If the maximum value vector size is ";
		lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
		lOSS << "is equal to the last value of the maximum value vector.";
		Register::Description lDescription(
		    "Maximum ES vector values",
		    "DoubleArray",
		    dbl2str(DBL_MAX),
		    lOSS.str()
		);
		mMaxValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("es.value.max", new DoubleArray(1,DBL_MAX), lDescription));
	}
	{
		std::ostringstream lOSS;
		lOSS << "ES vector minimum values. This can be a scalar, which limit the values for all ES, ";
		lOSS << "or a vector which limit the values individually. If the minimum value vector size is ";
		lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
		lOSS << "is equal to the last value of the minimum value vector.";
		Register::Description lDescription(
		    "Minimum ES vector values",
		    "DoubleArray",
		    dbl2str(-DBL_MAX),
		    lOSS.str()
		);
		mMinValue = castHandleT<DoubleArray>(
		                ioSystem.getRegister().insertEntry("es.value.min", new DoubleArray(1,-DBL_MAX), lDescription));
	}
	{
		Register::Description lDescription(
		    "Blend crossover alpha value",
		    "Double",
		    "0.5",
		    "ES blend crossover alpha parameter moduling amplitude of operation."
		);
		mAlpha = castHandleT<Double>(
		             ioSystem.getRegister().insertEntry("es.cxblend.alpha", new Double(0.5), lDescription));
	}
	Beagle_StackTraceEndM("void GA::CrossoverBlendESVecOp::registerParams(System& ioSystem)");
}


/*!
 *  \brief Mate two ES individuals for blend crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
bool Beagle::GA::CrossoverBlendESVecOp::mate(Beagle::Individual& ioIndiv1,
        Beagle::Context&    ioContext1,
        Beagle::Individual& ioIndiv2,
        Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;

	Beagle_LogDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover", "Beagle::GA::CrossoverBlendESVecOp",
	    "Individuals mated (before ES blend crossover)"
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover", "Beagle::GA::CrossoverBlendESVecOp",
	    ioIndiv1
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover", "Beagle::GA::CrossoverBlendESVecOp",
	    ioIndiv2
	);

	for(unsigned int i=0; i<lNbGenotypes; ++i) {
		GA::ESVector::Handle lESVector1 = castHandleT<ESVector>(ioIndiv1[i]);
		GA::ESVector::Handle lESVector2 = castHandleT<ESVector>(ioIndiv2[i]);
		unsigned int lSize = minOf<unsigned int>(lESVector1->size(), lESVector2->size());
		for(unsigned int j=0; j<lSize; ++j) {
			const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
			const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
			const double lU_xi = ioContext1.getSystem().getRandomizer().rollUniform();
			const double lGamma_xi = ((1.0+2.0*mAlpha->getWrappedValue())*lU_xi)-mAlpha->getWrappedValue();
			const double lU_si = ioContext1.getSystem().getRandomizer().rollUniform();
			const double lGamma_si = ((1.0+2.0*mAlpha->getWrappedValue())*lU_si)-mAlpha->getWrappedValue();
			const double lX1_i = (*lESVector1)[j].mValue;
			const double lX2_i = (*lESVector2)[j].mValue;
			const double lS1_i = (*lESVector1)[j].mStrategy;
			const double lS2_i = (*lESVector2)[j].mStrategy;
			(*lESVector1)[j].mValue = ((1.0-lGamma_xi)*lX1_i) + (lGamma_xi*lX2_i);
			(*lESVector2)[j].mValue = (lGamma_xi*lX1_i) + ((1.0-lGamma_xi)*lX2_i);
			if((*lESVector1)[j].mValue > lMaxVal) (*lESVector1)[j].mValue = lMaxVal;
			if((*lESVector1)[j].mValue < lMinVal) (*lESVector1)[j].mValue = lMinVal;
			if((*lESVector2)[j].mValue > lMaxVal) (*lESVector2)[j].mValue = lMaxVal;
			if((*lESVector2)[j].mValue < lMinVal) (*lESVector2)[j].mValue = lMinVal;
			(*lESVector1)[j].mStrategy = ((1.0-lGamma_si)*lS1_i) + (lGamma_si*lS2_i);
			(*lESVector2)[j].mStrategy = (lGamma_si*lS1_i) + ((1.0-lGamma_si)*lS2_i);
		}
	}

	Beagle_LogDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover", "Beagle::GA::CrossoverBlendEsVecOp",
	    "Individuals mated (after ES blend crossover)"
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GA::CrossoverBlendEsVecOp",
	    ioIndiv1
	);
	Beagle_LogObjectDebugM(
	    ioContext1.getSystem().getLogger(),
	    "crossover",
	    "Beagle::GA::CrossoverBlendEsVecOp",
	    ioIndiv2
	);

	return true;
	Beagle_StackTraceEndM("bool GA::CrossoverBlendESVecOp::mate(Individual& ioIndiv1, Context& ioContext1, Individual& ioIndiv2, Context& ioContext2)");
}


