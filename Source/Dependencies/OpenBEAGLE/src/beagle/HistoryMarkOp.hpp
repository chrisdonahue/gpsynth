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
 *  \file   beagle/HistoryMarkOp.hpp
 *  \brief  Definition of the class HistoryMarkOp.
 *  \author Alexandre Devert
 *  $Revision: 1.6 $
 *  $Date: 2007/08/17 20:58:00 $
 */

#ifndef Beagle_HistoryMarkOp_hpp
#define Beagle_HistoryMarkOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"
#include "beagle/String.hpp"
#include "beagle/Bool.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"


namespace Beagle
{

/*!
 *  \class HistoryMarkOp beagle/HistoryMarkOp.hpp "beagle/HistoryMarkOp.hpp"
 *  \brief Flush history when writing a milestone operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup History
 */
class HistoryMarkOp : public Operator
{

public:

	//! HistoryMarkOp allocator type
	typedef AllocatorT<HistoryMarkOp,Operator::Alloc> Alloc;
	//! HistoryMarkOp handle type.
	typedef PointerT<HistoryMarkOp,Operator::Handle> Handle;
	//! HistoryMarkOp bag type.
	typedef ContainerT<HistoryMarkOp,Operator::Bag> Bag;

	explicit HistoryMarkOp(std::string inName="HistoryMarkOp");
	virtual ~HistoryMarkOp()
	{ }

	virtual void operate(Deme& ioDeme, Context& ioContext);

protected:

	virtual void mark(Individual::Bag& ioIndividuals, Context& ioContext);

};

}

#endif // Beagle_HistoryMarkOp_hpp
