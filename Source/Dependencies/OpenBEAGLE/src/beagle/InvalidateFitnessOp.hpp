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
 *  \file   beagle/InvalidateFitnessOp.hpp
 *  \brief  Definition of class InvalidateFitnessOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_InvalidateFitnessOp_hpp
#define Beagle_InvalidateFitnessOp_hpp

#include "beagle/BreederOp.hpp"


namespace Beagle
{


/*!
 *  \class InvalidateFitnessOp beagle/InvalidateFitnessOp.hpp "beagle/InvalidateFitnessOp.hpp"
 *  \brief Invalidate individual's fitness operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class InvalidateFitnessOp : public BreederOp
{

public:

	//! InvalidateFitnessOp allocator type.
	typedef AllocatorT<InvalidateFitnessOp,BreederOp::Alloc> Alloc;
	//! InvalidateFitnessOp handle type.
	typedef PointerT<InvalidateFitnessOp,BreederOp::Handle> Handle;
	//! InvalidateFitnessOp bag type.
	typedef ContainerT<InvalidateFitnessOp,BreederOp::Bag> Bag;

	explicit InvalidateFitnessOp(std::string inName="InvalidateFitnessOp");
	virtual ~InvalidateFitnessOp()
	{ }

	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual double             getBreedingProba(BreederNode::Handle inChild);
	virtual void               operate(Deme& ioDeme, Context& ioContext);

};

} // end of namespace Beagle


#endif // Beagle_InvalidateFitnessOp_hpp
