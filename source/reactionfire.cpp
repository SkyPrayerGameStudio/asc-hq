/*! \file controls.cpp
   Controlling units (which is graudally moved to #vehicletype.cpp and #unitctrl.cpp );
   Things that are run when starting and ending someones turn
*/


/*
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-2005  Martin Bickel  and  Marc Schellenberger

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


#include "buildings.h"
#include "vehicle.h"
#include "typen.h"
#include "gamemap.h"
#include "stack.h"
#include "viewcalculation.h"
#include "replay.h"
#include "attack.h"
#include "messaginghub.h"

#include "reactionfire.h"


treactionfirereplay :: treactionfirereplay ( void )
{
   num = 0;
   unit = NULL;
}

void treactionfirereplay :: init ( Vehicle* eht, const AStar3D::Path& fieldlist )
{
   if ( runreplay.status > 0 ) {
      preactionfire_replayinfo rpli;
      do {
         rpli = runreplay.getnextreplayinfo ( );
         if ( rpli )
            replay[num++] = rpli;

      } while ( rpli );

      unit = eht;
   }
}

int  treactionfirereplay :: checkfield ( const MapCoordinate3D& pos, Vehicle* &eht, MapDisplayInterface* md )
{
   int attacks = 0;

   if ( eht == unit ) {
      for ( int i = 0; i < num; i++ ) {
         preactionfire_replayinfo rpli = replay[ i ];
         if ( eht && rpli->x2 == pos.x  && rpli->y2 == pos.y ) {

             pfield fld = getfield ( rpli->x1, rpli->y1 );
             pfield targ = getfield ( rpli->x2, rpli->y2 );

             npush ( targ->vehicle );
             targ->vehicle = eht;
             eht->xpos = pos.x;
             eht->ypos = pos.y;
             eht->height = pos.getBitmappedHeight();


             int attackvisible = fieldvisiblenow ( fld ) || fieldvisiblenow ( targ );


             if ( md && attackvisible ) {
             /*
               // cursor.setcolor ( 8 );

               cursor.gotoxy ( rpli->x1, rpli->y1 );
               int t = ticker;
               while ( t + 15 > ticker )
                  releasetimeslice();

               cursor.gotoxy ( pos.x, pos.y );
               t = ticker;
               while ( t + 15 > ticker )
                  releasetimeslice();

               // cursor.setcolor ( 0 );
               // cursor.hide();
               */
             }
             attacks++;

             tunitattacksunit battle ( fld->vehicle, targ->vehicle, 1, rpli->wpnum );
             battle.av.damage = rpli->ad1;
             battle.dv.damage = rpli->dd1;
             if ( md && attackvisible  )
                battle.calcdisplay ( rpli->ad2, rpli->dd2 );
             else {
                battle.calc ();
                battle.av.damage = rpli->ad2;
                battle.dv.damage = rpli->dd2;
             }

             int killed = 0;
             if ( battle.dv.damage >= 100 )
                killed = 1;

             battle.setresult ();
             updateFieldInfo();

             npop ( targ->vehicle );

             if ( killed )
                eht = NULL;
         }
      }
   }
   return attacks;
}

treactionfirereplay :: ~treactionfirereplay ( )
{
   for ( int i = 0; i < num; i++ )
      delete replay[i];
}



                typedef struct tunitlist* punitlist;
                struct tunitlist {
                        Vehicle* eht;
                        punitlist next;
                     };

                punitlist unitlist[8];


int tsearchreactionfireingunits :: maxshootdist[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

tsearchreactionfireingunits :: tsearchreactionfireingunits ( void )
{
   for ( int i = 0; i < 8; i++ )
      unitlist[i] = NULL;
}

void tsearchreactionfireingunits :: init ( Vehicle* vehicle, const AStar3D::Path& fieldlist )
{
   int x1 = maxint;
   int y1 = maxint;
   int x2 = 0;
   int y2 = 0;
   int j, h;

   if ( maxshootdist[0] == -1 ) {
      for (int i = 0; i < 8; i++ )
         maxshootdist[i] = 0;

      for (int i = 0; i < vehicleTypeRepository.getNum(); i++ ) {
         Vehicletype* fzt = vehicleTypeRepository.getObject_byPos ( i );
         if ( fzt )
            for (j = 0; j < fzt->weapons.count; j++ )
               if ( fzt->weapons.weapon[j].shootable() )
                  for (h = 0; h < 8; h++ )
                     if ( fzt->weapons.weapon[j].targ & ( 1 << h ) )
                        if ( fzt->weapons.weapon[j].maxdistance > maxshootdist[h] )
                           maxshootdist[h] = fzt->weapons.weapon[j].maxdistance;
      }
   }

   for ( AStar3D::Path::const_iterator i = fieldlist.begin(); i != fieldlist.end(); i++) {
      if ( i->x > x2 )
         x2 = i->x ;
      if ( i->y > y2 )
         y2 = i->y ;

      if ( i->x < x1 )
         x1 = i->x ;
      if ( i->y < y1 )
         y1 = i->y ;
   }
   int height = log2 ( vehicle->height );

   x1 -= maxshootdist[height];
   y1 -= maxshootdist[height];
   x2 += maxshootdist[height];
   y2 += maxshootdist[height];

   if ( x1 < 0 )
      x1 = 0;
   if ( y1 < 0 )
      y1 = 0;
   if ( x2 >= actmap->xsize )
      x2 = actmap->xsize -1;
   if ( y2 >= actmap->ysize )
      y2 = actmap->ysize -1;

   for ( int y = y1; y <= y2; y++ )
      for ( int x = x1; x <= x2; x++ ) {
         Vehicle* eht = getfield ( x, y )->vehicle;
         if ( eht )
            if ( eht->color != vehicle->color )
               if ( eht->reactionfire.getStatus() >= Vehicle::ReactionFire::ready )
                  if ( eht->reactionfire.enemiesAttackable & ( 1 << ( vehicle->color / 8 )))
                     if ( actmap->player[eht->getOwner()].diplomacy.isHostile( actmap->actplayer))
                        if ( attackpossible2u ( eht, vehicle, NULL, vehicle->typ->height ) )
                           addunit ( eht );

      }
   if ( getfield(vehicle->xpos, vehicle->ypos)->vehicle == vehicle )
      for ( int i = 0; i < 8; i++ )
         if ( fieldvisiblenow ( getfield ( vehicle->xpos, vehicle->ypos ), i )) {
            punitlist ul  = unitlist[i];
            while ( ul ) {
               punitlist next = ul->next;
               
               pattackweap atw = attackpossible ( ul->eht, vehicle->xpos, vehicle->ypos );
               for ( int j = 0; j < atw->count; ++j )
                  if ( ul->eht->reactionfire.weaponShots[atw->num[j]] ) {
                     removeunit ( ul->eht );
                     break;
                  }   

               delete atw;
               ul = next;
            } /* endwhile */
         }

}

void  tsearchreactionfireingunits :: addunit ( Vehicle* eht )
{
   int c = eht->color / 8;
   punitlist ul = new tunitlist;
   ul->eht = eht;
   ul->next= unitlist[c];
   unitlist[c] = ul;
}


void tsearchreactionfireingunits :: removeunit ( Vehicle* vehicle )
{
   int c = vehicle->color / 8;
   punitlist ul = unitlist[c];
   punitlist last = NULL;
   while ( ul  &&  ul->eht != vehicle ) {
      last = ul;
      ul = ul->next;
  }
  if ( ul  &&  ul->eht == vehicle ) {
     if ( last )
        last->next = ul->next;
     else
        unitlist[c] = ul->next;

     delete ul;
  }
}


int  tsearchreactionfireingunits :: checkfield ( const MapCoordinate3D& pos, Vehicle* &vehicle, MapDisplayInterface* md )
{

   int attacks = 0;
   int result = 0;

   pfield fld = getfield( pos.x, pos.y );
   npush ( fld->vehicle );
   fld->vehicle = vehicle;
   vehicle->xpos = pos.x;
   vehicle->ypos = pos.y;
   int oldheight = vehicle->height;
   if ( pos.getNumericalHeight() >= 0 )
      vehicle->height = pos.getBitmappedHeight();

   for ( int i = 0; i < 8; i++ ) {
      evaluatevisibilityfield ( actmap, fld, i, -1, actmap->getgameparameter ( cgp_initialMapVisibility ) );
      if ( fieldvisiblenow ( fld, i )) {
         punitlist ul  = unitlist[i];
         while ( ul  &&  !result ) {
            punitlist next = ul->next;
            if ( find ( ul->eht->reactionfire.nonattackableUnits.begin(), ul->eht->reactionfire.nonattackableUnits.end(), vehicle->networkid) == ul->eht->reactionfire.nonattackableUnits.end() ) {
               pattackweap atw = attackpossible ( ul->eht, pos.x, pos.y );
               if ( atw->count && (ul->eht->reactionfire.enemiesAttackable & (1 << (vehicle->color / 8)))) {

                  int ad1, ad2, dd1, dd2;

                  int strength = 0;
                  int num = -1;
                  for ( int j = 0; j < atw->count; j++ )
                     if ( ul->eht->reactionfire.weaponShots[atw->num[j]] > 0 )
                        if ( atw->strength[j]  > strength ) {
                           strength = atw->strength[j];
                           num = j;
                        }

                  if ( num >= 0 ) {

                     int visibility = 0;
                     if ( md ) {
                        MessagingHub::Instance().statusInformation( "attacking with weapon " + ASCString::toString( atw->num[num] ));
                        
                        // cursor.setcolor ( 8 );

                        if ( fieldvisiblenow ( getfield (ul->eht->xpos, ul->eht->ypos ), actmap->playerView)) {
                           ++visibility;
                           md->cursor_goto( ul->eht->getPosition() );
                           int t = ticker;
                           while ( t + 15 > ticker )
                              releasetimeslice();
                        }

                        if ( fieldvisiblenow ( fld, actmap->playerView)) {
                           ++visibility;
                           md->cursor_goto( pos );
                           int t = ticker;
                           while ( t + 15 > ticker )
                              releasetimeslice();
                        }
/*
                        cursor.setcolor ( 0 );
                        cursor.hide();
                        */
                     }

                     Vehicle* veh = ul->eht;
                     tunitattacksunit battle ( veh, fld->vehicle, 0, atw->num[num] );
                     int nwid = fld->vehicle->networkid;

                     ad1 = battle.av.damage;
                     dd1 = battle.dv.damage;

                     if ( md && visibility)
                        battle.calcdisplay ();
                     else
                        battle.calc();

                     ad2 = battle.av.damage;
                     dd2 = battle.dv.damage;

                     attacks++;

                     if ( battle.dv.damage >= 100 )
                        result = 1;

                     // ul->eht->reactionfire.enemiesAttackable &= 0xff ^ ( 1 <<  (vehicle->color / 8) );

                     ul->eht->reactionfire.weaponShots[atw->num[num]]--;
                     ul->eht->reactionfire.nonattackableUnits.push_back ( nwid );

                     removeunit ( ul->eht );

                     battle.setresult();

                     if ( ad2 < 100 )
                        veh->attacked = false;

      //               logtoreplayinfo ( rpl_reactionfire, ulex, uley, x, y, ad1, ad2, dd1, dd2, atw->num[num] );

                     updateFieldInfo();
                  }


               }
               delete atw;
            }
            ul = next;
         } /* endwhile */
      }
   }
   npop ( fld->vehicle );
   for ( int i = 0; i < 8; i++ )
      evaluatevisibilityfield ( actmap, fld, i, -1, actmap->getgameparameter ( cgp_initialMapVisibility ) );

   vehicle->height = oldheight;

   if ( result )
      vehicle = NULL;

   return attacks;
}

tsearchreactionfireingunits :: ~tsearchreactionfireingunits()
{
   for ( int i = 0; i < 8; i++ ) {
      punitlist ul = unitlist[i];
      while ( ul ) {
         punitlist ul2 = ul->next;
         delete ul;
         ul = ul2;
      }
   }
}


