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
 *  \file   beagle/GA/InitBitStrOp.hpp
 *  \brief  Definition of the class GA::InitBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_InitBitStrOp_hpp
#define Beagle_GA_InitBitStrOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/Float.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class InitBitStrOp beagle/GA/InitBitStrOp.hpp "beagle/GA/InitBitStrOp.hpp"
 *  \brief GA bit string initialization operator class.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class InitBitStrOp : public InitializationOp
{

public:

	//! GA::InitBitStrOp allocator type.
	typedef AllocatorT<InitBitStrOp,InitializationOp::Alloc>
	Alloc;
	//! GA::InitBitStrOp handle type.
	typedef PointerT<InitBitStrOp,InitializationOp::Handle>
	Handle;
	//! GA::InitBitStrOp bag type.
	typedef ContainerT<InitBitStrOp,InitializationOp::Bag>
	Bag;

	explicit InitBitStrOp(unsigned int inNumberBits=0,
	                      std::string inReproProbaName="ec.repro.prob",
	                      std::string inName="GA-InitBitStrOp");
	virtual ~InitBitStrOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void initIndividual(Individual& outIndividual, Context& ioContext);

	/*!
	 *  \return Number of bits in strings used for initialization.
	 */
	unsigned int getNumberBits() const
	{
		Beagle_StackTraceBeginM();
		return mNumberBits->getWrappedValue();
		Beagle_StackTraceEndM("unsigned int GA::InitBitStrOp::getNumberBits() const");
	}

	/*!
	 *  \brief Set number of bits in strings used for initialization.
	 *  \param inNumberBits Number of bits of individuals.
	 */
	void setNumberBits(unsigned int inNumberBits)
	{
		Beagle_StackTraceBeginM();
		mNumberBits->getWrappedValue() = inNumberBits;
		Beagle_StackTraceEndM("void GA::InitBitStrOp::setNumberBits(unsigned int inNumberBits)");
	}

protected:

	UInt::Handle  mNumberBits;   //!< Number of bits in string.
	Float::Handle mBitOneProba;  //!< Probability that a bit is initialized to one.

};

}
}

#endif // Beagle_GA_InitBitStrOp_hpp
