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
 *  \file   beagle/Beagle.hpp
 *  \brief  File to include to import all the basic/generic Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.53 $
 *  $Date: 2007/08/29 17:38:31 $
 */

/*!
 *  \namespace Beagle
 *  \brief     Namespace of all classes and global functions of the Open BEAGLE framework.
 *  \ingroup OOF
 *  \ingroup ECF
 */

/*!
 *  \mainpage Open BEAGLE Reference Manual
 *
 *  You can use the actual manual in different ways. If you are looking for documentation to get
 *  information on specific element of the Open BEAGLE framework, you can check in the
 *  <A HREF="annotated.html">compound list</A> for a specific class, or the
 *  <A HREF="functions.html">compound member index</A> for a specific method. If you intent to use the
 *  manual in order to get familiar with the framework, start by browsing the
 *  <A HREF="modules.html">modules</A>, especially the examples modules. The different components are
 *  there logically regrouped to facilitate assimilation of the internal framework organization.
 */

/*!
 *  \defgroup OOF Object Oriented Foundations
 *  \brief Classes and functions related to the Object Oriented (OO) foundations, the basis of
 *    the Open BEAGLE architecture.
 */

/*!
 *  \defgroup ECF Generic EC Framework
 *  \brief Generic component of the framework architecture for evolutionary computation.
 *
 *  The generic EC framework is an extension of OO foundations. It offers a solid basis for
 *  implementing evolutionary algorithms. It is composed of a generic structure for populations,
 *  an evolution system and a set of operators packed in an evolver.
 */


#ifndef Beagle_Beagle_hpp
#define Beagle_Beagle_hpp

// PACC classes

#include "PACC/Math.hpp"
#include "PACC/Util.hpp"
#include "PACC/XML.hpp"

// Object Oriented Foundation

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/NamedObject.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/castObjectT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/NamedObject.hpp"
#include "beagle/Exception.hpp"
#include "beagle/TargetedException.hpp"
#include "beagle/ValidationException.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/BadCastException.hpp"
#include "beagle/InternalException.hpp"
#include "beagle/ObjectException.hpp"
#include "beagle/IOException.hpp"
#include "beagle/RunTimeException.hpp"
#include "beagle/IsEqualPointerPredicate.hpp"
#include "beagle/IsLessPointerPredicate.hpp"
#include "beagle/IsMorePointerPredicate.hpp"
#include "beagle/IsEqualMapPairPredicate.hpp"
#include "beagle/IsLessMapPairPredicate.hpp"
#include "beagle/HashString.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Map.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/ArrayT.hpp"
#include "beagle/Bool.hpp"
#include "beagle/Char.hpp"
#include "beagle/Short.hpp"
#include "beagle/Int.hpp"
#include "beagle/Long.hpp"
#include "beagle/UChar.hpp"
#include "beagle/UShort.hpp"
#include "beagle/UInt.hpp"
#include "beagle/ULong.hpp"
#include "beagle/Float.hpp"
#include "beagle/Double.hpp"
#include "beagle/String.hpp"
#include "beagle/Matrix.hpp"
#include "beagle/Vector.hpp"

// Generic EC framework

#include "beagle/Stats.hpp"
#include "beagle/Fitness.hpp"
#include "beagle/FitnessSimple.hpp"
#include "beagle/FitnessSimpleMin.hpp"
#include "beagle/FitnessMultiObj.hpp"
#include "beagle/FitnessMultiObjMin.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualBag.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/HallOfFame.hpp"
#include "beagle/ParetoFrontHOF.hpp"
#include "beagle/ParetoFrontCalculateOp.hpp"
#include "beagle/MigrationBuffer.hpp"
#include "beagle/Register.hpp"
#include "beagle/RegisterReadOp.hpp"
#include "beagle/Logger.hpp"
#include "beagle/LoggerXML.hpp"
#include "beagle/Randomizer.hpp"
#include "beagle/Factory.hpp"
#include "beagle/Context.hpp"
#include "beagle/Component.hpp"
#include "beagle/ComponentMap.hpp"
#include "beagle/System.hpp"
#include "beagle/Operator.hpp"
#include "beagle/OperatorMap.hpp"
#include "beagle/Member.hpp"
#include "beagle/MemberMap.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"
#include "beagle/PackageBase.hpp"
#include "beagle/PackageMultiObj.hpp"
#include "beagle/Algorithm.hpp"
#include "beagle/AlgoSequential.hpp"
#include "beagle/AlgoGenerational.hpp"
#include "beagle/AlgoSteadyState.hpp"
#include "beagle/AlgoNSGA2.hpp"
#include "beagle/BreederOp.hpp"
#include "beagle/BreederNode.hpp"
#include "beagle/QuasiRandom.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/AdaptBreedingProbaOp.hpp"
#include "beagle/GenerationalOp.hpp"
#include "beagle/SteadyStateOp.hpp"
#include "beagle/MuCommaLambdaOp.hpp"
#include "beagle/MuPlusLambdaOp.hpp"
#include "beagle/OneCommaLambdaAdaptedOp.hpp"
#include "beagle/NSGA2Op.hpp"
#include "beagle/NPGA2Op.hpp"
#include "beagle/IfThenElseOp.hpp"
#include "beagle/MilestoneWriteOp.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/TermMaxGenOp.hpp"
#include "beagle/TermMaxEvalsOp.hpp"
#include "beagle/TermMaxFitnessOp.hpp"
#include "beagle/TermMinFitnessOp.hpp"
#include "beagle/StatsCalculateOp.hpp"
#include "beagle/StatsCalcFitnessMultiObjOp.hpp"
#include "beagle/StatsCalcFitnessSimpleOp.hpp"
#include "beagle/MigrationOp.hpp"
#include "beagle/MigrationMapOp.hpp"
#include "beagle/MigrationRingOp.hpp"
#include "beagle/MigrationGridOp.hpp"
#include "beagle/HierarchicalFairCompetitionOp.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/EvaluationMultipleOp.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/SelectionOp.hpp"
#include "beagle/SelectBestOp.hpp"
#include "beagle/SelectFirstOp.hpp"
#include "beagle/SelectParsimonyTournOp.hpp"
#include "beagle/SelectRandomOp.hpp"
#include "beagle/SelectRouletteOp.hpp"
#include "beagle/SelectTournamentOp.hpp"
#include "beagle/SelectWorstOp.hpp"
#include "beagle/SelectWorstTournOp.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/RecombinationOp.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/SteadyStateOp.hpp"
#include "beagle/DecimateOp.hpp"
#include "beagle/OversizeOp.hpp"
#include "beagle/RandomShuffleDemeOp.hpp"
#include "beagle/Randomizer.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/InvalidateFitnessOp.hpp"
#include "beagle/IndividualSizeFrequencyStatsOp.hpp"
#include "beagle/DataSetClassification.hpp"
#include "beagle/DataSetRegression.hpp"
#include "beagle/History.hpp"
#include "beagle/HistoryEntry.hpp"
#include "beagle/HistoryTrace.hpp"
#include "beagle/HistoryFlushOp.hpp"
#include "beagle/HistoryID.hpp"
#include "beagle/HistoryMarkOp.hpp"

#if defined(BEAGLE_USE_OMP_R) || defined(BEAGLE_USE_OMP_NR)
#include "beagle/RandomizerMulti.hpp"
#include "beagle/OpenMP.hpp"
#endif

#endif // Beagle_Beagle_hpp
