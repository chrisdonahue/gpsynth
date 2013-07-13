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
 *  \file   beagle/FitnessSimple.hpp
 *  \brief  Definition of the class FitnessSimple.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_FitnessSimple_hpp
#define Beagle_FitnessSimple_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Fitness.hpp"

namespace Beagle
{

/*!
 *  \class FitnessSimple beagle/FitnessSimple.hpp "beagle/FitnessSimple.hpp"
 *  \brief Simple fitness class, made of a unique measure.
 *  \ingroup ECF
 *  \ingroup FitStats
 */
class FitnessSimple : public Fitness
{

public:

	//! FitnessSimple allocator type.
	typedef AllocatorT<FitnessSimple,Fitness::Alloc> Alloc;
	//! FitnessSimple handle type.
	typedef PointerT<FitnessSimple,Fitness::Handle> Handle;
	//! FitnessSimple bag type.
	typedef ContainerT<FitnessSimple,Fitness::Bag> Bag;

	FitnessSimple();
	explicit FitnessSimple(double inValue);
	virtual ~FitnessSimple()
	{ }

	virtual void                copy(const Member& inOriginal, System& ioSystem);
	virtual double              getDistance(const Fitness& inRightFitness) const;
	virtual unsigned int        getNumberOfObjectives() const;
	virtual double              getObjective(unsigned int inObjIndex) const;
	virtual const std::string&  getType() const;
	virtual bool                isDominated(const Fitness& inRightFitness) const;
	virtual bool                isEqual(const Object& inRightObj) const;
	virtual bool                isLess(const Object& inRightObj) const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                setValue(double inValue);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief Return fitness value.
	 *  \return Fitness value.
	 */
	inline double getValue() const
	{
		Beagle_StackTraceBeginM();
		return mValue;
		Beagle_StackTraceEndM("double FitnessSimple::getValue() const");
	}

protected:

	double mValue;  //!< Unique measure of the fitness.

};

}

#endif // Beagle_FitnessSimple_hpp

