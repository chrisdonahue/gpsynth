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
 *  \file   beagle/SelectParsimonyTournOp.hpp
 *  \brief  Definition of the class SelectParsimonyTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_SelectParsimonyTournOp_hpp
#define Beagle_SelectParsimonyTournOp_hpp

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
 *  \class SelectParsimonyTournOp beagle/SelectParsimonyTournOp.hpp
 *     "beagle/SelectParsimonyTournOp.hpp"
 *  \brief Lexicographic parsimony pressure tournament selection operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  A simple lexicographic parsimony pressure tournament selection operator, based an idea
 *  presented in: Luke, S., and L. Panait. 2002. Lexicographic Parsimony Pressure. In GECCO-2002:
 *  Proceedings of the Genetic and Evolutionary Computation Conference. W. B. Langdon et al, eds.
 *  Morgan Kauffman. 829-836.
 *
 */
class SelectParsimonyTournOp : public SelectionOp
{

public:

	//! SelectParsimonyTournOp allocator type.
	typedef AllocatorT<SelectParsimonyTournOp,SelectionOp::Alloc> Alloc;
	//! SelectParsimonyTournOp handle type.
	typedef PointerT<SelectParsimonyTournOp,SelectionOp::Handle> Handle;
	//! SelectParsimonyTournOp bag type.
	typedef ContainerT<SelectParsimonyTournOp,SelectionOp::Bag> Bag;

	explicit SelectParsimonyTournOp(std::string inReproProbaName="ec.repro.prob",
	                                std::string inName="SelectParsimonyTournOp");
	virtual ~SelectParsimonyTournOp()
	{ }

	virtual void         registerParams(System& ioSystem);
	virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext);

protected:

	UInt::Handle mNumberParticipants;  //!< Number of participants to each tournament.

};

}

#endif // Beagle_SelectParsimonyTournOp_hpp
