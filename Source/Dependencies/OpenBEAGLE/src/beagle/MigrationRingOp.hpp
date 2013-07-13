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
 *  \file   beagle/MigrationRingOp.hpp
 *  \brief  Definition of the class MigrationRingOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.9 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_MigrationRingOp_hpp
#define Beagle_MigrationRingOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/MigrationOp.hpp"
#include "beagle/Individual.hpp"

namespace Beagle
{

/*!
 *  \class MigrationRingOp beagle/MigrationRingOp.hpp "beagle/MigrationRingOp.hpp"
 *  \brief Migrate individuals choosen randomly, following a ring topology, operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MigrationRingOp : public MigrationMapOp
{

public:

	//! MigrationRingOp allocator type
	typedef AllocatorT<MigrationRingOp,MigrationMapOp::Alloc>
	Alloc;
	//! MigrationRingOp handle type.
	typedef PointerT<MigrationRingOp,MigrationMapOp::Handle>
	Handle;
	//! MigrationRingOp bag type.
	typedef ContainerT<MigrationRingOp,MigrationMapOp::Bag>
	Bag;

	explicit MigrationRingOp(std::string inName="MigrationRingOp");
	explicit MigrationRingOp(SelectionOp::Handle inSelectionOp,
	                         SelectionOp::Handle inReplacementOp,
	                         std::string inName="MigrationRingOp");
	virtual ~MigrationRingOp()
	{ }

	virtual void init(System& ioSystem);
	virtual void registerParams(System& ioSystem);

protected:

	UInt::Handle mNumberMigrants;     //!< Number of migrants

};

}

#endif // Beagle_MigrationRingOp_hpp
