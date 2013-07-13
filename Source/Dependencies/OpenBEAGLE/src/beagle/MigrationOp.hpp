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
 *  \file   beagle/MigrationOp.hpp
 *  \brief  Definition of the class MigrationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.15 $
 *  $Date: 2007/08/13 13:04:34 $
 */

#ifndef Beagle_MigrationOp_hpp
#define Beagle_MigrationOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"
#include "beagle/UInt.hpp"


namespace Beagle
{

/*!
 *  \class MigrationOp beagle/MigrationOp.hpp "beagle/MigrationOp.hpp"
 *  \brief Individuals migration operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MigrationOp : public Operator
{

public:

	//! MigrationOp allocator type
	typedef AbstractAllocT<MigrationOp,Operator::Alloc>
	Alloc;
	//! MigrationOp handle type.
	typedef PointerT<MigrationOp,Operator::Handle>
	Handle;
	//! MigrationOp bag type.
	typedef ContainerT<MigrationOp,Operator::Bag>
	Bag;

	explicit MigrationOp(std::string inName="MigrationOp");
	virtual ~MigrationOp()
	{ }

	/*!
	 *  \brief Migrate individuals of a deme.
	 *  \param ioDeme Deme containing individuals to migrate.
	 *  \param ioContext Evolution context.
	 */
	virtual void migrate(Deme& ioDeme, Context& ioContext) =0;

	virtual void init(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void registerParams(System& ioSystem);

protected:

	UInt::Handle      mMigrationInterval;  //!< Random ring migration interval (in generation).
	UIntArray::Handle mPopSize;            //!< Population size for the evolution.

};

}

#endif // Beagle_MigrationOp_hpp
