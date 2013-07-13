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
 *  \file   beagle/TermMaxGenOp.hpp
 *  \brief  Definition of the class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_TermMaxGenOp_hpp
#define Beagle_TermMaxGenOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Context.hpp"
#include "beagle/System.hpp"


namespace Beagle
{

/*!
 *  \class TermMaxGenOp beagle/TermMaxGenOp.hpp "beagle/TermMaxGenOp.hpp"
 *  \brief Maximum generation termination criterion operator.
 *  \ingroup ECF
 *  \ingroup Op
 */
class TermMaxGenOp : public TerminationOp
{

public:

	//! TermMaxGenOp allocator type
	typedef AllocatorT<TermMaxGenOp,TerminationOp::Alloc> Alloc;
	//! TermMaxGenOp handle type.
	typedef PointerT<TermMaxGenOp,TerminationOp::Handle> Handle;
	//! TermMaxGenOp bag type.
	typedef ContainerT<TermMaxGenOp,TerminationOp::Bag> Bag;

	explicit TermMaxGenOp(std::string inName="TermMaxGenOp");
	virtual ~TermMaxGenOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:
	UInt::Handle mMaxGeneration;     //!< Maximum number of generation of the evolution.

};

}

#endif // Beagle_TerminationCheckOp_hpp
