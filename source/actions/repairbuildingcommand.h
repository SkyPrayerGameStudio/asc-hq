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


#ifndef RepairBuildingCommandH
#define RepairBuildingCommandH

#include "containercommand.h"

#include "../typen.h"
#include "../objects.h"
#include "../mapfield.h"


class RepairBuildingCommand : public ContainerCommand {
   public:
      static bool avail ( const Building* building );
   private:
      
      RepairBuildingCommand( GameMap* map ) : ContainerCommand( map ) {};
      template<class Child> friend GameAction* GameActionCreator( GameMap* map);
      
      
   protected:
      void readData ( tnstream& stream );
      void writeData ( tnstream& stream ) const;
      
      GameActionID getID() const;
      ASCString getDescription() const;
      
   public:
      struct RepairData {
         int newDamage;
         int damageDelta;
         Resources cost; 
      };
      
      RepairData getCost();
      
      RepairBuildingCommand ( Building* building );
      
      ActionResult go ( const Context& context ); 
      ASCString getCommandString() const;
};

#endif

