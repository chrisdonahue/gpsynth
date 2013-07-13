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
 *  \file   beagle/MigrationBuffer.hpp
 *  \brief  Definition of the class MigrationBuffer.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.5 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_MigrationBuffer_hpp
#define Beagle_MigrationBuffer_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Individual.hpp"
#include "beagle/Member.hpp"
#include <vector>

namespace Beagle
{

// Forward declaration
class Deme;

/*!
 *  \class MigrationBuffer beagle/MigrationBuffer.hpp "beagle/MigrationBuffer.hpp"
 *  \brief A migration buffer is used to contain the state of individual migration between demes.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class MigrationBuffer : public Member
{

public:

	//! MigrationBuffer allocator type.
	typedef AllocatorT<MigrationBuffer,Member::Alloc> Alloc;
	//! MigrationBuffer handle type.
	typedef PointerT<MigrationBuffer,Member::Handle> Handle;
	//! MigrationBuffer mixed bag type.
	typedef ContainerT<MigrationBuffer,Member::Bag> Bag;

	//! Construct migration buffer
	MigrationBuffer()
	{ }
	virtual ~MigrationBuffer()
	{ }

	void                        clear();
	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual const std::string&  getName() const;
	virtual const std::string&  getType() const;
	void                        insertEmigrants(const std::vector<unsigned int>& inEmigrantIndices, Deme& ioDeme, Context& ioContext);
	void                        insertReplaced(const std::vector<unsigned int>& inReplacedIndices, Deme& ioDeme);
	void                        mergeImmigrantsToDeme(Deme& ioDeme);
	void                        moveMigrants(unsigned int inNbMigrants, Deme& ioDestDeme, Context& ioContext);
	virtual void                readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
	virtual void                writeContent(PACC::XML::Streamer &ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return number of emigrants available in current migration buffer.
	 *  \return Number of emigrants available in current migration buffer.
	 */
	inline unsigned int getNumberOfEmigrants() const
	{
		Beagle_StackTraceBeginM();
		return mEmigrants.size();
		Beagle_StackTraceEndM("unsigned int MigrationBuffer::getNumberOfEmigrants() const");
	}

	/*!
	 *  \brief Return number of immigrants available in current migration buffer.
	 *  \return Number of emigrants available in current migration buffer.
	 */
	inline unsigned int getNumberOfImmigrants() const
	{
		Beagle_StackTraceBeginM();
		return mImmigrants.size();
		Beagle_StackTraceEndM("unsigned int MigrationBuffer::getNumberOfImmigrants() const");
	}

protected:

	std::deque<Individual::Handle> mEmigrants;         //!< Buffer of emigrant individuals.
	std::deque<Individual::Handle> mImmigrants;        //!< Individuals waiting to be inserted in deme.
	std::deque<unsigned int>       mReplacedIndices;   //!< Individual to be replaced by immigrants.

};

}

#endif // Beagle_Deme_hpp
