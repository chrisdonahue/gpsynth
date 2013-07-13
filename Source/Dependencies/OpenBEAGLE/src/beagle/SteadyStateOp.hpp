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
 *  \file   beagle/SteadyStateOp.hpp
 *  \brief  Definition of the class SteadyStateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_SteadyStateOp_hpp
#define Beagle_SteadyStateOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/SelectionOp.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/ReplacementStrategyOp.hpp"


namespace Beagle
{

/*!
 *  \class SteadyStateOp beagle/SteadyStateOp.hpp "beagle/SteadyStateOp.hpp"
 *  \brief Steady state replacement strategy operator.
 *  \ingroup ECF
 *  \ingroup Op
 */
class SteadyStateOp : public ReplacementStrategyOp
{

public:

	//! SteadyStateOp allocator type
	typedef AllocatorT<SteadyStateOp,ReplacementStrategyOp::Alloc> Alloc;
	//! SteadyStateOp handle type.
	typedef PointerT<SteadyStateOp,ReplacementStrategyOp::Handle> Handle;
	//! SteadyStateOp bag type.
	typedef ContainerT<SteadyStateOp,ReplacementStrategyOp::Bag> Bag;

	explicit SteadyStateOp(std::string inName="SteadyStateOp");
	virtual ~SteadyStateOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);

protected:

	UInt::Handle  mElitismKeepSize;      //!< Number of individuals keep with elitism

};

}

#endif // Beagle_SteadyStateOp_hpp
