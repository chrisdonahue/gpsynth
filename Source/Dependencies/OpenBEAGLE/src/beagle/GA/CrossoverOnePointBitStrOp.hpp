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
 *  \file   beagle/GA/CrossoverOnePointBitStrOp.hpp
 *  \brief  Definition of the class GA::CrossoverOnePointBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverOnePointBitStrOp_hpp
#define Beagle_GA_CrossoverOnePointBitStrOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverOnePointOpT.hpp"
#include "beagle/GA/BitString.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverOnePointBitStrOp beagle/GA/CrossoverOnePointBitStrOp.hpp
 *    "beagle/GA/CrossoverOnePointBitStrOp.hpp"
 *  \brief Bit string GA one point crossover operator class.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class CrossoverOnePointBitStrOp : public GA::CrossoverOnePointOpT<GA::BitString>
{

public:

	//! GA::CrossoverOnePointBitStrOp allocator type.
	typedef AllocatorT<CrossoverOnePointBitStrOp,GA::CrossoverOnePointOpT<GA::BitString>::Alloc>
	Alloc;
	//! GA::CrossoverOnePointBitStrOp handle type.
	typedef PointerT<CrossoverOnePointBitStrOp,GA::CrossoverOnePointOpT<GA::BitString>::Handle>
	Handle;
	//! GA::CrossoverOnePointBitStrOp bag type.
	typedef ContainerT<CrossoverOnePointBitStrOp,GA::CrossoverOnePointOpT<GA::BitString>::Bag>
	Bag;

	/*!
	 *  \brief Construct bit string GA one point crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the bit string GA one point crossover operator.
	 */
	explicit CrossoverOnePointBitStrOp(std::string inMatingPbName="ga.cx1p.prob",
	                                   std::string inName="GA-CrossoverOnePointBitStrOp") :
			GA::CrossoverOnePointOpT<GA::BitString>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverOnePointBitStrOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverOnePointBitStrOp_hpp
