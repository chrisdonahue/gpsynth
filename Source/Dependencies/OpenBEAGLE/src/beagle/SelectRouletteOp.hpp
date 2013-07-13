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
 *  \file   beagle/SelectRouletteOp.hpp
 *  \brief  Definition of the class SelectRouletteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_SelectRouletteOp_hpp
#define Beagle_SelectRouletteOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/SelectionOp.hpp"
#include "beagle/RouletteT.hpp"


namespace Beagle
{

/*!
 *  \class SelectRouletteOp beagle/SelectRouletteOp.hpp "beagle/SelectRouletteOp.hpp"
 *  \brief Proportionnal roulette selection operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \warning To use roulette selection, every individuals composing the population must have 
 *    non-negative fitness value. Roulette selection is valid only with FitnessSimple object
 *    and derived (FitnessSimpleMin, FitnessKoza).
 */
class SelectRouletteOp : public SelectionOp
{

public:

	//! SelectRouletteOp allocator type.
	typedef AllocatorT<SelectRouletteOp,SelectionOp::Alloc> Alloc;
	//! SelectRouletteOp handle type.
	typedef PointerT<SelectRouletteOp,SelectionOp::Handle> Handle;
	//! SelectRouletteOp bag type.
	typedef ContainerT<SelectRouletteOp,SelectionOp::Bag> Bag;

	explicit SelectRouletteOp(std::string inReproProbaName="ec.repro.prob",
	                          std::string inName="SelectRouletteOp");
	virtual ~SelectRouletteOp()
	{ }

	virtual void         operate(Deme& ioDeme, Context& ioContext);
	virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext);

protected:

	RouletteT<unsigned int> mRoulette;       //!< Roulette with cumulative probabilities.
	bool                    mRouletteValid;  //!< Flag whether roulette is still valid.

};

}

#endif // Beagle_SelectRouletteOp_hpp

