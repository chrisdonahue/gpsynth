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
 *  \file   beagle/GA/CrossoverUniformBitStrOp.hpp
 *  \brief  Definition of the class GA::CrossoverUniformBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverUniformBitStrOp_hpp
#define Beagle_GA_CrossoverUniformBitStrOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverUniformOpT.hpp"
#include "beagle/GA/BitString.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverUniformBitStrOp beagle/GA/CrossoverUniformBitStrOp.hpp
 *    "beagle/GA/CrossoverUniformBitStrOp.hpp"
 *  \brief Bit string GA uniform crossover operator class.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class CrossoverUniformBitStrOp : public GA::CrossoverUniformOpT<GA::BitString>
{

public:

	//! GA::CrossoverUniformBitStrOp allocator type.
	typedef AllocatorT<CrossoverUniformBitStrOp,GA::CrossoverUniformOpT<GA::BitString>::Alloc>
	Alloc;
	//! GA::CrossoverUniformBitStrOp handle type.
	typedef PointerT<CrossoverUniformBitStrOp,GA::CrossoverUniformOpT<GA::BitString>::Handle>
	Handle;
	//! GA::CrossoverUniformBitStrOp bag type.
	typedef ContainerT<CrossoverUniformBitStrOp,GA::CrossoverUniformOpT<GA::BitString>::Bag>
	Bag;

	/*!
	 *  \brief Construct bit string GA uniform crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inDistribPbName Name of the distribution probability parameter used in the register.
	 *  \param inName Name of the bit string GA uniform crossover operator.
	 */
	explicit CrossoverUniformBitStrOp(std::string inMatingPbName="ga.cxunif.prob",
	                                  std::string inDistribPbName="ga.cxunif.distribprob",
	                                  std::string inName="GA-CrossoverUniformBitStrOp") :
			GA::CrossoverUniformOpT<GA::BitString>(inMatingPbName, inDistribPbName, inName)
	{ }

	virtual ~CrossoverUniformBitStrOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverUniformBitStrOp_hpp
