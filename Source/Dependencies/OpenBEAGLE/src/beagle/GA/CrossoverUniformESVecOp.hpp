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
 *  \file   beagle/GA/CrossoverUniformESVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverUniformESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverUniformESVecOp_hpp
#define Beagle_GA_CrossoverUniformESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/GA/CrossoverUniformOpT.hpp"
#include "beagle/GA/ESVector.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverUniformESVecOp beagle/GA/CrossoverUniformESVecOp.hpp
 *    "beagle/GA/CrossoverUniformESVecOp.hpp"
 *  \brief Evolution strategy uniform crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 */
class CrossoverUniformESVecOp : public GA::CrossoverUniformOpT<GA::ESVector>
{

public:

	//! GA::CrossoverUniformESVecOp allocator type.
	typedef AllocatorT<CrossoverUniformESVecOp,GA::CrossoverUniformOpT<GA::ESVector>::Alloc>
	Alloc;
	//! GA::CrossoverUniformESVecOp handle type.
	typedef PointerT<CrossoverUniformESVecOp,GA::CrossoverUniformOpT<GA::ESVector>::Handle>
	Handle;
	//! GA::CrossoverUniformESVecOp bag type.
	typedef ContainerT<CrossoverUniformESVecOp,GA::CrossoverUniformOpT<GA::ESVector>::Bag>
	Bag;

	/*!
	 *  \brief Construct evolution strategy uniform crossover operator.
	 *  \param inMatingPbName Name of the mating probability parameter used in the register.
	 *  \param inDistribPbName Name of the distribution probability parameter used in the register.
	 *  \param inName Name of the evolution strategy uniform crossover operator.
	 */
	explicit CrossoverUniformESVecOp(std::string inMatingPbName="es.cxunif.prob",
	                                 std::string inDistribPbName="es.cxunif.distribprob",
	                                 std::string inName="GA-CrossoverUniformESVecOp") :
			GA::CrossoverUniformOpT<GA::ESVector>(inMatingPbName, inDistribPbName, inName)
	{ }

	virtual ~CrossoverUniformESVecOp()
	{ }

};

}
}

#endif // Beagle_GA_CrossoverUniformESVecOp_hpp
