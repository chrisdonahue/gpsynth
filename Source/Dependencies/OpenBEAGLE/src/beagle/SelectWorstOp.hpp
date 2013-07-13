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
 *  \file   beagle/SelectWorstOp.hpp
 *  \brief  Definition of the class SelectWorstOp.
 *  \author Christian Gagne
 *  $Revision $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_SelectWorstOp_hpp
#define Beagle_SelectWorstOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/SelectionOp.hpp"


namespace Beagle
{

/*!
 *  \class SelectWorstOp beagle/SelectWorstOp.hpp "beagle/SelectWorstOp.hpp"
 *  \brief Select worst individual(s) of the population.
 *  \ingroup ECF
 *  \ingroup Op
 */
class SelectWorstOp : public SelectionOp
{

public:

	//! SelectWorstOp allocator type.
	typedef AllocatorT<SelectWorstOp,SelectionOp::Alloc> Alloc;
	//! SelectWorstOp handle type.
	typedef PointerT<SelectWorstOp,SelectionOp::Handle> Handle;
	//! SelectWorstOp bag type.
	typedef ContainerT<SelectWorstOp,SelectionOp::Bag> Bag;

	explicit SelectWorstOp(std::string inReproProbaName="ec.repro.prob",
	                       std::string inName="SelectWorstOp");
	virtual ~SelectWorstOp()
	{ }

	virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext);
	virtual void         selectNIndividuals(unsigned int inN,
	                                        Individual::Bag& ioPool,
	                                        Context& ioContext,
	                                        std::vector<unsigned int>& outSelections);

};

}

#endif // Beagle_SelectWorstOp_hpp
