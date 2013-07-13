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
 *  \file   beagle/OneCommaLambdaAdaptedOp.hpp
 *  \brief  Definition of the class OneCommaLambdaAdaptedOp.
 *  \author Christian Gagne
 *  $Revision: 1.7 $
 *  $Date: 2007/08/17 18:09:13 $
 */


#ifndef Beagle_OneCommaLambdaAdaptedOp_hpp
#define Beagle_OneCommaLambdaAdaptedOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/UInt.hpp"

namespace Beagle
{


/*!
 *  \class OneCommaLambdaAdaptedOp beagle/OneCommaLambdaAdaptedOp.hpp "beagle/OneCommaLambdaAdaptedOp.hpp"
 *  \brief One comma lambda with adaptation (1,lambda_adapt) replacement strategy operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  A (1,lambda_adapt) operator generates lambda children individuals from a population of one parent
 *  with an adaptation of the number of children generated following the success rate of the
 *  evolution. More specifically, the replacement strategy is a standard (1,lambda) approach where the
 *  lambda value (initially given by parameter 'ec.1cladp.lambdainit') is adapted at the end of each
 *  generation, when the number of children generated that have a fitness value higher than their
 *  parent is different from 1. If there are more than one individual with a fitness higher than the
 *  parent the population size is reduced by a factor (alpha), and conversely if there is no individual
 *  with a fitness value higher than the parent, the value of lambda is increased by the inverse of the
 *  factor (i.e. 1/alpha).
 */
class OneCommaLambdaAdaptedOp : public ReplacementStrategyOp
{

public:

	//! OneCommaLambdaAdaptedOp allocator type.
	typedef AllocatorT<OneCommaLambdaAdaptedOp,ReplacementStrategyOp::Alloc>
	Alloc;
	//! OneCommaLambdaAdaptedOp handle type.
	typedef PointerT<OneCommaLambdaAdaptedOp,ReplacementStrategyOp::Handle>
	Handle;
	//! OneCommaLambdaAdaptedOp bag type.
	typedef ContainerT<OneCommaLambdaAdaptedOp,ReplacementStrategyOp::Bag>
	Bag;

	explicit OneCommaLambdaAdaptedOp(std::string inName="OneCommaLambdaAdaptedOp");
	virtual ~OneCommaLambdaAdaptedOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void init(System& ioSystem);

protected:

	Float::Handle mLambdaFactorAdapt;        //!< Factor used to adapt lambda value.
	UInt::Handle  mLambda;                   //!< Adapted value of lambda.
	UInt::Handle  mLambdaInit;               //!< Initial lambda value.
	UInt::Handle  mMaxLambda;                //!< Maximum value of lambda.
	UInt::Handle  mMinLambda;                //!< Minimum value of lambda.

};

}


#endif // Beagle_OneCommaLambdaAdaptedOp_hpp

