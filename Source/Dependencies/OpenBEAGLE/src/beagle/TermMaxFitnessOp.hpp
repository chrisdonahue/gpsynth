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
 *  \file   beagle/TermMaxFitnessOp.hpp
 *  \brief  Definition of the class TermMaxFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:13 $
 */

#ifndef Beagle_TermMaxFitnessOp_hpp
#define Beagle_TermMaxFitnessOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/Float.hpp"
#include "beagle/Context.hpp"
#include "beagle/System.hpp"


namespace Beagle
{

/*!
 *  \class TermMaxFitnessOp beagle/TermMaxFitnessOp.hpp "beagle/TermMaxFitnessOp.hpp"
 *  \brief Maximum fitness value termination criterion operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class TermMaxFitnessOp : public TerminationOp
{

public:

	//! TermMaxFitnessOp allocator type
	typedef AllocatorT<TermMaxFitnessOp,TerminationOp::Alloc> Alloc;
	//! TermMaxFitnessOp handle type.
	typedef PointerT<TermMaxFitnessOp,TerminationOp::Handle> Handle;
	//! TermMaxFitnessOp bag type.
	typedef ContainerT<TermMaxFitnessOp,TerminationOp::Bag> Bag;

	explicit TermMaxFitnessOp(float inMaxFitness=1.0, std::string inName="TermMaxFitnessOp");
	virtual ~TermMaxFitnessOp()
	{ }

	virtual void registerParams(System& ioSystem);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual bool terminate(const Deme& inDeme, Context& ioContext);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

	Float::Handle mMaxFitness;        //!< Maximum fitness of an individual for an evolution.
	float         mMaxFitnessDefault; //!< Default maximum fitness value.

};

}

#endif // Beagle_TerminationCheckOp_hpp
