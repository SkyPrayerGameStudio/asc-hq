/***************************************************************************
                          service.cpp  -  description
                             -------------------
    begin                : Fri Mar 30 2001
    copyright            : (C) 2001 by Martin Bickel
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

#include "ai_common.h"



AI :: ServiceOrder :: ServiceOrder ( AI* _ai, VehicleService::Service _requiredService, int UnitID, int _pos )
{
   failure = 0;
   ai = _ai;
   targetUnitID = UnitID;
   requiredService = _requiredService;
   serviceUnitID = 0;
   position = _pos;
   time = ai->getMap()->time;

   nextServiceBuilding = ai->getMap()->getField( ai->findServiceBuilding ( *this, &nextServiceBuildingDistance ))->building;
}

AI :: ServiceOrder :: ServiceOrder ( AI* _ai, tnstream& stream )
{
  failure = 0;
  ai = _ai;
  read ( stream );
}

const int currentServiceOrderVersion = 10001;

void AI :: ServiceOrder :: read ( tnstream& stream )
{
  int version = stream.readInt( );
  if ( version > currentServiceOrderVersion )
     throw tinvalidversion ( "AI :: ServiceOrder", currentServiceOrderVersion, version );
  targetUnitID = stream.readInt (  );
  serviceUnitID = stream.readInt (  );
  position = stream.readInt (  );
  time.abstime = stream.readInt (  );
  if ( version >= 10001 ) {
     int i= stream.readInt();
     if ( i )  {
        MapCoordinate mc;
        mc.read ( stream );
        nextServiceBuilding = ai->getMap()->getField( mc )->building;
        nextServiceBuildingDistance = stream.readInt();
     } else
        nextServiceBuilding = NULL;
  }
}

void AI :: ServiceOrder :: write ( tnstream& stream ) const
{
  stream.writeInt( currentServiceOrderVersion );
  stream.writeInt ( targetUnitID );
  stream.writeInt ( serviceUnitID );
  stream.writeInt ( position );
  stream.writeInt ( time.abstime );
  if ( nextServiceBuilding ) {
     stream.writeInt ( 1 );
     nextServiceBuilding->getEntry().write ( stream );
     stream.writeInt ( nextServiceBuildingDistance );
  } else {
     stream.writeInt ( 0 );
  }
}

bool AI :: ServiceOrder :: completelyFailed ()
{
  return failure > 2 ;
}

AI :: ServiceOrder :: ~ServiceOrder (  )
{
   pvehicle serv = getServiceUnit();
   if ( serv )
      serv->aiparam[ai->getPlayerNum()]->resetTask();
}

void AI :: issueServices ( )
{
   serviceOrders.erase ( remove_if ( serviceOrders.begin(), serviceOrders.end(), ServiceOrder::targetDestroyed ), serviceOrders.end());

   for ( Player::VehicleList::iterator vi = getPlayer().vehicleList.begin(); vi != getPlayer().vehicleList.end(); vi++ ) {
      pvehicle veh = *vi;
      if ( veh->damage > config.damageLimit )
         serviceOrders.push_back ( ServiceOrder ( this, VehicleService::srv_repair, veh->networkid ) );

      for ( int i = 0; i< resourceTypeNum; i++ )
         if ( veh->maxMovement() ) // stationary units are ignored
            if ( veh->tank.resource(i) < veh->typ->tank.resource(i) * config.resourceLimit.resource(i) / 100 )
               serviceOrders.push_back ( ServiceOrder ( this, VehicleService::srv_resource, veh->networkid, i));

      for ( int w = 0; w< veh->typ->weapons->count; w++ )
         if ( veh->typ->weapons->weapon[w].count )
            if ( veh->ammo[w] <= veh->typ->weapons->weapon[w].count * config.ammoLimit / 100 )
               serviceOrders.push_back ( ServiceOrder ( this, VehicleService::srv_ammo, veh->networkid, w));

   }
}


MapCoordinate3D AI :: findServiceBuilding ( const ServiceOrder& so, int* distance )
{
   pvehicle veh = so.getTargetUnit();

   AStar3D astar ( getMap(), veh );
   astar.findAllAccessibleFields (  );

   pbuilding bestBuilding = NULL;
   int bestDistance = maxint;
   MapCoordinate3D bestPos;

   pbuilding bestBuilding_p = NULL;
   int bestDistance_p = maxint;
   MapCoordinate3D bestPos_p;

   for ( Player::BuildingList::iterator bi = getPlayer().buildingList.begin(); bi != getPlayer().buildingList.end(); bi++ ) {
      pbuilding bld = *bi;
      if ( bld->getEntryField()->a.temp & bld->typ->loadcapability ) {
         MapCoordinate3D buildingPos = bld->getEntry();
         if ( !(bld->typ->loadcapability & buildingPos.z))
            buildingPos.z = 1 << log2 ( bld->getEntryField()->a.temp & bld->typ->loadcapability );

         // the unit can reach the building

         /*
         the new 3D-Astar makes this obsolete
         bool loadable = false;
         if ( bld->vehicleloadable ( veh ))
            loadable = true;
         else
            for ( int i = 0; i < 8; i++ )
                if ( veh->typ->height & ( 1 << i))
                   if ( bld->vehicleloadable ( veh, 1 << i ))
                      loadable = true;
         */
         bool loadable = true;
         if ( loadable ) {
            int fullfillableServices = 0;
            int partlyFullfillabelServices = 0;
            switch ( so.requiredService ) {
               case VehicleService::srv_repair : if ( bld->canRepair() ) {
                                                    int mr =  bld->getMaxRepair( veh );
                                                    if ( mr == 0 )
                                                       fullfillableServices++;

                                                    if ( mr < veh->damage )
                                                       partlyFullfillabelServices++;

                                                 }
                                                 break;
               case VehicleService::srv_resource:  {
                                                     Resources needed =  veh->typ->tank - veh->tank;
                                                     Resources avail = bld->getResource ( needed, 1 );
                                                     if ( avail < needed )
                                                        avail += bld->netResourcePlus( ) * config.waitForResourcePlus;

                                                     int missing = 0;
                                                     int pmissing = 0;
                                                     for ( int r = 0; r < resourceTypeNum; r++ ) {
                                                        if( needed.resource(r) * 75 / 100 > avail.resource(r) )
                                                           missing ++;

                                                        if( needed.resource(r) * 10 / 100 > avail.resource(r) )
                                                           pmissing ++;
                                                     }
                                                     if ( missing == 0 )
                                                        fullfillableServices++;

                                                     if ( pmissing == 0)
                                                       partlyFullfillabelServices++;
                                                 }
                                                 break;
               case VehicleService::srv_ammo :  {
                                                   int missing = 0;
                                                   int pmissing = 0;
                                                   int ammoNeeded[waffenanzahl];
                                                   for ( int t = 0; t < waffenanzahl; t++ )
                                                      ammoNeeded[t] = 0;

                                                   for ( int i = 0; i < veh->typ->weapons->count; i++ )
                                                      if ( veh->typ->weapons->weapon[i].requiresAmmo() )
                                                         ammoNeeded[ veh->typ->weapons->weapon[i].getScalarWeaponType() ] += veh->typ->weapons->weapon[i].count - veh->ammo[i];

                                                   Resources needed;
                                                   for ( int  j = 0; j < waffenanzahl; j++ ) {
                                                       int n = ammoNeeded[j] - bld->ammo[j];
                                                       if ( n > 0 )
                                                          if ( bld->typ->special & cgammunitionproductionb ) {
                                                             for ( int r = 0; r < resourceTypeNum; r++ )
                                                                needed.resource(r) += (n+4)/5 * cwaffenproduktionskosten[j][r];
                                                          } else
                                                             missing++;
                                                   }
                                                   Resources avail = bld->getResource ( needed, 1 );
                                                   if ( avail < needed )
                                                      avail += bld->netResourcePlus( ) * config.waitForResourcePlus;

                                                   for ( int r = 0; r < resourceTypeNum; r++ ) {
                                                      if ( avail.resource(r) < needed.resource (r) )
                                                         missing++;
                                                      if ( avail.resource(r) <= needed.resource (r)/3 )
                                                         pmissing++;
                                                   }

                                                   if ( missing == 0 )
                                                       fullfillableServices++;

                                                   if ( pmissing == 0)
                                                     partlyFullfillabelServices++;

                                                 }
                                                 break;

            };

            if ( fullfillableServices ) {
               int dist = astar.fieldVisited(buildingPos)->gval;
               if ( dist < bestDistance ) {
                  bestDistance = dist;
                  bestBuilding = bld;
                  bestPos = buildingPos;
               }
            } else
               if ( partlyFullfillabelServices ) {
                  int dist = astar.fieldVisited(buildingPos)->gval;
                  if ( dist < bestDistance_p ) {
                     bestDistance_p = dist;
                     bestBuilding_p = bld;
                     bestPos_p = buildingPos;
                  }
               }
         }
      }
   }

   if ( bestBuilding && (bestDistance < bestDistance_p*3)) {
      if ( distance )
         *distance = bestDistance * maxmalq;
      return bestPos;
   } else {
      if ( distance )
         *distance = bestDistance_p * maxmalq;
      return bestPos_p;
   }
}

int AI::ServiceOrder::possible ( pvehicle supplier )
{
   bool result = false;

   VehicleService vs ( NULL, NULL);
   vs.fieldSearch.bypassChecks.distance = true;
   vs.fieldSearch.bypassChecks.height   = true;
   vs.fieldSearch.init ( supplier, NULL );
   vs.fieldSearch.checkVehicle2Vehicle ( getTargetUnit(), getTargetUnit()->xpos, getTargetUnit()->ypos );

   VehicleService::TargetContainer::iterator i = vs.dest.find ( targetUnitID );
   if ( i != vs.dest.end() ) {
      VehicleService::Target target = i->second;
      int serviceAmount = 0;
      for ( int j = 0; j < target.service.size(); j++ ) {
         serviceAmount += target.service[j].maxPercentage;
         if ( target.service[j].type == requiredService ) {
            bool enoughResources = false;
            if ( requiredService == VehicleService::srv_repair ) {
                if ( target.service[j].minAmount <  target.service[j].curAmount - 5 )
                  enoughResources = true;
            } else
               if ( target.service[j].targetPos == position ) {  // is this stuff the one we need
                  int requiredAmount = target.service[j].maxAmount - target.service[j].curAmount;
                  if ( requiredAmount > 0 ) {
                     if ( requiredService == VehicleService::srv_resource ) {
                        if ( target.service[j].targetPos == Resources::Fuel ) { // fuel  ; 30 fields requiredForHome
                            if ( requiredAmount < target.service[j].orgSourceAmount - supplier->typ->fuelConsumption * 30 )
                               enoughResources = true;
                        } else {
                            if ( getTargetUnit()->aiparam[ai->getPlayerNum()]->job == AiParameter::job_supply ) {
                               if ( target.service[j].orgSourceAmount > requiredAmount )
                                  enoughResources = true;
                            } else
                               if ( target.service[j].maxPercentage > 80 )
                                  enoughResources = true;
                        }
                     }
                     if ( requiredService == VehicleService::srv_ammo ) {
                        if ( getTargetUnit()->aiparam[ai->getPlayerNum()]->job == AiParameter::job_supply ) {
                           if ( target.service[j].orgSourceAmount > requiredAmount )
                              enoughResources = true;
                        } else
                           if ( target.service[j].maxPercentage > 80 )
                              enoughResources = true;
                     }
                  }
               }

            if ( enoughResources )
               result = true;

         }
      }

      if( result )
         return serviceAmount;
   }
   return 0;
}


bool AI::ServiceOrder::execute1st ( pvehicle supplier )
{
   bool result = false;

   pvehicle targ = getTargetUnit();
   MapCoordinate3D meet;
   int xy_dist = maxint;
   int z_dist = maxint;
   int currentHeight = log2( supplier->height );
   for ( int h = 0; h < 8; h++ )
      if ( supplier->typ->height & ( 1<<h))
         for ( int i = 0; i < sidenum; i++ ) {
             int x = targ->xpos;
             int y = targ->ypos;
             getnextfield ( x, y, i );
             pfield fld = getfield ( x, y );
             if ( fld && fieldaccessible ( fld, supplier, 1<<h ) == 2 && !fld->building && !fld->vehicle ) {
                int d = beeline ( x, y, supplier->xpos, supplier->ypos);
                AStar ast ( ai->getMap(), supplier );
                vector<MapCoordinate> path;
                ast.findPath(  path, x, y );
                if ( path.size() ) {
                   if ( abs ( currentHeight - h) < z_dist || ( abs( currentHeight - h) == z_dist && d < xy_dist )) {
                      TemporaryContainerStorage tus ( supplier );
                      supplier->xpos = x;
                      supplier->ypos = y;
                      supplier->height = 1 << h;

                      VehicleService vs ( NULL, NULL);
                      vs.fieldSearch.init ( supplier, NULL );
                      vs.fieldSearch.checkVehicle2Vehicle ( targ, targ->xpos, targ->ypos );

                      VehicleService::TargetContainer::iterator i = vs.dest.find ( targetUnitID );
                      if ( i != vs.dest.end() ) {
                         VehicleService::Target target = i->second;
                         for ( int j = 0; j < target.service.size(); j++ )
                            if ( target.service[j].type == requiredService )
                               result = true;
                      }

                      tus.restore();

                      if ( result ) {
                         meet.x = x;
                         meet.y = y;
                         meet.z = h;
                         xy_dist = d;
                         z_dist = abs ( currentHeight - h);
                      }

                   }
                }
             }
         }


   if ( xy_dist < maxint ) {
      int supplySpeed = supplier->maxMovement();
      if ( !canWait() )
         supplySpeed += targ->maxMovement();

      if ( xy_dist / supplySpeed < nextServiceBuildingDistance/targ->maxMovement()) {
         supplier->aiparam[ai->getPlayerNum()]->dest = meet;
         supplier->aiparam[ai->getPlayerNum()]->task = AiParameter::tsk_move;
         supplier->aiparam[ai->getPlayerNum()]->dest_nwid = targ->networkid;

         setServiceUnit ( supplier );
         return true;
      }
   }

   return false;
}


bool AI::ServiceOrder::timeOut ( )
{
  tgametime t = time;
  t.a.turn += 2;
  return ( t.abstime <= ai->getMap()->time.abstime );
}

bool AI::ServiceOrder::canWait( )
{
   if ( requiredService == VehicleService::srv_repair )
      return false;

   bool fuelLack = requiredService == VehicleService::srv_resource && position == 2 ;
   return fuelLack || !timeOut();
}



bool AI::ServiceOrder::valid (  ) const
{
   switch ( requiredService ) {
      case VehicleService::srv_repair     : return getTargetUnit()->damage > 0;
      case VehicleService::srv_ammo       : return getTargetUnit()->ammo[position] < getTargetUnit()->typ->weapons[position].count;
      case VehicleService::srv_resource   : return getTargetUnit()->tank.resource(position) < getTargetUnit()->typ->tank.resource(position);
      default: return false;
   }
}

bool AI::ServiceOrder::invalid ( const ServiceOrder& so )
{
   return !so.valid();
}


bool AI :: ServiceTargetEquals :: operator() (const ServiceOrder& so ) const
{
   pvehicle t = so.getTargetUnit();
   return target== t;
}

void AI::removeServiceOrdersForUnit ( const pvehicle veh )
{
   serviceOrders.erase ( remove_if ( serviceOrders.begin(), serviceOrders.end(), ServiceTargetEquals( veh ) ), serviceOrders.end());
}

void AI :: runServiceUnit ( pvehicle supplyUnit )
{
   bool destinationReached = false;
   typedef multimap<float,ServiceOrder*> ServiceMap;
   ServiceMap serviceMap;

   for ( ServiceOrderContainer::iterator i = serviceOrders.begin(); i != serviceOrders.end(); i++ ) {
       if ( !i->getServiceUnit() ) {
          int poss = i->possible( supplyUnit );
          if ( poss ) {
             float f =  i->getTargetUnit()->aiparam[getPlayerNum()]->getValue() * poss/100 / beeline( i->getTargetUnit() ,supplyUnit );
             serviceMap.insert(make_pair(f,&(*i)));
          }
  /*
          veh->aiparam[getPlayerNum()]->task = AiParameter::tsk_serviceRetreat;
          pbuilding bld = findServiceBuilding( **i );
          if ( bld )
             veh->aiparam[ getPlayerNum() ]->dest = bld->getEntry ( );
  */
       }
   }
   for ( ServiceMap::reverse_iterator ri = serviceMap.rbegin(); ri != serviceMap.rend(); ri++ ) {
      if ( (*ri).second->execute1st( supplyUnit ) ) {
         destinationReached = runUnitTask ( supplyUnit );
         break;
      }
   }

   if ( destinationReached ) {
      VehicleService vs ( mapDisplay, NULL );
      if ( !vs.available ( supplyUnit ))
         displaymessage ("AI :: runServiceUnit ; inconsistency in VehicleService.availability",1 );

      vs.execute ( supplyUnit, -1, -1, 0, -1, -1 );
      int target = supplyUnit->aiparam[getPlayerNum()]->dest_nwid;
      VehicleService::TargetContainer::iterator i = vs.dest.find ( target );
      if ( i != vs.dest.end() ) {
         vs.fillEverything ( target, true );
         pvehicle targetUnit = getMap()->getUnit(target);
         if ( targetUnit->aiparam[getPlayerNum()]->task == AiParameter::tsk_serviceRetreat ) {
             targetUnit->aiparam[getPlayerNum()]->task = AiParameter::tsk_nothing;
             targetUnit->aiparam[getPlayerNum()]->dest = MapCoordinate ( -1, -1 );
         }


         removeServiceOrdersForUnit ( targetUnit );

         // search for next unit to be serviced
         runServiceUnit( supplyUnit );
      } else
         displaymessage ("AI :: runServiceUnit ; inconsistency in VehicleService.execution level 0",1 );

   }
}


AI::AiResult AI :: executeServices ( )
{
  // removing all service orders for units which no longer exist
  removeServiceOrdersForUnit ( NULL );
  serviceOrders.erase ( remove_if ( serviceOrders.begin(), serviceOrders.end(), ServiceOrder::invalid ), serviceOrders.end());

  AiResult res;

  for ( Player::VehicleList::iterator vi = getPlayer().vehicleList.begin(); vi != getPlayer().vehicleList.end(); vi++ ) {
      pvehicle veh = *vi;
      checkKeys();
      if ( veh->aiparam[getPlayerNum()]->job == AiParameter::job_supply )
         runServiceUnit ( veh );
  }

  for ( ServiceOrderContainer::iterator i = serviceOrders.begin(); i != serviceOrders.end(); i++ ) {
      if ( !i->canWait() ) {
         pvehicle veh = i->getTargetUnit();
         veh->aiparam[getPlayerNum()]->task = AiParameter::tsk_serviceRetreat;
         if ( i->getServiceUnit()) {
            veh->aiparam[ getPlayerNum() ]->dest = i->getServiceUnit()->getPosition();
            veh->aiparam[ getPlayerNum() ]->dest_nwid = i->getServiceUnit()->networkid;
         } else {
            if ( (veh->height & ( chtieffliegend | chfliegend | chhochfliegend )) && veh->typ->fuelConsumption ) {
               AirplaneLanding apl ( *this, veh );
               MapCoordinate3D dst = apl.getNearestLandingPosition( true, true, false );
               if ( dst.x != -1 ) {
                  veh->aiparam[ getPlayerNum() ]->dest = dst;
               }
            } else {
               MapCoordinate3D dest = findServiceBuilding( *i );
               if ( dest.valid() )
                  veh->aiparam[ getPlayerNum() ]->dest = dest;
               else {
                  // displaymessage("warning: no service building found found for unit %s - %d!",1, veh->typ->description, veh->typ->id);
               }
            }
         }

      }
  }

  for ( Player::VehicleList::iterator vi = getPlayer().vehicleList.begin(); vi != getPlayer().vehicleList.end(); vi++ ) {
     pvehicle veh = *vi;
     checkKeys();

     if ( veh->aiparam[getPlayerNum()]->task == AiParameter::tsk_serviceRetreat ) {
        moveUnit ( veh, veh->aiparam[ getPlayerNum() ]->dest, true );
        if ( veh->getPosition() == veh->aiparam[ getPlayerNum() ]->dest ) {
           VehicleService vc ( mapDisplay, NULL );
           pfield fld = getfield ( veh->xpos, veh->ypos );
           if ( fld->building ) {
              MapCoordinate mc = fld->building->getEntry();
              vc.execute ( NULL, mc.x, mc.y, 0, -1, -1 );

              if ( vc.getStatus () == 2 ) {
                 if ( vc.dest.find ( veh->networkid ) != vc.dest.end() )
                    vc.fillEverything ( veh->networkid, true );
                 else
                    displaymessage ( "AI :: executeServices / Vehicle cannot be serviced (1) ", 1);
              } else
                 displaymessage ( "AI :: executeServices / Vehicle cannot be serviced (2) ", 1);

              removeServiceOrdersForUnit ( veh );

              if ( veh->aiparam[getPlayerNum()]->task == AiParameter::tsk_serviceRetreat ) {
                  veh->aiparam[getPlayerNum()]->task = AiParameter::tsk_nothing;
                  veh->aiparam[getPlayerNum()]->dest = MapCoordinate ( -1, -1 );
              }

           }
        }
     }
  }

  /*
  for ( ServiceOrderContainer::iterator i = serviceOrders.begin(); i != serviceOrders.end(); i++ ) {
      if ( !i->timeOut() && i->requiredService == VehicleService::srv_repair ) {
         pvehicle veh = i->getTargetUnit();
         veh->aiparam[getPlayerNum()]->task = AiParameter::tsk_serviceRetreat;
         pbuilding bld = findServiceBuilding( *i );
         if ( bld )
            veh->aiparam[ getPlayerNum() ]->dest = bld->getEntry ( );
         else {
            // displaymessage("warning: no service building found found for unit %s - %d!",1, veh->typ->description, veh->typ->id);
         }

      }
  }
*/
  return res;

}
