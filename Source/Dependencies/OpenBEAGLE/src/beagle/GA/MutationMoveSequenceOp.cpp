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
 *  \file   beagle/GA/src/MutationMoveSequenceOp.cpp
 *  \brief  Source code of class GA::MutationMoveSequenceOp.
 *  \author Francois-Michel De Rainville
 *  $Revision: $
 *  $Date: $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GA move sequence mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::MutationMoveSequenceOp::MutationMoveSequenceOp(std::string inMutationPbName,
												   std::string inName) :
Beagle::MutationOp(inMutationPbName, inName)
{ }


/*!
 *  \brief Register the parameters of the GA move sequence mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationMoveSequenceOp::registerParams(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	{
		Register::Description lDescription(
										   "Individual reverse sequence mutation prob.",
										   "Double",
										   "0.1",
										   "Reverse sequence mutation probability for each GA individual."
										   );
		mMutationProba = castHandleT<Double>(
											ioSystem.getRegister().insertEntry(mMutationPbName, new Double(0.1), lDescription));
	}
	Beagle::MutationOp::registerParams(ioSystem);
	Beagle_StackTraceEndM("void GA::MutationMoveSequenceOp::registerParams(System&)");
}


/*!
 *  \brief Mutate by moving a sequence of the genotype of a GA individual.
 *  \param ioIndividual GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationMoveSequenceOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
	Beagle_StackTraceBeginM();
	
	bool lMutated = false;
	
	for(unsigned int i=0; i<ioIndividual.size(); i++) {
		Beagle_LogVerboseM(
						   ioContext.getSystem().getLogger(),
						   "mutation", "Beagle::GA::MutationMoveSequenceOp",
						   std::string("Reversing a sequence of the ")+uint2ordinal(i+1)+" individual's genotype"
						   );
		Beagle_LogDebugM(
						 ioContext.getSystem().getLogger(),
						 "mutation", "Beagle::GA::MutationMoveSequenceOp",
						 "The individual's genotype before GA reverse sequence mutation"
						 );
		Beagle_LogObjectDebugM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GA::MutationMoveSequenceOp",
							   *ioIndividual[i]
							   );
		
		unsigned int a = ioContext.getSystem().getRandomizer().rollInteger(0, ioIndividual[i]->getSize()-1);
		unsigned int b = ioContext.getSystem().getRandomizer().rollInteger(0, ioIndividual[i]->getSize()-1);
		if(a > b) std::swap(a, b);
		unsigned int c = ioContext.getSystem().getRandomizer().rollInteger(0, ioIndividual[i]->getSize() - (b - a) - 1);
		//If c > a then rotate X X X A Y Y Y B Z Z C X X X --> X X X Z Z C A Y Y Y B X X X
		//ElseIf c < a then rotate X X X C Y Y Y A Z Z B X X X --> X X X A Z Z B C Y Y Y X X X
		//Else c == a then no mutation occurs since A Z Z B rotate wth itself
		if(c > a){
			lMutated = true;
			unsigned int lFirst = a, lMiddle = b + 1, lLast = b - a + c + 1, lNext = lMiddle;
			while(lFirst != lNext){
				ioIndividual[i]->swap(lFirst++, lNext++);
				if(lNext == lLast) lNext = lMiddle;
				else if(lFirst == lMiddle) lMiddle = lNext;
			}
		}else if(c < a){
			lMutated = true;
			unsigned int lFirst = c, lMiddle = a, lLast = b + 1, lNext = lMiddle;
			while(lFirst != lNext){
				ioIndividual[i]->swap(lFirst++, lNext++);
				if(lNext == lLast) lNext = lMiddle;
				else if(lFirst == lMiddle) lMiddle = lNext;
			}
		}
		
		if(lMutated) {
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GA::MutationMoveSequenceOp",
							   std::string("The individual's genotype has been shuffle mutated")
							   );
			Beagle_LogDebugM(
							 ioContext.getSystem().getLogger(),
							 "mutation", "Beagle::GA::MutationMoveSequenceOp",
							 "The individual's genotype after GA shuffle mutation"
							 );
			Beagle_LogObjectDebugM(
								   ioContext.getSystem().getLogger(),
								   "mutation", "Beagle::GA::MutationMoveSequenceOp",
								   *ioIndividual[i]
								   );
		} else {
			Beagle_LogVerboseM(
							   ioContext.getSystem().getLogger(),
							   "mutation", "Beagle::GA::MutationMoveSequenceOp",
							   std::string("The individual's genotype has not been mutated")
							   );
		}
	}
	return lMutated;
	Beagle_StackTraceEndM("bool GA::MutationMoveSequenceOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)");
}