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
 *  \file   beagle/AdaptBreedingProbaOp.hpp
 *  \brief  Definition of class AdaptBreedingProbaOp.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:11 $
 */


#ifndef Beagle_AdaptBreedingProbaOp_hpp
#define Beagle_AdaptBreedingProbaOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Double.hpp"
#include "beagle/Individual.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Context.hpp"
#include "beagle/BreederOp.hpp"


namespace Beagle
{

/*!
 *  \class AdaptBreedingProbaOp beagle/AdaptBreedingProbaOp.hpp "beagle/AdaptBreedingProbaOp.hpp"
 *  \brief Adaptation of breeding probabilities operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  The AdaptBreedingProbaOp operator is used to adapt the breeding probabilities of underlying
 *  variation operators (generally mutation operators) during the course of evolution, by
 *  increasing the breeding probability of operators that produces offsprings with
 *  fitness better than their parents, and decreasing the breeding probability of operators that
 *  produce offsprings with a fitness worse or equal to the fitness of their parents.
 *
 *  The adaptation of breeding probabilities is done each time two variation operators have generated
 *  a number of candidates higher or equal to the adaptation period (given by parameter
 *  'ec.adaptbreed.period'). The variation operator which generate the most offsprings of improved
 *  fitness has its breeding probability increased by a multiplication with the the inverse of the
 *  adaptation factor (given by parameter 'ec.adaptbreed.factor'), that is p'_i = p_i / c, where p_i,
 *  p'_i and c are respectively the old and new breeding probabilities, and the adaptation factor.
 *  For the variation operator that generated less offsprings of improved fitness compared the the
 *  other, the breeding probabilites is decreased by multiply it with the same adaptation factor,
 *  that is p'_j = p_j * c. After an adaptation of probabilities, all the breeding probabilities
 *  are normalized to be equal to 1.0 when summed.
 */
class AdaptBreedingProbaOp : public Beagle::BreederOp
{

public:

	//! AdaptBreedingProbaOp allocator type.
	typedef AllocatorT<AdaptBreedingProbaOp,Beagle::BreederOp::Alloc>
	Alloc;
	//! AdaptBreedingProbaOp handle type.
	typedef PointerT<AdaptBreedingProbaOp,Beagle::BreederOp::Handle>
	Handle;
	//! AdaptBreedingProbaOp bag type.
	typedef ContainerT<AdaptBreedingProbaOp,Beagle::BreederOp::Bag>
	Bag;

	explicit AdaptBreedingProbaOp(std::string inName="AdaptBreedingProbaOp");
	virtual ~AdaptBreedingProbaOp()
	{ }

	virtual double getBreedingProba(BreederNode::Handle inChild);
	virtual void  registerParams(System& ioSystem);
	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual void  operate(Deme& ioDeme, Context& ioContext);

protected:

	DoubleArray::Handle mAdaptedProbas;     //!< Selection proba different operators.
	UIntArray::Handle   mSuccessCount;      //!< Number of successful variations observed.
	UIntArray::Handle   mVariationCount;    //!< Number of variations observed.
	UInt::Handle        mAdaptationPeriod;  //!< Number of variations between adaptation.
	Double::Handle      mAdaptationFactor;  //!< Adaptation factor used to modify breeding proba.

};

}


#endif // Beagle_AdaptBreedingProbaOp_hpp
