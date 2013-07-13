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
 *  \file   beagle/GA/CrossoverTwoPointsBitStrOp.hpp
 *  \brief  Definition of the class GA::CrossoverTwoPointsBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverTwoPointsBitStrOp_hpp
#define Beagle_GA_CrossoverTwoPointsBitStrOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverTwoPointsOpT.hpp"
#include "beagle/GA/BitString.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverTwoPointsBitStrOp beagle/GA/CrossoverTwoPointsBitStrOp.hpp
 *    "beagle/GA/CrossoverTwoPointsBitStrOp.hpp"
 *  \brief Bit string GA two points crossover operator class.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class CrossoverTwoPointsBitStrOp : public GA::CrossoverTwoPointsOpT<GA::BitString>
{

public:

	//! GA::CrossoverTwoPointsBitStrOp allocator type.
	typedef AllocatorT<CrossoverTwoPointsBitStrOp,GA::CrossoverTwoPointsOpT<GA::BitString>::Alloc>
	Alloc;
	//! GA::CrossoverTwoPointsBitStrOp handle type.
	typedef PointerT<CrossoverTwoPointsBitStrOp,GA::CrossoverTwoPointsOpT<GA::BitString>::Handle>
	Handle;
	//! GA::CrossoverTwoPointsBitStrOp bag type.
	typedef ContainerT<CrossoverTwoPointsBitStrOp,GA::CrossoverTwoPointsOpT<GA::BitString>::Bag>
	Bag;

	/*!
	 *  \brief Construct bit string GA two points crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inName Name of the bit string GA two points crossover operator.
	 */
	explicit CrossoverTwoPointsBitStrOp(std::string inMatingPbName="ga.cx2p.prob",
	                                    std::string inName="GA-CrossoverTwoPointsBitStrOp") :
			GA::CrossoverTwoPointsOpT<GA::BitString>(inMatingPbName, inName)
	{ }

	virtual ~CrossoverTwoPointsBitStrOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverTwoPointsBitStrOp_hpp
