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
 *  \file   beagle/AlgoGenerational.hpp
 *  \brief  Definition of the class AlgoGenerational.
 *  \author Christian Gagne
 *  $Revision: 1.2 $
 *  $Date: 2007/08/17 18:09:11 $
 */

#ifndef Beagle_AlgoGenerational_hpp
#define Beagle_AlgoGenerational_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Algorithm.hpp"


namespace Beagle
{

/*!
 *  \class AlgoGenerational beagle/AlgoGenerational.hpp "beagle/AlgoGenerational.hpp"
 *  \brief Generational algorithm class, rely on GenerationalOp replacement strategy.
 *  \ingroup ECF
 */
class AlgoGenerational : public Algorithm
{

public:

	//! AlgoGenerational allocator type.
	typedef AllocatorT< AlgoGenerational, Algorithm::Alloc > Alloc;
	//! AlgoGenerational handle type.
	typedef PointerT< AlgoGenerational, Algorithm::Handle > Handle;
	//! AlgoGenerational bag type.
	typedef ContainerT< AlgoGenerational, Algorithm::Bag > Bag;

	AlgoGenerational(void);
	virtual ~AlgoGenerational()
	{ }

	virtual void configure(Evolver& ioEvolver, System& ioSystem);

};

}

#endif // Beagle_AlgoGenerational_hpp
