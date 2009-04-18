/***************************************************************************
                          cargodialog.cpp  -  description
                             -------------------
    begin                : Tue Oct 24 2000
    copyright            : (C) 2000 by Martin Bickel
    email                : bickel@asc-hq.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "containercontrols.h"
#include "gamemap.h"
#include "replay.h"
#include "mapdisplayinterface.h"
#include "itemrepository.h"


const GameMap* ContainerConstControls::getMap() const
{
   return container->getMap();
}


GameMap* ContainerControls::getMap()
{
   return container->getMap();
}

const Player& ContainerConstControls::getPlayer() const
{
   return getMap()->player[ getMap()->actplayer ];
}


Player& ContainerControls::getPlayer()
{
   return getMap()->player[ getMap()->actplayer ];
}

int ContainerControls::getPlayerNum()
{
   return getMap()->actplayer;
}


bool ContainerConstControls::unitProductionAvailable() const
{
   if ( container->getOwner() == container->getMap()->actplayer )
      if ( container->vehiclesLoaded() < container->baseType->maxLoadableUnits )
         if ( container->baseType->hasFunction( ContainerBaseType::InternalVehicleProduction ))
           return true;

   return false;
}

int  ContainerConstControls::unitProductionPrerequisites( const Vehicletype* type, bool internally ) const
{
   int l = 0;
   Resources cost = container->getProductionCost( type );
   for ( int r = 0; r < resourceTypeNum; r++ )
      if ( container->getAvailableResource( cost.resource(r), r ) < cost.resource(r) )
         l |= 1 << r;
   
   if ( internally ) {
      if ( !getPlayer().research.vehicletypeavailable ( type ) && getMap()->getgameparameter( cgp_produceOnlyResearchedStuffInternally ) ) 
         l |= 1 << 10;
   
      if ( !container->vehicleUnloadable( type ) && !container->baseType->hasFunction( ContainerBaseType::ProduceNonLeavableUnits ))
         l |= 1 << 11;
   } else {
      if ( !getPlayer().research.vehicletypeavailable ( type ) && getMap()->getgameparameter( cgp_produceOnlyResearchedStuffExternally ) ) 
         l |= 1 << 10;
   }
      
   return l;
}



Vehicle* ContainerControls::produceUnitHypothetically( const Vehicletype* type )
{
   if ( !unitProductionAvailable() )
      return NULL;
   
   if ( unitProductionPrerequisites( type, true ))
      return NULL;

   
   Vehicle* vehicle = new Vehicle ( type, getMap(), getPlayerNum() );
   vehicle->setnewposition( container->getPosition() );

   if ( getMap()->getgameparameter(cgp_bi3_training) >= 1 ) {
      int cnt = 0;

      for ( Player::BuildingList::iterator bi = getMap()->player[getMap()->actplayer].buildingList.begin(); bi != getMap()->player[getMap()->actplayer].buildingList.end(); bi++ )
         if ( (*bi)->typ->hasFunction( ContainerBaseType::TrainingCenter  ) )
            cnt++;

      vehicle->experience += cnt * getMap()->getgameparameter(cgp_bi3_training);
      if ( vehicle->experience > maxunitexperience )
         vehicle->experience = maxunitexperience;
   }
   return vehicle;
}




int  ContainerControls :: ammoProducable ( int weaptype, int num )
{
   if ( container->baseType->hasFunction( ContainerBaseType::AmmoProduction ) ) {
      Resources needed = Resources( ammoProductionCost[weaptype][0] * num, ammoProductionCost[weaptype][1] * num, ammoProductionCost[weaptype][2] * num );
      Resources avail = container->getResource( needed, true );

      int perc = 100;
      for ( int r = 0; r < 3; ++r )
         if ( needed.resource(r) > 0 )
          perc = min ( 100 * avail.resource(r) / needed.resource(r), perc);

      if ( perc < 0 )
         perc = 0;

      return num * perc / 100;
   } else {
      return 0;
   }
}


int ContainerControls ::  produceAmmo ( int weaptype, int num )
{
   int n = ammoProducable( weaptype, num );
   container->getResource ( Resources( ammoProductionCost[weaptype][0] * n, ammoProductionCost[weaptype][1] * n, ammoProductionCost[weaptype][2] * n ), false );
   container->putAmmo ( weaptype, n, false );

   logtoreplayinfo ( rpl_produceAmmo, container->getPosition().x, container->getPosition().y, weaptype, n );
   return n;
}



int    ContainerControls :: getammunition ( int weapontype, int num, bool abbuchen, bool produceifrequired )
{
   int got = container->getAmmo( weapontype, num, !abbuchen );
   if ( got < num && container->baseType->hasFunction( ContainerBaseType::AmmoProduction ) && produceifrequired ) {
      if ( abbuchen ) {
         produceAmmo ( weapontype, num - got );
         got += container->getAmmo( weapontype, num - got, !abbuchen );
      } else
         got += ammoProducable( weapontype, num - got );
   }
   return got;
}

bool   ContainerControls :: ammotypeavail ( int type )
{
   return true;
}



void  ContainerControls :: fillResource (Vehicle* eht, int resourcetype, int newamount)
{
   int oldamount = eht->getResource(maxint, resourcetype, true);
   int storable = eht->putResource(newamount - oldamount, resourcetype, true);

   eht->putResource( container->getResource ( storable, resourcetype, false ), resourcetype, false );

   logtoreplayinfo ( rpl_refuel2, eht->xpos, eht->ypos, eht->networkid, int(1000+resourcetype), eht->getTank().resource(resourcetype), oldamount );
   logtoreplayinfo ( rpl_refuel3, container->getIdentification(), int(1000+resourcetype), eht->getTank().resource(resourcetype) - oldamount );
};



void  ContainerControls :: fillAmmo (Vehicle* eht, int weapon, int newa )
{
   if ( eht->typ->weapons.weapon[ weapon ].requiresAmmo() ) {
      if ( newa > eht->typ->weapons.weapon[ weapon ].count )
         newa = eht->typ->weapons.weapon[ weapon ].count;

      if ( newa > eht->ammo[weapon] )
         eht->ammo[weapon]  +=  getammunition ( eht->typ->weapons.weapon[ weapon ].getScalarWeaponType() , newa - eht->ammo[weapon], 1, true ); // CGameOptions::Instance()->container.autoproduceammunition
      else {
         container->putAmmo ( eht->typ->weapons.weapon[ weapon ].getScalarWeaponType() , eht->ammo[weapon]  - newa, 1 );
         eht->ammo[weapon] = newa;
      }
   }
   logtoreplayinfo ( rpl_refuel, eht->xpos, eht->ypos, eht->networkid, weapon, newa );

};



void ContainerControls :: refillAmmo ( Vehicle* eht )
{
   for (int i = 0; i < eht->typ->weapons.count; i++)
      if ( eht->typ->weapons.weapon[ i ].requiresAmmo() )
         fillAmmo ( eht, i, maxint );
}

void ContainerControls :: refillResources ( Vehicle* veh )
{
   fillResource ( veh, 1, maxint );
   fillResource ( veh, 2, maxint );
}

void  ContainerControls :: refilleverything ( Vehicle* eht )
{
   refillResources( eht );
   refillAmmo( eht );
}

void  ContainerControls :: emptyeverything ( Vehicle* eht )
{
   fillResource ( eht, 1, 0 );
   fillResource ( eht, 2, 0 );
   for (int i = 0; i < eht->typ->weapons.count; i++)
      if ( eht->typ->weapons.weapon[ i ].requiresAmmo() )
         fillAmmo ( eht, i, 0 );
/*
   for ( int i = 0; i < 32; i++ )
   if ( eht->loading[i] )
   emptyeverything ( eht->loading[i] );
*/         
}


