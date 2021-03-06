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


#include "unitcommand.h"

#include "../gamemap.h"

const Vehicle* UnitCommand::getUnit() const
{
   return getMap()->getUnit( unitNetworkID );
}

Vehicle* UnitCommand::getUnit() 
{
   return getMap()->getUnit( unitNetworkID );
}

UnitCommand::UnitCommand( Vehicle* vehicle )
   : Command( vehicle->getMap() ), 
     unitNetworkID ( vehicle->networkid ), 
     startingPosition( vehicle->getPosition() ),
     unitTypeID( vehicle->typ->id )
{
};

UnitCommand::UnitCommand( GameMap* map )
   : Command( map ), unitNetworkID ( -1 ), unitTypeID( -1 )
{
     
}

static const int unitCommandStreamVersion = 2;

void UnitCommand::readData ( tnstream& stream )
{
   Command::readData( stream );
   int version = stream.readInt();
   if ( version < 1 || version > unitCommandStreamVersion ) 
      throw tinvalidversion ( "UnitCommand", unitCommandStreamVersion, version );
   unitNetworkID = stream.readInt();   
   
   if ( version >= 2 ) {
      unitTypeID  = stream.readInt();
      startingPosition.read( stream );
   }
}


void UnitCommand::writeData ( tnstream& stream ) const
{
   Command::writeData( stream );
   stream.writeInt( unitCommandStreamVersion );
   stream.writeInt( unitNetworkID );
   stream.writeInt( unitTypeID );
   startingPosition.write( stream );
}

vector<MapCoordinate> UnitCommand::getCoordinates() const
{
   vector<MapCoordinate> v = Command::getCoordinates();
   if ( startingPosition.valid() )
      v.push_back( startingPosition );
   return v;
}

int UnitCommand::getUnitTypeID() const 
{ 
   if ( unitTypeID == -1 ) {
      const Vehicle* veh = getMap()->getUnit( unitNetworkID );
      if ( veh )
         return veh->typ->id;
   } 
   
   return unitTypeID; 
}; 

ActionResult UnitCommand::checkExecutionPrecondition() const
{
   if ( getMap()->actplayer != getUnit()->getOwner() )
      return ActionResult(101);
   else
      return ActionResult(0);
}
