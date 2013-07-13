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
 *  \file   beagle/StatsCalculateOp.hpp
 *  \brief  Definition of the class StatsCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_StatsCalculateOp_hpp
#define Beagle_StatsCalculateOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"
#include "beagle/UInt.hpp"

namespace Beagle
{


/*!
 *  \class StatsCalculateOp beagle/StatsCalculateOp.hpp "beagle/StatsCalculateOp.hpp"
 *  \brief Calculate statistics of a deme, for a generation, operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup FitStats
 */
class StatsCalculateOp : public Operator
{

public:

	//! StatsCalculateOp allocator type
	typedef AbstractAllocT<StatsCalculateOp,Operator::Alloc> Alloc;
	//! StatsCalculateOp handle type.
	typedef PointerT<StatsCalculateOp,Operator::Handle> Handle;
	//! StatsCalculateOp bag type.
	typedef ContainerT<StatsCalculateOp,Operator::Bag> Bag;

	explicit StatsCalculateOp(std::string inName="StatsCalculateOp");
	virtual ~StatsCalculateOp()
	{ }

	/*!
	 *  \brief  Calculate statistics of a given deme.
	 *  \param  outStats Computed stats of the deme.
	 *  \param  ioDeme Deme to evaluate the statistics.
	 *  \param  ioContext Context of the evolution.
	 *  \return Handle to the generational statistics of the given deme.
	 */
	virtual void calculateStatsDeme(Stats& outStats, Deme& ioDeme, Context& ioContext) const =0;

	virtual void registerParams(System& ioSystem);
	virtual void operate(Deme& ioDeme, Context& ioContext);
	virtual void calculateStatsVivarium(Stats& outStats,
	                                    Vivarium& ioVivarium,
	                                    Context& ioContext) const;

protected:

	UIntArray::Handle  mPopSize;              //!< Population size for the evolution.
	unsigned int       mNbDemesCalculated;    //!< Number of demes for which stats is computed.
	unsigned int       mGenerationCalculated; //!< Actual generation for which stats is computed.

};

}

#endif // Beagle_StatsCalculateOp_hpp
