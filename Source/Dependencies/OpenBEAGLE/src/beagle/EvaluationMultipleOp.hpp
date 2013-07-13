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
 *  \file   beagle/EvaluationMultipleOp.hpp
 *  \brief  Definition of the class EvaluationMultipleOp.
 *  \author Matthew Walker
 *  $Revision: 1.14 $
 *  $Date: 2007/08/17 18:09:12 $
 */

#ifndef Beagle_EvaluationMultipleOp_hpp
#define Beagle_EvaluationMultipleOp_hpp

#include <string>
#include <iostream>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"
#include "beagle/System.hpp"
#include "beagle/Context.hpp"
#include "beagle/Logger.hpp"
#include "beagle/BreederOp.hpp"


namespace Beagle
{

/*!
 *  \class EvaluationMultipleOp beagle/EvaluationMultipleOp.hpp "beagle/EvaluationMultipleOp.hpp"
 *  \brief Multiple individual evaluation operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup FitStats
 */
class EvaluationMultipleOp : public EvaluationOp
{

public:

	//! EvaluationMultipleOp allocator type.
	typedef AbstractAllocT<EvaluationMultipleOp,EvaluationOp::Alloc>
	Alloc;
	//! EvaluationMultipleOp handle type.
	typedef PointerT<EvaluationMultipleOp,EvaluationOp::Handle>
	Handle;
	//! EvaluationMultipleOp bag type.
	typedef ContainerT<EvaluationMultipleOp,EvaluationOp::Bag>
	Bag;

	explicit EvaluationMultipleOp(unsigned int inIndisPerCaseAndGroup=0,
	                              std::string inName="EvaluationMultipleOp");
	explicit EvaluationMultipleOp(unsigned int inIndisPerCase,
	                              unsigned int inIndisPerGroup,
	                              std::string inName="EvaluationMultipleOp");
	virtual ~EvaluationMultipleOp()
	{ }

	/*!
	 *  \brief Combine two fitnesses into one.
	 *  \param ioFitness1 First fitness to combine and result from the combination.
	 *  \param inFitness2 Second fitness to combine.
	 */
	virtual void combineFitnesses(Fitness::Handle ioFitness1,
	                              const Fitness::Handle inFitness2) const =0;

	/*!
	 *  \brief Evaluate the fitness of a set of individuals.
	 *  \param inIndividuals The individuals to evaluate.
	 *  \param ioContexts Evolutionary contexts.
	 *  \return Handle to the set of fitness values of the individuals.
	 *
	 *  The index of the individual will be the same as the index for
	 *  the individual's fitness.
	 */
	virtual Fitness::Bag::Handle evaluateCase(Individual::Bag& inIndividuals,
	        Context::Bag& ioContexts) =0;

	/*!
	 *  \brief Set number of individual per evaluation cases.
	 *  \param inIndisPerCase Number of individual per evaluation cases.
	 */
	inline void setIndisPerCase(unsigned int inIndisPerCase)
	{
		Beagle_StackTraceBeginM();
		mIndisPerCase=inIndisPerCase;
		Beagle_StackTraceEndM("void EvaluationMultipleOp::setIndisPerCase(unsigned int inIndisPerCase)");
	}

	/*!
	 *  \brief Set number of individual per groups.
	 *  \param inIndisPerGroup Number of individual per evaluation groups.
	 */
	inline void setIndisPerGroup(unsigned int inIndisPerGroup)
	{
		Beagle_StackTraceBeginM();
		mIndisPerGroup=inIndisPerGroup;
		Beagle_StackTraceEndM("void EvaluationMultipleOp::setIndisPerGroup(unsigned int inIndisPerGroup)");
	}

	/*!
	 *  \return Number of individual per evaluation cases.
	 */
	inline unsigned int getIndisPerCase() const
	{
		Beagle_StackTraceBeginM();
		return mIndisPerCase;
		Beagle_StackTraceEndM("unsigned int EvaluationMultipleOp::getIndisPerCase() const");
	}

	/*!
	 *  \return Number of individual per groups.
	 */
	inline unsigned int getIndisPerGroup() const
	{
		Beagle_StackTraceBeginM();
		return mIndisPerGroup;
		Beagle_StackTraceEndM("unsigned int EvaluationMultipleOp::getIndisPerGroup() const");
	}

	virtual Individual::Handle   breed(Individual::Bag& inBreedingPool,
	                                   BreederNode::Handle inChild,
	                                   Context& ioContext);
	virtual Fitness::Handle      evaluate(Individual& inIndividual, Context& ioContext);
	virtual Fitness::Bag::Handle evaluateIndividuals(Individual::Bag& ioIndividuals, Context::Bag& ioContexts);
	virtual void                 operate(Deme& ioDeme, Context& ioContext);


protected:

struct Case : public Beagle::Object
	{
		//! Case allocator type.
		typedef AbstractAllocT<Case,Object::Alloc>
		Alloc;
		//! Case handle type.
		typedef PointerT<Case,Object::Handle>
		Handle;
		//! Case bag type.
		typedef ContainerT<Case,Object::Bag>
		Bag;

		virtual const std::string& getName()
		{
			const static std::string lName("Case");
			return lName;
		}

		std::vector<unsigned int> mIndices;
	};

	virtual unsigned int      enlargeGroup(Individual::Bag& ioIndividuals, Context::Bag& ioContexts);
	virtual Case::Bag::Handle pruneIgnorableCases(unsigned int inNumToIgnore);
	virtual void              setupCases(unsigned int inSize, Context& ioContext);
	virtual void              setupCaseRecursive(unsigned int inSize,
	        unsigned int inLastIndex,
	        Case::Handle inCase);

	Case::Bag::Handle mCases;           //!< Evaluation cases.
	unsigned int      mIndisPerCase;    //!< Number of individual per cases.
	unsigned int      mIndisPerGroup;   //!< Number of individual per groups.

};

} // end of Beagle namespace


#endif // Beagle_EvaluationMultipleOp_hpp
