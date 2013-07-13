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
 *  \file   beagle/src/PackageBase.cpp
 *  \brief  Source code of class PackageBase.
 *  \author Christian Gagne
 *  $Revision: 1.8 $
 *  $Date: 2007/09/20 14:46:33 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct package with basic Open BEAGLE objects.
 */
PackageBase::PackageBase(void) :
		Package("PackageBase")
{ }


/*!
 *  \brief Configure system by adding necessary components into it.
 *  \param ioSystem System to configure.
 */
void PackageBase::configure(System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Factory& lFactory = ioSystem.getFactory();

	// Add available basic types to the factory
	lFactory.insertAllocator("Beagle::AdaptBreedingProbaOp", new AdaptBreedingProbaOp::Alloc);
	lFactory.insertAllocator("Beagle::AlgoGenerational", new AlgoGenerational::Alloc);
	lFactory.insertAllocator("Beagle::AlgoSequential", new AlgoSequential::Alloc);
	lFactory.insertAllocator("Beagle::AlgoSteadyState", new AlgoSteadyState::Alloc);
	lFactory.insertAllocator("Beagle::Bool", new Bool::Alloc);
	lFactory.insertAllocator("Beagle::BoolArray", new BoolArray::Alloc);
	lFactory.insertAllocator("Beagle::Char", new Char::Alloc);
	lFactory.insertAllocator("Beagle::Container", new Container::Alloc);
	lFactory.insertAllocator("Beagle::Context", new Context::Alloc);
	lFactory.insertAllocator("Beagle::DataSetClassification", new DataSetClassification::Alloc);
	lFactory.insertAllocator("Beagle::DataSetRegression", new DataSetRegression::Alloc);
	lFactory.insertAllocator("Beagle::DecimateOp", new DecimateOp::Alloc);
	lFactory.insertAllocator("Beagle::Deme", new Deme::Alloc);
	lFactory.insertAllocator("Beagle::Double", new Double::Alloc);
	lFactory.insertAllocator("Beagle::DoubleArray", new DoubleArray::Alloc);
	lFactory.insertAllocator("Beagle::Evolver", new Evolver::Alloc);
	lFactory.insertAllocator("Beagle::Factory", new Factory::Alloc);
	lFactory.insertAllocator("Beagle::FitnessSimple", new FitnessSimple::Alloc);
	lFactory.insertAllocator("Beagle::FitnessSimpleMin", new FitnessSimpleMin::Alloc);
	lFactory.insertAllocator("Beagle::Float", new Float::Alloc);
	lFactory.insertAllocator("Beagle::FloatArray", new FloatArray::Alloc);
	lFactory.insertAllocator("Beagle::GenerationalOp", new GenerationalOp::Alloc);
	lFactory.insertAllocator("Beagle::HallOfFame", new HallOfFame::Alloc);
	lFactory.insertAllocator("Beagle::HierarchicalFairCompetitionOp", new HierarchicalFairCompetitionOp::Alloc);
	lFactory.insertAllocator("Beagle::HistoryFlushOp", new HistoryFlushOp::Alloc);
	lFactory.insertAllocator("Beagle::History", new History::Alloc);
	lFactory.insertAllocator("Beagle::HistoryID", new HistoryID::Alloc);
	lFactory.insertAllocator("Beagle::HistoryMarkOp", new HistoryMarkOp::Alloc);
	lFactory.insertAllocator("Beagle::IfThenElseOp", new IfThenElseOp::Alloc);
	lFactory.insertAllocator("Beagle::Individual", new Individual::Alloc);
	lFactory.insertAllocator("Beagle::IndividualSizeFrequencyStatsOp", new IndividualSizeFrequencyStatsOp::Alloc);
	lFactory.insertAllocator("Beagle::Int", new Int::Alloc);
	lFactory.insertAllocator("Beagle::IntArray", new IntArray::Alloc);
	lFactory.insertAllocator("Beagle::InvalidateFitnessOp", new InvalidateFitnessOp::Alloc);
	lFactory.insertAllocator("Beagle::LoggerXML", new LoggerXML::Alloc);
	lFactory.insertAllocator("Beagle::Long", new Long::Alloc);
	lFactory.insertAllocator("Beagle::LongArray", new LongArray::Alloc);
	lFactory.insertAllocator("Beagle::Map", new Map::Alloc);
	lFactory.insertAllocator("Beagle::Matrix", new Matrix::Alloc);
	lFactory.insertAllocator("Beagle::MigrationBuffer", new MigrationBuffer::Alloc);
	lFactory.insertAllocator("Beagle::MigrationGridOp", new MigrationGridOp::Alloc);
	lFactory.insertAllocator("Beagle::MigrationMapOp", new MigrationMapOp::Alloc);
	lFactory.insertAllocator("Beagle::MigrationRingOp", new MigrationRingOp::Alloc);
	lFactory.insertAllocator("Beagle::MilestoneWriteOp", new MilestoneWriteOp::Alloc);
	lFactory.insertAllocator("Beagle::MuCommaLambdaOp", new MuCommaLambdaOp::Alloc);
	lFactory.insertAllocator("Beagle::MuPlusLambdaOp", new MuPlusLambdaOp::Alloc);
	lFactory.insertAllocator("Beagle::OneCommaLambdaAdaptedOp", new OneCommaLambdaAdaptedOp::Alloc);
	lFactory.insertAllocator("Beagle::OversizeOp", new OversizeOp::Alloc);
	lFactory.insertAllocator("Beagle::PackageBase", new OversizeOp::Alloc);
	lFactory.insertAllocator("Beagle::QuasiRandom", new QuasiRandom::Alloc);
	lFactory.insertAllocator("Beagle::Randomizer", new Randomizer::Alloc);
	lFactory.insertAllocator("Beagle::RandomShuffleDemeOp", new RandomShuffleDemeOp::Alloc);
	lFactory.insertAllocator("Beagle::Register", new Register::Alloc);
	lFactory.insertAllocator("Beagle::RegisterReadOp", new RegisterReadOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectBestOp", new SelectBestOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectFirstOp", new SelectFirstOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectParsimonyTournOp", new SelectParsimonyTournOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectRandomOp", new SelectRandomOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectRouletteOp", new SelectRouletteOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectTournamentOp", new SelectTournamentOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectWorstOp", new SelectWorstOp::Alloc);
	lFactory.insertAllocator("Beagle::SelectWorstTournOp", new SelectWorstTournOp::Alloc);
	lFactory.insertAllocator("Beagle::Short", new Short::Alloc);
	lFactory.insertAllocator("Beagle::ShortArray", new ShortArray::Alloc);
	lFactory.insertAllocator("Beagle::StatsCalcFitnessSimpleOp", new StatsCalcFitnessSimpleOp::Alloc);
	lFactory.insertAllocator("Beagle::Stats", new Stats::Alloc);
	lFactory.insertAllocator("Beagle::SteadyStateOp", new SteadyStateOp::Alloc);
	lFactory.insertAllocator("Beagle::String", new String::Alloc);
	lFactory.insertAllocator("Beagle::System", new System::Alloc);
	lFactory.insertAllocator("Beagle::TermMaxEvalsOp", new TermMaxEvalsOp::Alloc);
	lFactory.insertAllocator("Beagle::TermMaxFitnessOp", new TermMaxFitnessOp::Alloc);
	lFactory.insertAllocator("Beagle::TermMaxGenOp", new TermMaxGenOp::Alloc);
	lFactory.insertAllocator("Beagle::TermMinFitnessOp", new TermMinFitnessOp::Alloc);
	lFactory.insertAllocator("Beagle::UChar", new UChar::Alloc);
	lFactory.insertAllocator("Beagle::UInt", new UInt::Alloc);
	lFactory.insertAllocator("Beagle::UIntArray", new UIntArray::Alloc);
	lFactory.insertAllocator("Beagle::ULong", new ULong::Alloc);
	lFactory.insertAllocator("Beagle::ULongArray", new ULongArray::Alloc);
	lFactory.insertAllocator("Beagle::UShort", new UShort::Alloc);
	lFactory.insertAllocator("Beagle::UShortArray", new UShortArray::Alloc);
	lFactory.insertAllocator("Beagle::Vector", new Vector::Alloc);
	lFactory.insertAllocator("Beagle::Vivarium", new Vivarium::Alloc);

	// Set aliases
	lFactory.aliasAllocator("Beagle::AdaptBreedingProbaOp", "AdaptBreedingProbaOp");
	lFactory.aliasAllocator("Beagle::AlgoGenerational", "AlgoGenerational");
	lFactory.aliasAllocator("Beagle::AlgoSequential", "AlgoSequential");
	lFactory.aliasAllocator("Beagle::AlgoSteadyState", "AlgoSteadyState");
	lFactory.aliasAllocator("Beagle::Bool", "Bool");
	lFactory.aliasAllocator("Beagle::BoolArray", "BoolArray");
	lFactory.aliasAllocator("Beagle::Char", "Char");
	lFactory.aliasAllocator("Beagle::Container", "Container");
	lFactory.aliasAllocator("Beagle::Context", "Context");
	lFactory.aliasAllocator("Beagle::DataSetClassification", "DataSetClassification");
	lFactory.aliasAllocator("Beagle::DataSetRegression", "DataSetRegression");
	lFactory.aliasAllocator("Beagle::DecimateOp", "DecimateOp");
	lFactory.aliasAllocator("Beagle::Deme", "Deme");
	lFactory.aliasAllocator("Beagle::Double", "Double");
	lFactory.aliasAllocator("Beagle::DoubleArray", "DoubleArray");
	lFactory.aliasAllocator("Beagle::Evolver", "Evolver");
	lFactory.aliasAllocator("Beagle::Factory", "Factory");
	lFactory.aliasAllocator("Beagle::FitnessSimple", "FitnessSimple");
	lFactory.aliasAllocator("Beagle::FitnessSimpleMin", "FitnessSimpleMin");
	lFactory.aliasAllocator("Beagle::Float", "Float");
	lFactory.aliasAllocator("Beagle::FloatArray", "FloatArray");
	lFactory.aliasAllocator("Beagle::GenerationalOp", "GenerationalOp");
	lFactory.aliasAllocator("Beagle::HallOfFame", "HallOfFame");
	lFactory.aliasAllocator("Beagle::HierarchicalFairCompetitionOp", "HierarchicalFairCompetitionOp");
	lFactory.aliasAllocator("Beagle::HistoryFlushOp", "HistoryFlushOp");
	lFactory.aliasAllocator("Beagle::History", "History");
	lFactory.aliasAllocator("Beagle::HistoryID", "HistoryID");
	lFactory.aliasAllocator("Beagle::HistoryMarkOp", "HistoryMarkOp");
	lFactory.aliasAllocator("Beagle::IfThenElseOp", "IfThenElseOp");
	lFactory.aliasAllocator("Beagle::Individual", "Individual");
	lFactory.aliasAllocator("Beagle::IndividualSizeFrequencyStatsOp", "IndividualSizeFrequencyStatsOp");
	lFactory.aliasAllocator("Beagle::Int", "Int");
	lFactory.aliasAllocator("Beagle::IntArray", "IntArray");
	lFactory.aliasAllocator("Beagle::InvalidateFitnessOp", "InvalidateFitnessOp");
	lFactory.aliasAllocator("Beagle::LoggerXML", "LoggerXML");
	lFactory.aliasAllocator("Beagle::Long", "Long");
	lFactory.aliasAllocator("Beagle::LongArray", "LongArray");
	lFactory.aliasAllocator("Beagle::Map", "Map");
	lFactory.aliasAllocator("Beagle::Matrix", "Matrix");
	lFactory.aliasAllocator("Beagle::MigrationBuffer", "MigrationBuffer");
	lFactory.aliasAllocator("Beagle::MigrationGridOp", "MigrationGridOp");
	lFactory.aliasAllocator("Beagle::MigrationMapOp", "MigrationMapOp");
	lFactory.aliasAllocator("Beagle::MigrationRingOp", "MigrationRingOp");
	lFactory.aliasAllocator("Beagle::MilestoneWriteOp", "MilestoneWriteOp");
	lFactory.aliasAllocator("Beagle::MuCommaLambdaOp", "MuCommaLambdaOp");
	lFactory.aliasAllocator("Beagle::MuPlusLambdaOp", "MuPlusLambdaOp");
	lFactory.aliasAllocator("Beagle::OneCommaLambdaAdaptedOp", "OneCommaLambdaAdaptedOp");
	lFactory.aliasAllocator("Beagle::OversizeOp", "OversizeOp");
	lFactory.aliasAllocator("Beagle::PackageBase", "PackageBase");
	lFactory.aliasAllocator("Beagle::QuasiRandom", "QuasiRandom");
	lFactory.aliasAllocator("Beagle::Randomizer", "Randomizer");
	lFactory.aliasAllocator("Beagle::RandomShuffleDemeOp", "RandomShuffleDemeOp");
	lFactory.aliasAllocator("Beagle::Register", "Register");
	lFactory.aliasAllocator("Beagle::RegisterReadOp", "RegisterReadOp");
	lFactory.aliasAllocator("Beagle::SelectBestOp", "SelectBestOp");
	lFactory.aliasAllocator("Beagle::SelectFirstOp", "SelectFirstOp");
	lFactory.aliasAllocator("Beagle::SelectParsimonyTournOp", "SelectParsimonyTournOp");
	lFactory.aliasAllocator("Beagle::SelectRandomOp", "SelectRandomOp");
	lFactory.aliasAllocator("Beagle::SelectRouletteOp", "SelectRouletteOp");
	lFactory.aliasAllocator("Beagle::SelectTournamentOp", "SelectTournamentOp");
	lFactory.aliasAllocator("Beagle::SelectWorstOp", "SelectWorstOp");
	lFactory.aliasAllocator("Beagle::SelectWorstTournOp", "SelectWorstTournOp");
	lFactory.aliasAllocator("Beagle::Short", "Short");
	lFactory.aliasAllocator("Beagle::ShortArray", "ShortArray");
	lFactory.aliasAllocator("Beagle::StatsCalcFitnessSimpleOp", "StatsCalcFitnessSimpleOp");
	lFactory.aliasAllocator("Beagle::Stats", "Stats");
	lFactory.aliasAllocator("Beagle::SteadyStateOp", "SteadyStateOp");
	lFactory.aliasAllocator("Beagle::String", "String");
	lFactory.aliasAllocator("Beagle::System", "System");
	lFactory.aliasAllocator("Beagle::TermMaxEvalsOp", "TermMaxEvalsOp");
	lFactory.aliasAllocator("Beagle::TermMaxFitnessOp", "TermMaxFitnessOp");
	lFactory.aliasAllocator("Beagle::TermMaxGenOp", "TermMaxGenOp");
	lFactory.aliasAllocator("Beagle::TermMinFitnessOp", "TermMinFitnessOp");
	lFactory.aliasAllocator("Beagle::UChar", "UChar");
	lFactory.aliasAllocator("Beagle::UInt", "UInt");
	lFactory.aliasAllocator("Beagle::UIntArray", "UIntArray");
	lFactory.aliasAllocator("Beagle::ULong", "ULong");
	lFactory.aliasAllocator("Beagle::ULongArray", "ULongArray");
	lFactory.aliasAllocator("Beagle::UShort", "UShort");
	lFactory.aliasAllocator("Beagle::UShortArray", "UShortArray");
	lFactory.aliasAllocator("Beagle::Vector", "Vector");
	lFactory.aliasAllocator("Beagle::Vivarium", "Vivarium");

	// Set some basic concept-type associations in factory
	lFactory.setConcept("Vivarium", "Vivarium");
	lFactory.setConcept("Deme", "Deme");
	lFactory.setConcept("Individual", "Individual");
	lFactory.setConcept("Fitness", "FitnessSimple");
	lFactory.setConcept("Stats", "Stats");
	lFactory.setConcept("HallOfFame", "HallOfFame");
	lFactory.setConcept("MigrationBuffer", "MigrationBuffer");
	lFactory.setConcept("Context", "Context");
	lFactory.setConcept("Factory", "Factory");
	lFactory.setConcept("Logger", "LoggerXML");
	lFactory.setConcept("Randomizer", "Randomizer");
	lFactory.setConcept("Register", "Register");
	lFactory.setConcept("System", "System");
	lFactory.setConcept("StatsCalculateOp", "StatsCalcFitnessSimpleOp");
	lFactory.setConcept("SelectionOp", "SelectTournamentOp");
	lFactory.setConcept("MigrationOp", "MigrationRingOp");
	lFactory.setConcept("TerminationOp", "TermMaxGenOp");
	lFactory.setConcept("Algorithm", "AlgoSequential");

	Beagle_StackTraceEndM("void PackageBase::configureSystem(System&)");
}


/*!
 *  \brief List packages which current package depends.
 *  \return Bag with the packages instanciation on which current package depends.
 *
 *  PackageBase doesn't depend on any other packages, it is the basic package of all the
    others.
 */
Package::Bag PackageBase::listDependencies(void)
{
	Beagle_StackTraceBeginM();
	return Package::Bag();
	Beagle_StackTraceEndM("Package::Bag PackageBase::listDependencies(void)");
}
