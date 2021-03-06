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


#ifndef UnitFieldRegistrationH
#define UnitFieldRegistrationH


#include "unitaction.h"


class UnitFieldRegistration : public UnitAction {
   public:
      enum Operation { RegisterOnField, UnregisterOnField, AddView, RemoveView, Position, Position3D, RegisterInCarrier, UnRegisterFromCarrier };
      
      UnitFieldRegistration( Vehicle* vehicle, const MapCoordinate3D& pos, Operation operation, ContainerBase* carrier = 0 );
      
      ASCString getDescription() const;
   private:
      Operation operation;
      MapCoordinate3D position;
      
      int carrierID;
      
      MapCoordinate3D previousPosition;
      
      ASCString getOpName() const;
      
      UnitFieldRegistration( GameMap* map ) : UnitAction( map ) {};
      template<class Child> friend GameAction* GameActionCreator( GameMap* map);

      
   protected:
      virtual GameActionID getID() const;
      
      virtual ActionResult runAction( const Context& context );
      virtual ActionResult undoAction( const Context& context );
      virtual ActionResult preCheck();
      virtual ActionResult postCheck();
      
      virtual void readData ( tnstream& stream );
      virtual void writeData ( tnstream& stream ) const;
      
};

#endif

