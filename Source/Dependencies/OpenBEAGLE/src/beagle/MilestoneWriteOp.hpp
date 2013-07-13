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
 *  \file   beagle/MilestoneWriteOp.hpp
 *  \brief  Definition of the class MilestoneWriteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:44 $
 */

#ifndef Beagle_MilestoneWriteOp_hpp
#define Beagle_MilestoneWriteOp_hpp

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
 *  \class MilestoneWriteOp beagle/MilestoneWriteOp.hpp "beagle/MilestoneWriteOp.hpp"
 *  \brief Write a milestone operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MilestoneWriteOp : public Operator
{

public:

	//! MilestoneWriteOp allocator type
	typedef AllocatorT<MilestoneWriteOp,Operator::Alloc>
	Alloc;
	//! MilestoneWriteOp handle type.
	typedef PointerT<MilestoneWriteOp,Operator::Handle>
	Handle;
	//! MilestoneWriteOp bag type.
	typedef ContainerT<MilestoneWriteOp,Operator::Bag>
	Bag;

	explicit MilestoneWriteOp(std::string inName="MilestoneWriteOp");
	virtual ~MilestoneWriteOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void writeMilestone(std::string inFilename, Context& ioContext);

private:
	String::Handle    mMilestonePrefix;     //!< Prefix of the milestone filenames.
	UInt::Handle      mWritingInterval;     //!< Milestone's writing interval (in generation).
	Bool::Handle      mPerDemeMilestone;    //!< True: write a milestone for every deme.
	Bool::Handle      mOverwriteMilestone;  //!< True: Overwrite file. False: Backup everything.
	UIntArray::Handle mPopSize;             //!< Population size for the evolution.

#ifdef BEAGLE_HAVE_LIBZ
	Bool::Handle          mCompressMilestone;   //!< True: compress file, false: keep as clear text.
#endif // BEAGLE_HAVE_LIBZ

};

}

#endif // Beagle_MilestoneWriteOp_hpp
