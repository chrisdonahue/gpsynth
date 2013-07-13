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
 *  \file   beagle/GenerationalOp.hpp
 *  \brief  Definition of the class GenerationalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:12 $
 */


#ifndef Beagle_GenerationalOp_hpp
#define Beagle_GenerationalOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"

namespace Beagle
{

/*!
 *  \class GenerationalOp beagle/GenerationalOp.hpp "beagle/GenerationalOp.hpp"
 *  \brief Generational replacement strategy operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  A generational operator process deme with a breeding tree following a generation by generation
 *  replacement strategy.
 */
class GenerationalOp : public ReplacementStrategyOp
{

public:

	//! GenerationalOp allocator type.
	typedef AllocatorT<GenerationalOp,ReplacementStrategyOp::Alloc>
	Alloc;
	//! GenerationalOp handle type.
	typedef PointerT<GenerationalOp,ReplacementStrategyOp::Handle>
	Handle;
	//! GenerationalOp bag type.
	typedef ContainerT<GenerationalOp,ReplacementStrategyOp::Bag>
	Bag;

	GenerationalOp(std::string inName="GenerationalOp");
	virtual ~GenerationalOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);

protected:
	UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism

};

}

#endif // Beagle_GenerationalOp_hpp

