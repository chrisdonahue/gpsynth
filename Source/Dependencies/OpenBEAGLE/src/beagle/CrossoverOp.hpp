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
 *  \file   beagle/CrossoverOp.hpp
 *  \brief  Definition of the class CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_CrossoverOp_hpp
#define Beagle_CrossoverOp_hpp

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
 *  \class CrossoverOp beagle/CrossoverOp.hpp "beagle/CrossoverOp.hpp"
 *  \brief Abstract crossover operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class CrossoverOp : public BreederOp
{

public:

	//! CrossoverOp allocator type.
	typedef AbstractAllocT<CrossoverOp,BreederOp::Alloc>
	Alloc;
	//! CrossoverOp handle type.
	typedef PointerT<CrossoverOp,BreederOp::Handle>
	Handle;
	//! CrossoverOp bag type.
	typedef ContainerT<CrossoverOp,BreederOp::Bag>
	Bag;

	explicit CrossoverOp(std::string inMatingPbName="ec.cx.prob",
	                     std::string inName="CrossoverOp");
	virtual ~CrossoverOp()
	{ }

	/*!
	 *  \brief Mate two individuals for a crossover.
	 *  \param ioIndiv1   First individual to mate.
	 *  \param ioContext1 Evolutionary context of the first individual.
	 *  \param ioIndiv2   Second individual to mate.
	 *  \param ioContext2 Evolutionary context of the second individual.
	 *  \return True if the individuals are effectively mated, false if not.
	 */
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2) = 0;

	virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
	                                 BreederNode::Handle inChild,
	                                 Context& ioContext);
	virtual double             getBreedingProba(BreederNode::Handle inChild);
	virtual void               registerParams(System& ioSystem);
	virtual void               operate(Deme& ioDeme, Context& ioContext);
	virtual void               readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
	Double::Handle mMatingProba;      //!< Single individual mating probability
	std::string   mMatingProbaName;  //!< Single individual mating probability parameter name

};

}

#endif // Beagle_CrossoverOp_hpp
