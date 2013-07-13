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
 *  \file   beagle/InitializationOp.hpp
 *  \brief  Definition of the class InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_InitializationOp_hpp
#define Beagle_InitializationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/BreederOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/String.hpp"
#include "beagle/Double.hpp"
#include "beagle/Individual.hpp"


namespace Beagle
{

/*!
 *  \class InitializationOp beagle/InitializationOp.hpp "beagle/InitializationOp.hpp"
 *  \brief Abstract initialization operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup Pop
 */
class InitializationOp : public BreederOp
{

public:

	//! InitializationOp allocator type.
	typedef AbstractAllocT<InitializationOp,BreederOp::Alloc>
	Alloc;
	//! InitializationOp handle type.
	typedef PointerT<InitializationOp,BreederOp::Handle>
	Handle;
	//! InitializationOp  bag type.
	typedef ContainerT<InitializationOp,BreederOp::Bag>
	Bag;

	explicit InitializationOp(std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="InitializationOp");
	virtual ~InitializationOp()
	{ }

	/*!
	 *  \brief Initialize an individual.
	 *  \param outIndividual Individual to initialize.
	 *  \param ioContext Evolution context.
	 */
	virtual void initIndividual(Individual& outIndividual, Context& ioContext) = 0;

	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual double       getBreedingProba(BreederNode::Handle inChild);
	virtual void         registerParams(System& ioSystem);
	virtual void         operate(Deme& ioDeme, Context& ioContext);
	virtual unsigned int readSeeds(std::string inFileName, Deme& ioDeme, Context& ioContext);
	virtual void         readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void         writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Double::Handle    mReproductionProba;    //!< Reproduction probability.
	std::string       mReproProbaName;       //!< Reproduction probability name.
	UIntArray::Handle mPopSize;              //!< Population size for the evolution.
	String::Handle    mSeedsFile;            //!< File to use to seed population.

};

}

#endif // Beagle_InitializationOp_hpp
