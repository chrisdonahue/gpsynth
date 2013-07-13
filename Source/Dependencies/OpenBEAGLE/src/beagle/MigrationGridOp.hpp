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
 *  \file   beagle/MigrationGridOp.hpp
 *  \brief  Definition of the class MigrationGridOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_MigrationGridOp_hpp
#define Beagle_MigrationGridOp_hpp

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
 *  \class MigrationGridOp beagle/MigrationGridOp.hpp "beagle/MigrationGridOp.hpp"
 *  \brief Migrate individuals following a grid topology.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  This migration operator exchange individuals following a grid topology.
 */
class MigrationGridOp : public MigrationMapOp
{

public:

	//! MigrationGridOp allocator type
	typedef AllocatorT<MigrationGridOp,MigrationMapOp::Alloc>
	Alloc;
	//! MigrationGridOp handle type.
	typedef PointerT<MigrationGridOp,MigrationMapOp::Handle>
	Handle;
	//! MigrationGridOp bag type.
	typedef ContainerT<MigrationGridOp,MigrationMapOp::Bag>
	Bag;

	explicit MigrationGridOp(std::string inName="MigrationGridOp");
	explicit MigrationGridOp(SelectionOp::Handle inSelectionOp,
	                         SelectionOp::Handle inReplacementOp,
	                         std::string inName="MigrationGridOp");
	virtual ~MigrationGridOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void init(System& ioSystem);

protected:

	UInt::Handle mNumberMigrants; //!< Number of migrants.
	UInt::Handle mGridWidth;      //!< Width of the grid (height is determined automatically).
	Bool::Handle mToroidal;       //!< If grid will be toroidal (wraps left-right, top-bottom).

};

}

#endif // Beagle_MigrationGridOp_hpp
