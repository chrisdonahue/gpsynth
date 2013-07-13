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
 *  \file   beagle/TerminationOp.hpp
 *  \brief  Definition of the class TerminationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_TerminationOp_hpp
#define Beagle_TerminationOp_hpp

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
 *  \class TerminationOp beagle/TerminationOp.hpp "beagle/TerminationOp.hpp"
 *  \brief Check for termination criterion reached operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \par Note:
 *    The operator contained an handle to an other termination operator handle.
 *    If the handle is non-null, the termination test for the other termination operator is done
 *    and the evolution is stopped if necessary.
 */
class TerminationOp : public Operator
{

public:

	//! TerminationOp allocator type
	typedef AbstractAllocT<TerminationOp,Operator::Alloc> Alloc;
	//! TerminationOp handle type.
	typedef PointerT<TerminationOp,Operator::Handle> Handle;
	//! TerminationOp bag type.
	typedef ContainerT<TerminationOp,Operator::Bag> Bag;

	explicit TerminationOp(std::string inName="TerminationOp");
	virtual ~TerminationOp()
	{ }

	/*!
	 *  \brief Check if the termination criterion is reached.
	 *  \param inDeme Actual deme of the evolution.
	 *  \param ioContext Actual evolution context.
	 *  \return True if the ending criterion is reached, false if not.
	 */
	virtual bool terminate(const Deme& inDeme, Context& ioContext) = 0;

	virtual void operate(Deme& ioDeme, Context& ioContext);

};

}

#endif // Beagle_TerminationOp_hpp
