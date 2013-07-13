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
 *  \file   beagle/RandomShuffleDemeOp.hpp
 *  \brief  Definition of the class RandomShuffleDemeOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_RandomShuffleDemeOp_hpp
#define Beagle_RandomShuffleDemeOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"

namespace Beagle
{

/*!
 *  \class RandomShuffleDemeOp beagle/RandomShuffleDemeOp.hpp "beagle/RandomShuffleDemeOp.hpp"
 *  \brief Random shuffle a deme operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  Random shuffle order of individuals in the actual deme.
 */
class RandomShuffleDemeOp : public Operator
{

public:

	//! RandomShuffleDemeOp allocator type
	typedef AllocatorT<RandomShuffleDemeOp,Operator::Alloc> Alloc;
	//! RandomShuffleDemeOp handle type.
	typedef PointerT<RandomShuffleDemeOp,Operator::Handle> Handle;
	//! RandomShuffleDemeOp bag type.
	typedef ContainerT<RandomShuffleDemeOp,Operator::Bag> Bag;

	explicit RandomShuffleDemeOp(std::string inName="RandomShuffleDemeOp");
	virtual ~RandomShuffleDemeOp()
	{ }

	virtual void operate(Deme& ioDeme, Context& ioContext);

};

}

#endif // Beagle_RandomShuffleDemeOp_hpp
