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
 *  \file   beagle/GA/CrossoverBlendESVecOp.hpp
 *  \brief  Definition of the class GA::CrossoverBlendESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/08 19:26:34 $
 */

#ifndef Beagle_GA_CrossoverBlendESVecOp_hpp
#define Beagle_GA_CrossoverBlendESVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/CrossoverOp.hpp"

namespace Beagle
{
namespace GA
{

/*!
 *  \class CrossoverBlendESVecOp beagle/GA/CrossoverBlendESVecOp.hpp
 *    "beagle/GA/CrossoverBlendESVecOp.hpp"
 *  \brief ES blend crossover operator class.
 *  \ingroup GAF
 *  \ingroup GAES
 *
 *  Real-valued ES blend crossover (BLX-alpha) proceed by blending two ES individuals,
 *  with values \f$(x^{(1,t)},x^{(2,t)})\f$ and strategy parameters \f$(s^{(1,t)},s^{(2,t)})\f$,
 *  using a parameter \f$\alpha\in[0,1]\f$. The resulting children \f$(x^{(1,t+1)},x^{(2,t+1)})\f$
 *  are equal to \f$x^{(1,t+1)}_i=(1-\gamma^x_i) x^{(1,t)}_i+\gamma^x_i x^{(2,t)}_i\f$,
 *  \f$x^{(2,t+1)}_i=\gamma^x_i x^{(1,t)}_i+(1-\gamma^x_i) x^{(2,t)}_i\f$,
 *  \f$s^{(1,t+1)}_i=(1-\gamma^s_i) s^{(1,t)}_i+\gamma^s_i s^{(2,t)}_i\f$, and
 *  \f$s^{(2,t+1)}_i=\gamma^s_i s^{(1,t)}_i+(1-\gamma^s_i) s^{(2,t)}_i\f$,
 *  where \f$u^x_i\in[0,1]\f$ and \f$u^s_i\in[0,1]\f$ are a random value,
 *  \f$\gamma^x_i=(1 + 2\alpha)u^x_i-\alpha\f$, and \f$\gamma^s_i=(1 + 2\alpha)u^s_i-\alpha\f$ .
 *
 *  This crossover operation consists in generating two children that depends on the
 *  difference between the parents individuals.
 *
 */
class CrossoverBlendESVecOp : public CrossoverOp
{

public:

	//! GA::CrossoverBlendESVecOp allocator type.
	typedef AllocatorT<CrossoverBlendESVecOp,CrossoverOp::Alloc>
	Alloc;
	//! GA::CrossoverBlendESVecOp handle type.
	typedef PointerT<CrossoverBlendESVecOp,CrossoverOp::Handle>
	Handle;
	//! GA::CrossoverBlendESVecOp bag type.
	typedef ContainerT<CrossoverBlendESVecOp,CrossoverOp::Bag>
	Bag;

	explicit CrossoverBlendESVecOp(std::string inMatingPbName="es.cxblend.prob",
	                               std::string inName="GA-CrossoverBlendESVecOp");
	virtual ~CrossoverBlendESVecOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual bool mate(Individual& ioIndiv1, Context& ioContext1,
	                  Individual& ioIndiv2, Context& ioContext2);

protected:

	DoubleArray::Handle  mMaxValue;      //!< Maximum of ES vector values.
	DoubleArray::Handle  mMinValue;      //!< Minimum of ES vector values.
	Double::Handle       mAlpha;         //!< Blend crossover alpha parameter.

};

}
}


#endif // Beagle_GA_CrossoverBlendESVecOp_hpp
