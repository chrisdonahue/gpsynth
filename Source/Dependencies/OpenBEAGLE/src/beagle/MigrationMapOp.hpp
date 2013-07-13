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
 *  \file   beagle/MigrationMapOp.hpp
 *  \brief  Definition of the class MigrationMapOp.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.14 $
 *  $Date: 2007/08/13 13:04:34 $
 */

#ifndef Beagle_MigrationMapOp_hpp
#define Beagle_MigrationMapOp_hpp

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
#include "beagle/SelectionOp.hpp"


namespace Beagle
{

/*!
 *  \class MigrationMapOp beagle/MigrationMapOp.hpp "beagle/MigrationMapOp.hpp"
 *  \brief Migration operator that stores a mapping of deme to deme migration
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  readWithSystem() allows specification via configuration file of
 *  selection and replacement strategies.  So 
 *  <tt>\<NameOfMigrationOp select="SelectTournamentOp"\\\></tt>
 *  specifies that tournament selection will be used to select a deme's emigrants.
 *
 *  If a replacement operator is specified then it will be used to
 *  select which individuals will be replaced by incomming immigrants.
 *  <tt>\<NameOfMigrationOp replace="SelectRandomOp"\\\></tt> will replace
 *  randomly selected individuals with any incomming immigrants.
 
 */
class MigrationMapOp : public MigrationOp
{

public:

	//! MigrationMapOp allocator type
	typedef AllocatorT<MigrationMapOp,MigrationOp::Alloc>
	Alloc;
	//! MigrationMapOp handle type.
	typedef PointerT<MigrationMapOp,MigrationOp::Handle>
	Handle;
	//! MigrationMapOp bag type.
	typedef ContainerT<MigrationMapOp,MigrationOp::Bag>
	Bag;

	explicit MigrationMapOp(std::string inName="MigrationMapOp");
	explicit MigrationMapOp(SelectionOp::Handle inSelectionOp,
	                        SelectionOp::Handle inReplacementOp,
	                        std::string inName="MigrationMapOp");
	virtual ~MigrationMapOp()
	{ }

	virtual void migrate(Deme& ioDeme, Context& ioContext);
	virtual void init(System& ioSystem);
	virtual void registerParams(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
	virtual void writeContent(PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

protected:

	void logMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
	            System& ioSystem) const;
	void validateMap(const std::vector< std::vector<unsigned int> >& inMigrationMap,
	                 System& ioSystem) const;

	SelectionOp::Handle mSelectionOp;   //!< To select the emigrants.
	SelectionOp::Handle mReplacementOp; //!< To select individuals replaced by immigrants.

	//! Migration matrix listing number of emigrants between each deme.
	std::vector< std::vector<unsigned int> > mMigrationMap;


};

}

#endif // Beagle_MigrationMapOp_hpp
