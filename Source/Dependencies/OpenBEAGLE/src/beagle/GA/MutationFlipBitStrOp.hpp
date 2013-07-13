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
 *  \file   beagle/GA/MutationFlipBitStrOp.hpp
 *  \brief  Definition of the class GA::MutationFlipBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_MutationFlipBitStrOp_hpp
#define Beagle_GA_MutationFlipBitStrOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle
{
namespace GA
{

/*!
 *  \class MutationFlipBitStrOp beagle/GA/MutationFlipBitStrOp.hpp "beagle/GA/MutationFlipBitStrOp.hpp"
 *  \brief Bit string GA flip bit mutation operator class.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class MutationFlipBitStrOp : public Beagle::MutationOp
{

public:

	//! GA::MutationFlipBitStrOp allocator type.
	typedef AllocatorT<MutationFlipBitStrOp,Beagle::MutationOp::Alloc>
	Alloc;
	//! GA::MutationFlipBitStrOp handle type.
	typedef PointerT<MutationFlipBitStrOp,Beagle::MutationOp::Handle>
	Handle;
	//! GA::MutationFlipBitStrOp bag type.
	typedef ContainerT<MutationFlipBitStrOp,Beagle::MutationOp::Bag>
	Bag;

	explicit MutationFlipBitStrOp(std::string inMutationPbName="ga.mutflip.indpb",
	                              std::string inBitMutatePbName="ga.mutflip.bitpb",
	                              std::string inName="GA-MutationFlipBitStrOp");
	virtual ~MutationFlipBitStrOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Float::Handle mBitMutateProba;   //!< Single bit mutation probability.
	std::string   mBitMutatePbName;  //!< Single bit mutation probability parameter name.

};

}
}

#endif // Beagle_GA_MutationFlipBitStrOp_hpp
