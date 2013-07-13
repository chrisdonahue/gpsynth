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
 *  \file   beagle/GA/CrossoverOnePointOpT.hpp
 *  \brief  Definition of the class GA::CrossoverOnePointOpT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17 $
 *  $Date: 2007/08/17 18:09:10 $
 */

#ifndef Beagle_GA_CrossoverOnePointOpT_hpp
#define Beagle_GA_CrossoverOnePointOpT_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverOnePointOpT beagle/GA/CrossoverOnePointOpT.hpp
 *    "beagle/GA/CrossoverOnePointOpT.hpp"
 *  \brief GA one point crossover operator class.
 *  \param T GA representation type used in crossover.
 *  \ingroup GAF
 *
 *  The GA genotype type T must fullfil the std::vector template interface.
 */
template <class T>
class CrossoverOnePointOpT : public CrossoverOp
{

public:

	//! GA::CrossoverOnePointOpT allocator type.
	typedef AllocatorT<CrossoverOnePointOpT<T>,CrossoverOp::Alloc>
	Alloc;
	//! GA::CrossoverOnePointOpT handle type.
	typedef PointerT<CrossoverOnePointOpT<T>,CrossoverOp::Handle>
	Handle;
	//! GA::CrossoverOnePointOpT bag type.
	typedef ContainerT<CrossoverOnePointOpT<T>,CrossoverOp::Bag>
	Bag;

	explicit CrossoverOnePointOpT(std::string inMatingPbName="ga.cx1p.prob",
	                              std::string inName="GA-CrossoverOnePointOpT");
	virtual ~CrossoverOnePointOpT()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);

};

}
}


/*!
 *  \brief Construct a GA one point crossover operator.
 *  \param inMatingPbName Mating probability parameter name.
 *  \param inName Name of the operator.
 */
template <class T>
Beagle::GA::CrossoverOnePointOpT<T>::CrossoverOnePointOpT(std::string inMatingPbName,
        std::string inName) :
		Beagle::CrossoverOp(inMatingPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the GA one point crossover operator.
 *  \param ioSystem System of the evolution.
 */
template <class T>
void Beagle::GA::CrossoverOnePointOpT<T>::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lProbaDescription(
		    "Individual 1-point crossover pb.",
		    "Double",
		    "0.3",
		    "GA one-point crossover probability of a single individual."
		);
		mMatingProba = castHandleT<Double>(
		                   ioSystem.getRegister().insertEntry(mMatingProbaName, new Double(0.3f), lProbaDescription));
	}
	CrossoverOp::registerParams(ioSystem);
	Beagle_StackTraceEndM("void GA::CrossoverOnePointOpT<T>::registerParams(System& ioSystem)");
}


/*!
 *  \brief Mate two GA individuals for one point crossover.
 *  \param ioIndiv1   First individual to mate.
 *  \param ioContext1 Evolutionary context of the first individual.
 *  \param ioIndiv2   Second individual to mate.
 *  \param ioContext2 Evolutionary context of the second individual.
 *  \return True if the individuals are effectively mated, false if not.
 */
template <class T>
bool Beagle::GA::CrossoverOnePointOpT<T>::mate(Beagle::Individual& ioIndiv1,
        Beagle::Context&    ioContext1,
        Beagle::Individual& ioIndiv2,
        Beagle::Context&    ioContext2)
{
	Beagle_StackTraceBeginM();
	unsigned int lNbGenotypes = minOf<unsigned int>(ioIndiv1.size(), ioIndiv2.size());
	if(lNbGenotypes == 0) return false;
	if(lNbGenotypes == 1) {
		typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[0]);
		typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[0]);
		unsigned int lSize = minOf<unsigned int>(lGenotype1->size(), lGenotype2->size());
		if(lSize < 2) return false;
		unsigned int lMatingPoint = ioContext1.getSystem().getRandomizer().rollInteger(1, (lSize-1));

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    "Genotypes mated (before GA one point crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype2
		);
		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    std::string("Mating individuals before the ")+
		    uint2ordinal(lMatingPoint+1)+std::string(" element")
		);

		for(unsigned int i=0; i<lMatingPoint; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    "Genotypes mated (after GA one point crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype2
		);
	} else {
		unsigned int lTotalSize = 0;
		std::vector<unsigned int> lSizes;
		for(unsigned int i=0; i<lNbGenotypes; i++) {
			typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[i]);
			typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[i]);
			unsigned int lSizeI = minOf<unsigned int>(lGenotype1->size(), lGenotype2->size());
			lSizes.push_back(lSizeI);
			lTotalSize += lSizeI;
		}
		if(lTotalSize < 2) return false;
		unsigned int lMatingPoint =
		    ioContext1.getSystem().getRandomizer().rollInteger(1, (lTotalSize-1));
		unsigned int lMatingGenotype = 0;
		for(; (lMatingGenotype+1)<lNbGenotypes; lMatingGenotype++) {
			if(lMatingPoint < lSizes[lMatingGenotype]) break;
			else lMatingPoint -= lSizes[lMatingGenotype];
		}

		Beagle_LogVerboseM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    std::string("Mating the ")+uint2ordinal(lMatingGenotype+1)+
		    std::string(" genotypes before the ")+uint2ordinal(lMatingPoint+1)+std::string(" element")
		);

		typename T::Handle lGenotype1 = castHandleT<T>(ioIndiv1[lMatingGenotype]);
		typename T::Handle lGenotype2 = castHandleT<T>(ioIndiv2[lMatingGenotype]);

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    "Genotypes mated (before GA one point crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover", "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype2
		);

		for(unsigned int i=0; i<lMatingPoint; ++i) {
			typename T::value_type lTemp = (*lGenotype1)[i];
			(*lGenotype1)[i] = (*lGenotype2)[i];
			(*lGenotype2)[i] = lTemp;
		}

		Beagle_LogDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",  "Beagle::GA::CrossoverOnePointOpT",
		    "Genotypes mated (after GA one point crossover)"
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",  "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype1
		);
		Beagle_LogObjectDebugM(
		    ioContext1.getSystem().getLogger(),
		    "crossover",  "Beagle::GA::CrossoverOnePointOpT",
		    *lGenotype2
		);
	}

	return true;
	Beagle_StackTraceEndM("bool GA::CrossoverOnePointOpT<T>::mate(Individual& ioIndiv1, Context& ioContext1, Individual& ioIndiv2, Context& ioContext2)");
}


#endif // Beagle_GA_CrossoverOnePointOpT_hpp
