/*
     This file is part of Advanced Strategic Command; http://www.asc-hq.de
     Copyright (C) 1994-2010  Martin Bickel  and  Marc Schellenberger
 
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.
 
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
 
     You should have received a copy of the GNU General Public License
     along with this program; see the file COPYING. If not, write to the 
     Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
     Boston, MA  02111-1307  USA
*/

#ifndef packageH
#define packageH

#include "versionidentifier.h"
#include "ascstring.h"
#include "basestreaminterface.h"
#include "typen.h"


class Package {
   public:
       
      class PackageDependency {
         public:
            ASCString name;
            VersionIdentifier version; 
            void read ( tnstream& stream );
            void write ( tnstream& stream ) const;
      };
      
      ASCString name;
      ASCString description;
      ASCString updateUrl;
      
      VersionIdentifier version;
      
      //! the given packages must be present to use this package
      typedef vector<PackageDependency> Dependencies;
      Dependencies dependencies;
      
      //! the archive in which the package is stored
      ASCString archive;
      
      ASCString location;
      
      void read ( tnstream& stream );
      void write ( tnstream& stream ) const;
   
      void runTextIO ( PropertyContainer& pc );
};



#endif
