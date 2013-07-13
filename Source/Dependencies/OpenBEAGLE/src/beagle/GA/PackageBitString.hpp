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
 *  \file   beagle/GA/PackageBitString.hpp
 *  \brief  Definition of the class GA::PackageBitString.
 *  \author Christian Gagne
 *  $Revision: 1.1 $
 *  $Date: 2007/08/08 14:51:33 $
 */

#ifndef Beagle_GA_PackageBitString_hpp
#define Beagle_GA_PackageBitString_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/System.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/Package.hpp"


namespace Beagle
{

namespace GA
{

/*!
 *  \class GA::PackageBitString beagle/GA/PackageBitString.hpp "beagle/GA/PackageBitString.hpp"
 *  \brief Package for adding objects related to bit string GA.
 *  \ingroup GAF
 *  \ingroup GABS
 */
class PackageBitString : public Package
{

public:

	//! GA::PackageBitString allocator type.
	typedef AllocatorT< PackageBitString, Package::Alloc >
	Alloc;
	//! GA::PackageBitString handle type.
	typedef PointerT< PackageBitString, Package::Handle >
	Handle;
	//! GA::PackageBitString bag type.
	typedef ContainerT< PackageBitString, Package::Bag >
	Bag;

	explicit PackageBitString(unsigned int inInitSize=0);

	virtual void          configure(System& ioSystem);
	virtual Package::Bag  listDependencies(void);

protected:

	unsigned int mInitSize;  //!< Size of vector to use at initialization.

};

}
}

#endif // Beagle_GA_PackageBitString_hpp
