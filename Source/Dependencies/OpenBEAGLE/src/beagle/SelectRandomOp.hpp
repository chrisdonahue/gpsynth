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
 *  \file   beagle/SelectRandomOp.hpp
 *  \brief  Definition of the class SelectRandomOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_SelectRandomOp_hpp
#define Beagle_SelectRandomOp_hpp

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
 *  \class SelectRandomOp beagle/SelectRandomOp.hpp "beagle/SelectRandomOp.hpp"
 *  \brief Select an individual in a population randomly operator class (uniform distribution).
 *  \ingroup ECF
 *  \ingroup Op
 */
class SelectRandomOp : public SelectionOp
{

public:

	//! SelectRandomOp allocator type.
	typedef AllocatorT<SelectRandomOp,SelectionOp::Alloc> Alloc;
	//! SelectRandomOp handle type.
	typedef PointerT<SelectRandomOp,SelectionOp::Handle> Handle;
	//! SelectRandomOp bag type.
	typedef ContainerT<SelectRandomOp,SelectionOp::Bag> Bag;

	explicit SelectRandomOp(std::string inReproProbaName="ec.repro.prob",
	                        std::string inName="SelectRandomOp");
	virtual ~SelectRandomOp()
	{ }

	virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext);

};

}

#endif // Beagle_SelectRandomOp_hpp
