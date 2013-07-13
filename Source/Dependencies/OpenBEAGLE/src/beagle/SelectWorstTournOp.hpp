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
 *  \file   beagle/SelectWorstTournOp.hpp
 *  \brief  Definition of the class SelectWorstTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1 $
 *  $Date: 2007/08/29 17:38:32 $
 */

#ifndef Beagle_SelectWorstTournOp_hpp
#define Beagle_SelectWorstTournOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/SelectionOp.hpp"
#include "beagle/UInt.hpp"


namespace Beagle
{

/*!
 *  \class SelectWorstTournOp beagle/SelectWorstTournOp.hpp "beagle/SelectWorstTournOp.hpp"
 *  \brief Selection of worst individual of a tournament operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class SelectWorstTournOp : public SelectionOp
{

public:

	//! SelectWorstTournOp allocator type.
	typedef AllocatorT<SelectWorstTournOp,SelectionOp::Alloc> Alloc;
	//! SelectWorstTournOp handle type.
	typedef PointerT<SelectWorstTournOp,SelectionOp::Handle> Handle;
	//! SelectWorstTournOp bag type.
	typedef ContainerT<SelectWorstTournOp,SelectionOp::Bag> Bag;

	explicit SelectWorstTournOp(std::string inReproProbaName="ec.repro.prob",
	                            std::string inName="SelectWorstTournOp");
	virtual ~SelectWorstTournOp()
	{ }

	virtual void         registerParams(System& ioSystem);
	virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext);

protected:

	UInt::Handle mNumberParticipants;  //!< Number of participants to each tournament.

};

}

#endif // Beagle_SelectWorstTournOp_hpp
