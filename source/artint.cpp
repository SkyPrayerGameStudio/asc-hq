//     $Id: artint.cpp,v 1.23 2000-09-07 16:42:27 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.22  2000/09/07 15:42:09  mbickel
//     *** empty log message ***
//
//     Revision 1.21  2000/09/02 13:59:47  mbickel
//      Worked on AI
//      Started using doxygen
//
//     Revision 1.20  2000/08/29 20:21:03  mbickel
//      Tried to make source GCC compliant, but some problems still remain
//
//     Revision 1.19  2000/08/25 13:42:49  mbickel
//      Fixed: zoom dialogbox in mapeditor was invisible
//      Fixed: ammoproduction: no numbers displayed
//      game options: produceammo and fillammo are now modified together
//      Fixed: sub could not be seen when standing on a mine
//      Some AI improvements
//
//     Revision 1.18  2000/08/12 09:17:13  gulliver
//     *** empty log message ***
//
//     Revision 1.17  2000/08/08 09:47:52  mbickel
//
//      speed up of dialog boxes in linux
//      fixed graphical errors in attack
//      fixed graphical error in ammo transfer
//      fixed reaction fire not allowing manual attack
//
//     Revision 1.16  2000/08/07 21:10:17  mbickel
//      Fixed some syntax errors
//
//     Revision 1.15  2000/08/07 16:29:18  mbickel
//      orbiting units don't consume fuel any more
//      Fixed bug in attack formula; improved attack formula
//      Rewrote reactionfire
//
//     Revision 1.14  2000/08/04 15:10:46  mbickel
//      Moving transports costs movement for units inside
//      refuelled vehicles now have full movement in the same turn
//      terrain: negative attack / defensebonus allowed
//      new mapparameters that affect damaging and repairing of building
//
//     Revision 1.13  2000/08/03 13:11:47  mbickel
//      Fixed: on/off switching of generator vehicle produced endless amounts of energy
//      Repairing units now reduces their experience
//      negative attack- and defenseboni possible
//      changed attackformula
//
//     Revision 1.12  2000/08/02 15:52:37  mbickel
//      New unit set definition files
//      demount accepts now more than one container file
//      Unitset information dialog added
//
//     Revision 1.11  2000/07/29 14:54:07  mbickel
//      plain text configuration file implemented
//
//     Revision 1.10  2000/07/23 17:59:50  mbickel
//      various AI improvements
//      new terrain information window
//
//     Revision 1.9  2000/07/16 14:19:58  mbickel
//      AI has now some primitive tactics implemented
//      Some clean up
//        moved weapon functions to attack.cpp
//      Mount doesn't modify PCX files any more.
//
//     Revision 1.8  2000/07/06 11:07:24  mbickel
//      More AI work
//      Started modularizing the attack formula
//
//     Revision 1.7  2000/07/05 13:26:06  mbickel
//      AI
//
//     Revision 1.6  2000/07/05 10:49:35  mbickel
//      Fixed AI bugs
//      setbuildingdamage event now updates the screen
//
//     Revision 1.5  2000/06/28 18:30:56  mbickel
//      Started working on AI
//      Started making loaders independent of memory layout
//      Destroyed buildings can now leave objects behind.
//
//     Revision 1.4  2000/06/19 20:05:01  mbickel
//      Fixed crash when transfering ammo to vehicle with > 8 weapons
//
//     Revision 1.3  1999/11/22 18:26:43  mbickel
//      Restructured graphics engine:
//        VESA now only for DOS
//        BASEGFX should be platform independant
//        new interface for initialization
//      Rewrote all ASM code in C++, but it is still available for the Watcom
//        versions
//      Fixed bugs in RLE decompression, BI map importer and the view calculation
//
//     Revision 1.2  1999/11/16 03:41:00  tmwilson
//     	Added CVS keywords to most of the files.
//     	Started porting the code to Linux (ifdef'ing the DOS specific stuff)
//     	Wrote replacement routines for kbhit/getch for Linux
//     	Cleaned up parts of the code that gcc barfed on (char vs unsigned char)
//     	Added autoconf/automake capabilities
//     	Added files used by 'automake --gnu'
//
//
/*
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-1999  Martin Bickel  and  Marc Schellenberger

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



#include <stdio.h>

#ifdef _DOS_
 #include <i86.h>
 #include <conio.h>
 #include <dos.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include "artint.h"

#include "tpascal.inc"
#include "basegfx.h"
#include "misc.h"
#include "newfont.h"
#include "mousehnd.h"
#include "typen.h"
#include "keybp.h"
#include "spfst.h"
#include "dlg_box.h"
#include "stack.h"
#include "missions.h"
#include "controls.h"
#include "dialog.h"
#include "timer.h"
#include "gamedlg.h"
//#include "building.h"
#include "attack.h"
#include "gameoptions.h"
#include "astar2.h"




const int value_armorfactor = 100;
const int value_weaponfactor = 70;

const int ccbt_repairfacility = 200;    //  basic threatvalues for buildings
const int ccbt_hq = 10000;
const int ccbt_recycling = 50;
const int ccbt_training = 150;

const int attack_unitdestroyed_bonus = 90;


void nop ( void )
{
}

int compareinteger ( const void* op1, const void* op2 )
{
   const int* a = (const int*) op1;
   const int* b = (const int*) op2;
   return *a > *b;
}

void AiThreat :: reset ( void )
{
   for ( int i = 0; i < aiValueTypeNum; i++ )
      threat[i] = 0;
}

void AiParameter :: reset ( void )
{
   threat.reset ( );

   value = 0;
   xtogo = -1;
   ytogo = -1;
   id = 1;
   task = tsk_nothing;
}

AI :: AI ( pmap _map ) : activemap ( _map ) , sections ( this )
{

   _isRunning = false;
   fieldThreats = NULL;

   reset(); 

   static ReplayMapDisplay* rmd = new ReplayMapDisplay ( &defaultMapDisplay );
   mapDisplay = rmd;
   rmd->setCursorDelay (CGameOptions::Instance()->replayspeed + 30 );
}

void AI :: reset ( void )
{
   maxTrooperMove = 0;  
   maxTransportMove = 0; 
   for ( int i= 0; i < 8; i++ )
      maxWeapDist[i] = -1;  
   baseThreatsCalculated = 0;

   if ( fieldThreats )
      delete[] fieldThreats;

   fieldThreats = NULL;
   fieldNum = 0;

   config.wholeMapVisible = 1;
   config.lookIntoTransports = 1;
   config.lookIntoBuildings = 1;
   config.aggressiveness  = 1;

   sections.reset();
}






  class CalculateThreat_VehicleType {
                         protected:
                              AI*               ai;

                              pvehicletype      fzt;
                              int               weapthreat[8];
                              int               value;

                              virtual int       getdamage ( void )      { return 0;   };
                              virtual int       getexpirience ( void )  { return 0;   };
                              virtual int       getammunition( int i )  { return 1;   };
                              virtual int       getheight ( void )      { return 255; };
                          public:
                              void              calc_threat_vehicletype ( pvehicletype _fzt );
                              CalculateThreat_VehicleType ( AI* _ai ) { ai = _ai; };
                       };

  class CalculateThreat_Vehicle : public CalculateThreat_VehicleType {
                           protected:
                                pvehicle          eht;
                                virtual int       getdamage ( void );
                                virtual int       getexpirience ( void );
                                virtual int       getammunition( int i );
                                virtual int       getheight ( void );
                           public:
                              void              calc_threat_vehicle ( pvehicle _eht );
                              CalculateThreat_Vehicle ( AI* _ai ) : CalculateThreat_VehicleType ( _ai ) {};
                       };


void         CalculateThreat_VehicleType :: calc_threat_vehicletype ( pvehicletype _fzt )
{ 
   fzt = _fzt;

   for ( int j = 0; j < 8; j++ )
      weapthreat[j] = 0;

   for ( int i = 0; i < fzt->weapons->count; i++) 
      if ( fzt->weapons->weapon[i].shootable() ) 
         if ( fzt->weapons->weapon[i].offensive() ) 
            for ( int j = 0; j < 8; j++) 
               if ( fzt->weapons->weapon[i].targ & (1 << j) ) { 
                  int d = 0; 
                  int m = 0; 
                  AttackFormula af;
                  for ( int e = (fzt->weapons->weapon[i].mindistance + maxmalq - 1)/ maxmalq; e <= fzt->weapons->weapon[i].maxdistance / maxmalq; e++ ) {    // the distance between two fields is maxmalq
                     d++; 
                     int n = int( weapDist.getWeapStrength( &fzt->weapons->weapon[i], e*maxmalq ) * fzt->weapons->weapon[i].maxstrength * af.strength_damage(getdamage()) * ( 1 + af.strength_experience(getexpirience())) );
                     m += n / (2*(1+d)); 
                  } 
                  if (getammunition(i) == 0) 
                     m /= 2; 

                  if ( (fzt->weapons->weapon[i].sourceheight & getheight()) == 0) 
                     m /= 2; 

                  if ( !(getheight() & ( 1 << j )))
                     m /= 2;

                  if (m > weapthreat[j]) 
                     weapthreat[j] = m; 
               } 


   if ( !fzt->aiparam[ai->getPlayer()] )
      fzt->aiparam[ ai->getPlayer() ] = new AiParameter;

   for ( int l = 0; l < 8; l++ )
      fzt->aiparam[ ai->getPlayer() ]->threat.threat[l] = weapthreat[l];

   value = fzt->armor * value_armorfactor * (100 - getdamage()) / 100 ;
   qsort ( weapthreat, 8, sizeof(int), compareinteger );

   for ( int k = 0; k < 8; k++ )
      value += weapthreat[k] * value_weaponfactor / (k+1);

   fzt->aiparam[ ai->getPlayer() ]->value = value;
   fzt->aiparam[ ai->getPlayer() ]->valueType = 0;
}


int          CalculateThreat_Vehicle :: getammunition( int i )
{
   return eht->munition[i];
}

int          CalculateThreat_Vehicle :: getheight(void)
{
   return eht->height;
}

int          CalculateThreat_Vehicle :: getdamage(void)
{
   return eht->damage;
}

int          CalculateThreat_Vehicle :: getexpirience(void)
{
   return eht->experience;
}


void         CalculateThreat_Vehicle :: calc_threat_vehicle ( pvehicle _eht )
{

   eht = _eht;
   calc_threat_vehicletype ( eht->typ );

   if ( !eht->aiparam[ai->getPlayer()] )
      eht->aiparam[ai->getPlayer()] = new AiParameter;

   for ( int l = 0; l < 8; l++ )
      eht->aiparam[ai->getPlayer()]->threat.threat[l] = eht->typ->aiparam[ ai->getPlayer() ]->threat.threat[l];

   eht->aiparam[ai->getPlayer()]->value = eht->typ->aiparam[ ai->getPlayer() ]->value;
   eht->aiparam[ai->getPlayer()]->valueType = log2 ( eht->height );

/*
   generatethreatvalue();
   int l = 0;
   for ( int b = 0; b <= 7; b++) {
      eht->threatvalue[b] = weapthreatvalue[b];
      if (weapthreatvalue[b] > l)
         l = weapthreatvalue[b];
   }
   eht->completethreatvalue = threatvalue2 + l;
   eht->completethreatvaluesurr = threatvalue2 + l;
   eht->threats = 0;
*/
}


void  AI :: calculateThreat ( pvehicletype vt)
{
   CalculateThreat_VehicleType ctvt ( this );
   ctvt.calc_threat_vehicletype( vt );
}


void  AI :: calculateThreat ( pvehicle eht )
{
   CalculateThreat_Vehicle ctv ( this );
   ctv.calc_threat_vehicle( eht );
}


void  AI :: calculateThreat ( pbuilding bld )
{
   if ( !bld->aiparam[ getPlayer() ] )
      bld->aiparam[ getPlayer() ] = new AiParameter;

   int b;

   // Since we have two different resource modes now, this calculation should be rewritten....
   bld->aiparam[ getPlayer() ]->value = (bld->plus.a.energy / 10) + (bld->plus.a.fuel / 10) + (bld->plus.a.material / 10) + (bld->actstorage.a.energy / 20) + (bld->actstorage.a.fuel / 20) + (bld->actstorage.a.material / 20) + (bld->maxresearchpoints / 10);
   for (b = 0; b <= 31; b++)
      if ( bld->loading[b]  ) {
         if ( !bld->loading[b]->aiparam[ getPlayer() ] )
            calculateThreat ( bld->loading[b] );
         bld->aiparam[ getPlayer() ]->value += bld->loading[b]->aiparam[ getPlayer() ]->value;
      }

   for (b = 0; b <= 31; b++)
      if ( bld->production[b] )  {
         if ( !bld->production[b]->aiparam[ getPlayer() ] )
            calculateThreat ( bld->production[b] );
         bld->aiparam[ getPlayer() ]->value += bld->production[b]->aiparam[ getPlayer() ]->value / 10;
      }

   if (bld->typ->special & cgrepairfacilityb )
      bld->aiparam[ getPlayer() ]->value += ccbt_repairfacility;
   if (bld->typ->special & cghqb )
      bld->aiparam[ getPlayer() ]->value += ccbt_hq;
   if (bld->typ->special & cgtrainingb )
      bld->aiparam[ getPlayer() ]->value += ccbt_training;
   if (bld->typ->special & cgrecyclingplantb )
      bld->aiparam[ getPlayer() ]->value += ccbt_recycling;
}



void AI :: WeaponThreatRange :: run ( pvehicle _veh, int x, int y, AiThreat* _threat, AI* _ai )
{
   ai = _ai;
   threat = _threat;
   veh = _veh;
   for ( height = 0; height < 8; height++ )
      for ( weap = 0; weap < veh->typ->weapons->count; weap++ )
         if ( veh->height & veh->typ->weapons->weapon[weap].sourceheight )
            if ( (1 << height) & veh->typ->weapons->weapon[weap].targ )
                if ( veh->typ->weapons->weapon[weap].shootable()  && veh->typ->weapons->weapon[weap].offensive() ) {
                   initsuche ( x, y, veh->typ->weapons->weapon[weap].maxdistance/maxmalq, veh->typ->weapons->weapon[weap].mindistance/maxmalq );
                   startsuche();
                }
}

void AI :: WeaponThreatRange :: testfield ( void )
{
   if ( getfield ( xp, yp ))
      if ( dist*maxmalq <= veh->typ->weapons->weapon[weap].maxdistance )
         if ( dist*maxmalq >= veh->typ->weapons->weapon[weap].mindistance ) {
            AttackFormula af;
            int strength = int ( weapDist.getWeapStrength( &veh->typ->weapons->weapon[weap], dist*maxmalq, veh->height, 1 << height )
                                 * veh->typ->weapons->weapon[weap].maxstrength
                                 * (1 + af.strength_experience ( veh->experience ) + af.strength_attackbonus ( getfield(startx,starty)->getattackbonus() ))
                                 * af.strength_damage ( veh->damage )
                                );

            if ( strength ) {
               int pos = xp + yp * ai->getMap()->xsize;
               if ( strength > threat[pos].threat[height] )
                  threat[pos].threat[height] = strength;
            }
         }
}

void AI :: calculateFieldThreats ( void )
{
   if ( fieldNum && fieldNum != activemap->xsize * activemap->ysize ) {
      delete[] fieldThreats;
      fieldThreats = NULL;
      fieldNum = 0;
   }
   if ( !fieldThreats ) {
      fieldNum = activemap->xsize * activemap->ysize;
      fieldThreats = new AiThreat[ fieldNum ];
   } else
      for ( int a = 0; a < fieldNum; a++ )
         fieldThreats[ a ].reset();

   AiThreat*  singleUnitThreat = new AiThreat[fieldNum];

   // we now check the whoe map
   for ( int y = 0; y < activemap->ysize; y++ )
      for ( int x = 0; x < activemap->xsize; x++ ) {
         pfield fld = getfield ( x, y );
         if ( config.wholeMapVisible || fieldvisiblenow ( fld, getPlayer() ) )
            if ( fld->vehicle && getdiplomaticstatus2 ( getPlayer()*8, fld->vehicle->color) == cawar ) {
               WeaponThreatRange wr;
               if ( !fld->vehicle->typ->wait ) {

                  // The unit may have already moved this turn.
                  // So we give it the maximum movementrange

                  int move = fld->vehicle->getMovement() ;

                  fld->vehicle->setMovement ( fld->vehicle->typ->movement [ log2 ( fld->vehicle->height )] );

                  VehicleMovement vm ( NULL, NULL );
                  if ( vm.available ( fld->vehicle )) {
                     vm.execute ( fld->vehicle, -1, -1, 0, -1, -1 );

                     // Now we cycle through all fields that are reachable...
                     for ( int f = 0; f < vm.reachableFields.getFieldNum(); f++ ) {
                        int xp, yp;
                        vm.reachableFields.getFieldCoordinates ( f, &xp, &yp );
                        // ... and check for each which fields are threatened if the unit was standing there
                        wr.run ( fld->vehicle, xp, yp, singleUnitThreat, this );
                     }

                     for ( int g = 0; g < vm.reachableFieldsIndirect.getFieldNum(); g++ ) {
                        int xp, yp;
                        vm.reachableFieldsIndirect.getFieldCoordinates ( g, &xp, &yp );
                        wr.run ( fld->vehicle, xp, yp, singleUnitThreat, this );
                     }
                  }
                  fld->vehicle->setMovement ( move );
               } else
                  wr.run ( fld->vehicle, x, y, singleUnitThreat, this );


               for ( int a = 0; a < fieldNum; a++ ) {
                  for ( int b = 0; b < 8; b++ )
                     fieldThreats[a].threat[b] += singleUnitThreat[a].threat[b];

                  singleUnitThreat[ a ].reset();
               }
            }
      }

}


void     AI :: calculateAllThreats( void )
{
   // Calculates the basethreats for all vehicle types
   if ( !baseThreatsCalculated ) {
      for ( int w = 0; w < vehicletypenum; w++) {
         pvehicletype fzt = getvehicletype_forpos(w);
         if ( fzt )
            calculateThreat( fzt );

      }
      baseThreatsCalculated = 1;
   }

   // Some further calculations that only need to be done once.
   if ( maxTrooperMove == 0) {
      for ( int v = 0; v < vehicletypenum; v++) {
         pvehicletype fzt = getvehicletype_forpos( v );
         if ( fzt )
            if ( fzt->functions & ( cf_conquer ) )
               if ( fzt->movement[chfahrend] > maxTrooperMove )   // buildings can only be conquered on ground level, or by moving to adjecent field which is less
                  maxTrooperMove = fzt->movement[chfahrend];
      }
   }
   if ( maxTransportMove == 0 ) {
      for (int v = 0; v < vehicletypenum; v++) {
         pvehicletype fzt = getvehicletype_forpos( v );
         if ( fzt )
            for ( int w = 0; w <= 7; w++) // cycle through all levels of height
               if (fzt->movement[w] > maxTransportMove)
                  maxTransportMove = fzt->movement[w];
      }
   }
   for ( int height = 0; height < 8; height++ )
      if ( maxWeapDist[height] < 0 ) {

         maxWeapDist[height] = 0; // It may be possible that there is no weapon to shoot to a specific height

         for ( int v = 0; v < vehicletypenum; v++) {
            pvehicletype fzt = getvehicletype_forpos( v );
            if ( fzt )
               for ( int w = 0; w < fzt->weapons->count ; w++)
                  if ( fzt->weapons->weapon[w].maxdistance > maxWeapDist[height] )
                     if ( fzt->weapons->weapon[w].targ & ( 1 << height ))   // targ is a bitmap, each bit standing for a level of height
                         maxWeapDist[height] = fzt->weapons->weapon[w].maxdistance;
         }
      }



   // Resetting all previos AI parameters for the units. This should later be removed to allow long-time planning,
   // but at the moment it provides clean starting conditions

   // There are only 8 players in ASC, but there may be neutral units (player == 8)
   for ( int v = 0; v < 9; v++)
      if (activemap->player[v].existent) {

         // Now we cycle through all units of this player
         pvehicle eht = activemap->player[v].firstvehicle;
         while ( eht ) {
            if ( !eht->aiparam[ getPlayer() ] )
               eht->aiparam[ getPlayer() ] = new AiParameter;
            else
               eht->aiparam[ getPlayer() ]->reset();

            calculateThreat ( eht );
            eht = eht->next;
         }
      }
}

AiThreat& AI :: getFieldThreat ( int x, int y )
{
   if ( !fieldThreats )
      calculateFieldThreats ();
   return fieldThreats[y * activemap->xsize + x ];
}


void AI :: Section :: init ( AI* _ai, int _x, int _y, int xsize, int ysize, int _xp, int _yp )
{
   ai = _ai;
   init ( _x, _y, xsize, ysize, _xp, _yp );
}

void AI :: Section :: init ( int _x, int _y, int xsize, int ysize, int _xp, int _yp )
{
   x1 = _x;
   y1 = _y;
   x2 = _x + xsize;
   y2 = _y + ysize;

   xp = _xp;
   yp = _yp;

   if ( x1 < 0 ) x1 = 0;
   if ( y1 < 0 ) y1 = 0;
   if ( x2 >= ai->activemap->xsize ) x2 = ai->activemap->xsize-1;
   if ( y2 >= ai->activemap->ysize ) y2 = ai->activemap->ysize-1;


   centerx = (x1 + x2) / 2;
   centery = (y1 + y2) / 2;
   numberOfFields = (x2-x1+1) * ( y2-y1+1);

   absUnitThreat.reset();
   absFieldThreat.reset();

   for ( int j = 0; j < aiValueTypeNum; j++ )
     value[j] = 0;

   for ( int y = y1; y <= y2; y++ )
      for ( int x = x1; x <= x2; x++ ) {
         absFieldThreat += ai->getFieldThreat ( x, y );
         pfield fld = getfield ( x, y );
         if ( fld->vehicle && getdiplomaticstatus ( fld->vehicle->color )==cawar) {
            if ( !fld->vehicle->aiparam[ ai->getPlayer() ] )
               ai->calculateThreat ( fld->vehicle );
            AiParameter& aip = * fld->vehicle->aiparam[ ai->getPlayer() ];
            absUnitThreat += aip.threat;
            value[ aip.valueType ] += aip.value;
         }
      }

   for ( int i = 0; i <  absUnitThreat.threatTypes; i++ ) {
      avgUnitThreat.threat[i] = absUnitThreat.threat[i] / numberOfFields;
      avgFieldThreat.threat[i] = absFieldThreat.threat[i] / numberOfFields;
   }

}

int AI :: Section :: numberOfAccessibleFields ( const pvehicle veh )
{
   int num = 0;
   for ( int y = y1; y <= y2; y++ )
      for ( int x = x1; x <= x2; x++ )
         if ( fieldaccessible ( getfield ( x, y ), veh ) == 2)
            num++;

   return num;
}

AI :: Sections :: Sections ( AI* _ai ) : ai ( _ai ) , section ( NULL )
{
   sizeX = 8;
   sizeY = 16;
   numX = ai->activemap->xsize * 2 / sizeX + 1;
   numY = ai->activemap->ysize * 2 / sizeY + 1;
}

void AI :: Sections :: reset ( void )
{
   if ( section ) {
      delete[] section;
      section = NULL;
   }
}


void AI :: Sections :: calculate ( void )
{
   if ( !section ) {
      section = new Section[ numX*numY ]; //  ( ai );
      for ( int x = 0; x < numX; x++ )
         for ( int y = 0; y < numY; y++ )
            section[ x + numX * y ].init ( ai, x * ai->activemap->xsize / numX, y * ai->activemap->ysize / numY, sizeX, sizeY, x, y );

    }
}

AI::Section& AI :: Sections :: getForCoordinate ( int xc, int yc )
{
   if ( !section )
      calculate();

   int dist = maxint;
   Section* sec = NULL;
   for ( int x = 0; x < numX; x++ )
      for ( int y = 0; y < numY; y++ ) {
         Section& s2 = getForPos ( x, y );
         int d = beeline ( xc, yc, s2.centerx, s2.centery);
         if ( d < dist ) {
            dist = d;
            sec = &getForPos ( x, y );
         }
      }

   return *sec;
}

AI::Section& AI :: Sections :: getForPos ( int xn, int yn )
{
   if ( xn >= numX || yn >= numY || xn < 0 || yn < 0 )
      displaymessage( "AI :: Sections :: getForPos - invalid parameters: %d %d", 2, xn, yn );

   return section[xn+yn*numX];
}

AI::Section* AI :: Sections :: getBest ( const pvehicle veh, int* xtogo, int* ytogo )
{
   AStar ast;
   ast.findAllAccessibleFields ( ai->getMap(), veh );


   AiParameter& aip = *veh->aiparam[ ai->getPlayer() ];

   float d = minfloat;
   AI::Section* frst = NULL;
   for ( int y = 0; y < numY; y++ )
      for ( int x = 0; x < numX; x++ ) {
          AI::Section& sec = getForPos( x, y );
          float t = 0;
          for ( int i = 0; i < aiValueTypeNum; i++ )
             t += aip.threat.threat[i] * sec.value[i];

          float f;
          if ( sec.avgUnitThreat.threat[aip.valueType] )
             f = t / sec.avgUnitThreat.threat[aip.valueType];
          else
             f = t;

          int dist = beeline ( veh->xpos, veh->ypos, sec.centerx, sec.centery );
          if ( dist )
             f /= log(dist);

          if ( f > d ) {
             int ac  = 0;
             int nac = 0;
             int mindist = maxint;
             for ( int yp = sec.y1; yp <= sec.y2; yp++ )
                for ( int xp = sec.x1; xp <= sec.x2; xp++ ) {
                   pfield fld = getfield ( xp, yp );
                   if ( fld->a.temp == 1 ) {
                      if ( xtogo && ytogo ) {
                         int mandist = abs( sec.centerx - xp ) + 2*abs ( sec.centery - yp );
                         if ( mandist < mindist ) {
                            mindist = mandist;
                            *xtogo = xp;
                            *ytogo = yp;
                         }
                      }
                      ac++;
                   } else
                      nac++;
                }

             if ( 100 * nac / (nac+ac) < 70 ) {   // less than 80% of fields not accessible
                d = f;
                frst = &getForPos ( x, y );
             }
          }
      }

   return frst;

}


void    AI :: setup (void)
{
   displaymessage2("calculating all threats ... ");
   calculateAllThreats ();

   displaymessage2("calculating field threats ... ");
   calculateFieldThreats();

   displaymessage2("calculating sections ... ");
   sections.calculate();

/*
   for ( i = 0; i <= 8; i++) {
      pbuilding building = actmap->player[i].firstbuilding;
      while (building != NULL) {
         generatethreatvaluebuilding(building);
         building = building->next;
      }
      if (i == actmap->actplayer) {
         building = actmap->player[i].firstbuilding;
         while (building != NULL) {
            tcmpcheckreconquerbuilding ccrcb;
            ccrcb.init(3);
            ccrcb.initsuche(building->xpos,building->ypos,(maxfusstruppenmove + maxtransportmove) / 8 + 1,0);
            ccrcb.startsuche();
            int j;
            ccrcb.returnresult( &j );
            ccrcb.done();
            building = building->next;
         }
      }
   }
   */

   /*
    punits units = new tunits;
    tjugdesituationspfd jugdesituationspfd;
    jugdesituationspfd.init(units,1);
    jugdesituationspfd.startsuche();
    jugdesituationspfd.done();
    delete units;
   */

   // showthreats("init: threatvals generated");
   displaymessage2("setup completed ... ");
}

void AI :: searchTargets ( pvehicle veh, int x, int y, TargetList* tl, int moveDist )
{
   npush ( veh->xpos );
   npush ( veh->ypos );

   veh->removeview();
   veh->xpos = x;
   veh->ypos = y;
   veh->addview();
   evaluateviewcalculation ( 1 << activemap->playerview );

   VehicleAttack va ( NULL, NULL );
   if ( va.available ( veh )) {
      va.execute ( veh, -1, -1, 0 , 0, -1 );
      for ( int g = 0; g < va.attackableVehicles.getFieldNum(); g++ ) {
         int xp, yp;
         va.attackableVehicles.getFieldCoordinates ( g, &xp, &yp );
         pattackweap aw = va.attackableVehicles.getData ( g );

         int bestweap = -1;
         int targdamage = -1;
         int weapstrength = -1;
         for ( int w = 0; w < aw->count; w++ ) {
            tunitattacksunit uau ( veh, getfield ( xp, yp)->vehicle, 1, aw->num[w] );
            uau.calc();
            if ( uau.dv.damage > targdamage ) {
               bestweap = aw->num[w];
               targdamage = uau.dv.damage;
               weapstrength = aw->strength[w];
            } else
            if ( uau.dv.damage == 100 )
               if ( weapstrength == -1 || weapstrength > aw->strength[w] ) {
                  bestweap = aw->num[w];
                  targdamage = uau.dv.damage;
                  weapstrength = aw->strength[w];
               }
         }

         if ( bestweap == -1 )
            displaymessage ( "inconsistency in AI :: searchTarget", 1 );

         MoveVariant* mv = &(*tl)[tl->getlength()+1 ];  // sometime this should all be modified to use the STL...

         tunitattacksunit uau ( veh, getfield ( xp, yp)->vehicle, 1, bestweap );
         mv->orgDamage = uau.av.damage;
         mv->damageAfterMove = uau.av.damage;
         mv->enemyOrgDamage = uau.dv.damage;
         uau.calc();


         mv->damageAfterAttack = uau.av.damage;
         mv->enemyDamage = uau.dv.damage;
         mv->enemy = getfield ( xp, yp )->vehicle;
         mv->movex = x;
         mv->movey = y;
         mv->attackx = xp;
         mv->attacky = yp;
         mv->weapNum = bestweap;
         mv->moveDist = moveDist;

      }
   }

   veh->removeview();
   npop ( veh->ypos );
   npop ( veh->xpos );

   veh->addview();
   evaluateviewcalculation ( 1 << activemap->playerview );
}


void AI::tactics( void )
{
   displaymessage2("starting tactics ... ");

   pvehicle veh = getMap()->player[ getPlayer() ].firstvehicle;
   while ( veh ) {
      if ( veh->weapexist() ) {
         int orgmovement = veh->getMovement();
         int orgxpos = veh->xpos ;
         int orgypos = veh->ypos ;

         VehicleMovement vm ( NULL, NULL );
         if ( vm.available ( veh )) {
            vm.execute ( veh, -1, -1, 0, -1, -1 );

            TargetList targetList;

            searchTargets ( veh, veh->xpos, veh->ypos, &targetList, 0 );

            // Now we cycle through all fields that are reachable...
            if ( !veh->typ->wait )
               for ( int f = 0; f < vm.reachableFields.getFieldNum(); f++ )
                  if ( !vm.reachableFields.getField( f )->vehicle && !vm.reachableFields.getField( f )->building ) {
                      int xp, yp;
                      vm.reachableFields.getFieldCoordinates ( f, &xp, &yp );
                      searchTargets ( veh, xp, yp, &targetList, beeline ( xp, yp, orgxpos, orgypos ) );
                   }

            int bestpos = -1;
            int bestres = -1;
            int bestmove = maxint;
            for ( int i = 0; i <= targetList.getlength(); i++ ) {
               MoveVariant* mv = &targetList[i];

               mv->result = int ((mv->enemyDamage - mv->enemyOrgDamage) * mv->enemy->aiparam[getPlayer()]->value - config.aggressiveness * (mv->damageAfterAttack - mv->orgDamage) * veh->aiparam[getPlayer()]->value );
               if ( mv->enemyDamage >= 100 )
                  mv->result += mv->enemy->aiparam[getPlayer()]->value * attack_unitdestroyed_bonus;

               if ( mv->result > bestres || (mv->result == bestres && bestmove > mv->moveDist )) {
                  bestres = mv->result;
                  bestpos = i;
                  bestmove = mv->moveDist;
               }
            }
            if ( bestpos >= 0 ) {
               MoveVariant* mv = &targetList[bestpos];

               if ( mv->movex != veh->xpos || mv->movey != veh->ypos ) {
                  VehicleMovement vm2 ( mapDisplay, NULL );
                  vm2.execute ( veh, -1, -1, 0, -1, -1 );
                  if ( vm2.getStatus() != 2 )
                     displaymessage ( "AI :: tactics \n error in movement step 0 with unit %d", 1, veh->networkid );

                  vm2.execute ( NULL, mv->movex, mv->movey, 2, -1, -1 );
                  if ( vm2.getStatus() != 3 )
                     displaymessage ( "AI :: tactics \n error in movement step 2 with unit %d", 1, veh->networkid );

                  vm2.execute ( NULL, mv->movex, mv->movey, 3, -1,  1 );
                  if ( vm2.getStatus() != 1000 )
                     displaymessage ( "AI :: tactics \n error in movement step 3 with unit %d", 1, veh->networkid );
               }

               VehicleAttack va ( mapDisplay, NULL );
               va.execute ( veh, -1, -1, 0 , 0, -1 );
               if ( va.getStatus() != 2 )
                  displaymessage ( "AI :: tactics \n error in attack step 2 with unit %d", 1, veh->networkid );

               va.execute ( NULL, mv->attackx, mv->attacky, 2 , -1, mv->weapNum );
               if ( va.getStatus() != 1000 )
                  displaymessage ( "AI :: tactics \n error in attack step 3 with unit %d", 1, veh->networkid );

            }
         }
      }
      veh = veh->next;
   }
   displaymessage2("tactics completed ... ");
}

void AI::strategy( void )
{
   displaymessage2("starting strategy ... ");

   pvehicle veh = getMap()->player[ getPlayer() ].firstvehicle;
   while ( veh ) {
      if ( veh->weapexist() ) {
         int orgmovement = veh->getMovement();
         int orgxpos = veh->xpos ;
         int orgypos = veh->ypos ;

         VehicleMovement vm ( mapDisplay, NULL );
         if ( vm.available ( veh )) {
            int x2, y2;

            AI::Section* sec = sections.getBest ( veh, &x2, &y2 );
            if ( sec ) {
               vm.execute ( veh, -1, -1, 0, -1, -1 );

               std::vector<int> path;
               findPath ( getMap(), path, veh, x2, y2 );
               int x = veh->xpos;
               int y = veh->ypos;
               int xtogo = x;
               int ytogo = y;

               for ( int i = path.size()-1; i >= 0 ; i-- ) {
                  getnextfield ( x, y, path[i] );

                  pfield fld = getfield ( x, y );
                  if ( !fld)
                     break;


                  if ( vm.reachableFields.isMember ( x, y ) && fieldaccessible ( fld, veh ) == 2 && !fld->building && !fld->vehicle) {
                     xtogo = x;
                     ytogo = y;
                  }
               }

               if ( xtogo != veh->xpos || ytogo != veh->ypos ) {
                  AiParameter& aip = *veh->aiparam[getPlayer()];

                  aip.xtogo = x;
                  aip.ytogo = y;

                  vm.execute ( NULL, xtogo, ytogo, 2, -1, -1 );
                  if ( vm.getStatus() != 3 )
                     displaymessage ( "AI :: strategy \n error in movement step 2 with unit %d", 1, veh->networkid );

                  vm.execute ( NULL, xtogo, ytogo, 3, -1,  1 );
                  if ( vm.getStatus() != 1000 )
                     displaymessage ( "AI :: strategy \n error in movement step 3 with unit %d", 1, veh->networkid );
               }

            }
         }
      }
      veh = veh->next;
   }

   displaymessage2("strategy completed ... ");
}


void AI:: run ( void )
{
   _isRunning = true;

   tempsvisible = false;
   setup();
   tempsvisible = true;

   tactics();
   strategy();
   displaymap();

   _isRunning = false;
}

bool AI :: isRunning ( void )
{
  return _isRunning;
}


void AI :: showFieldInformation ( int x, int y )
{
   if ( fieldThreats ) {
      const char* fieldinfo = "#font02#Field Information#font01##aeinzug20##eeinzug10##crtp10#"
                              "threat orbit: %d\n"
                              "threat high-level flight: %d\n"
                              "threat flight: %d\n"
                              "threat low-level flight: %d\n"
                              "threat ground level: %d\n"
                              "threat floating: %d\n"
                              "threat submerged: %d\n"
                              "threat deep submerged: %d\n";

      char text[10000];
      int pos = x + y * activemap->xsize;
      sprintf(text, fieldinfo, fieldThreats[pos].threat[7], fieldThreats[pos].threat[6], fieldThreats[pos].threat[5],
                               fieldThreats[pos].threat[4], fieldThreats[pos].threat[3], fieldThreats[pos].threat[2],
                               fieldThreats[pos].threat[1], fieldThreats[pos].threat[0] );

      pfield fld = getfield (x, y );
      if ( fld->vehicle && fieldvisiblenow ( fld )) {
         char text2[1000];
         sprintf(text2, "\nunit nwid: %d ; typeid: %d", fld->vehicle->networkid, fld->vehicle->typ->id );
         strcat ( text, text2 );
         AiParameter& aip = *fld->vehicle->aiparam[getPlayer()];

         if ( fld->vehicle->aiparam ) {
            sprintf(text2, "\nunit value: %d", aip.value );
            strcat ( text, text2 );
         }

         if ( aip.xtogo >= 0 && aip.ytogo >= 0 ) {
            getMap()->cleartemps ( 1 );
            getfield ( aip.xtogo, aip.ytogo )->a.temp = 1;
            displaymap();
         }

      }
      strcat ( text, "\n#font02#Section Information#font01##aeinzug20##eeinzug10##crtp10#");
      string s;

      Section& sec = sections.getForCoordinate ( x, y );

      s += "xp = ";
      s += strrr ( sec.xp );
      s += " ; yp = ";
      s += strrr ( sec.yp );
      s += "\n";
      const char* threattypes[4] = { "absUnitThreat", "avgUnitThreat", "absFieldThreat", "avgFieldThreat" };

      for ( int i = 0; i < 4; i++ ) {
         for ( int h = 0; h < 8; h++ ) {
            s += threattypes[i];
            s += " ";
            s += choehenstufen [h];
            s += " ";
            switch ( i ) {
               case 0: s += strrr ( sec.absUnitThreat.threat[h] );
                       break;
               case 1: s += strrr ( sec.avgUnitThreat.threat[h] );
                       break;
               case 2: s += strrr ( sec.absFieldThreat.threat[h] );
                       break;
               case 3: s += strrr ( sec.avgFieldThreat.threat[h] );
                       break;
            }
            s += "\n";
         }
         s += "\n";
      }

      for ( int j = 0; j < aiValueTypeNum; j++ ) {
         s+= "\nvalue ";
         s+= strrr ( j );
         s+= " = ";
         s+= strrr ( sec.value[j] );
      }

      strcat ( text, s.c_str() );
      tviewanytext vat;
      vat.init ( "AI information", text );
      vat.run();
      vat.done();
   }
}


AI :: ~AI ( )
{
   if ( fieldThreats ) {
      delete[] fieldThreats;
      fieldThreats = NULL;
      fieldNum = 0;
   }
}
