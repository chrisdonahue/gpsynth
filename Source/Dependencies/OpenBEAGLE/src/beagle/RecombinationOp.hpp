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
 *  \file   beagle/RecombinationOp.hpp
 *  \brief  Definition of the class RecombinationOp.
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_RecombinationOp_hpp
#define Beagle_RecombinationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
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
 *  \class RecombinationOp beagle/RecombinationOp.hpp "beagle/RecombinationOp.hpp"
 *  \brief Abstract recombination operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class RecombinationOp : public BreederOp
{

public:

	//! RecombinationOp allocator type.
	typedef AbstractAllocT<RecombinationOp,BreederOp::Alloc> Alloc;
	//! RecombinationOp handle type.
	typedef PointerT<RecombinationOp,BreederOp::Handle> Handle;
	//! RecombinationOp bag type.
	typedef ContainerT<RecombinationOp,BreederOp::Bag> Bag;

	explicit RecombinationOp(std::string inRecombProbaName="ec.rec.prob",
	                         std::string inNumberRecombName="ec.rec.nbrind",
	                         std::string inName="RecombinationOp");
	virtual ~RecombinationOp()
	{ }

	/*!
	 *  \brief Recombine several individuals together.
	 *  \param inIndivPool Pool of individuals to recombine.
	 *  \param ioContext Evolutionary context.
	 *  \return Individual resulting of recombination.
	 */
	virtual Individual::Handle recombine(Individual::Bag& inIndivPool, Context& ioContext) =0;

	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual double             getBreedingProba(BreederNode::Handle inChild);
	virtual void               registerParams(System& ioSystem);
	virtual void               operate(Deme& ioDeme, Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Double::Handle mRecombProba;       //!< Single individual recombination probability.
	std::string   mRecombProbaName;   //!< Single individual recombination probability parameter name.
	UInt::Handle  mNumberRecomb;      //!< Number of individuals recombined.
	std::string   mNumberRecombName;  //!< Number of individuals recombined parameter name.

};

}

#endif // Beagle_RecombinationOp_hpp
