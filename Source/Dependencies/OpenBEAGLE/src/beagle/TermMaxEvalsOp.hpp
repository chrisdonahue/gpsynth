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
 *  \file   beagle/TermMaxEvalsOp.hpp
 *  \brief  Definition of the class TermMaxEvalsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_TermMaxEvalsOp_hpp
#define Beagle_TermMaxEvalsOp_hpp

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
 *  \class TermMaxEvalsOp beagle/TermMaxEvalsOp.hpp "beagle/TermMaxEvalsOp.hpp"
 *  \brief Maximum number of fitness evaluations termination criterion operator.
 *  \ingroup ECF
 *  \ingroup Op
 */
class TermMaxEvalsOp : public TerminationOp
{

public:

	//! TermMaxEvalsOp allocator type
	typedef AllocatorT<TermMaxEvalsOp,TerminationOp::Alloc> Alloc;
	//! TermMaxEvalsOp handle type.
	typedef PointerT<TermMaxEvalsOp,TerminationOp::Handle> Handle;
	//! TermMaxEvalsOp bag type.
	typedef ContainerT<TermMaxEvalsOp,TerminationOp::Bag> Bag;

	explicit TermMaxEvalsOp(std::string inName="TermMaxEvalsOp");
	virtual ~TermMaxEvalsOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool terminate(const Deme& inDeme, Context& ioContext);

protected:

	UInt::Handle mMaxEvaluations;   //!< Maximum number of fitness evaluations of the evolution.

};

}

#endif // Beagle_TerminationCheckOp_hpp
