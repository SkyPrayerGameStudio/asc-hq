//     $Id: building_controls.h,v 1.2 2000-09-25 20:04:37 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.1  2000/09/17 15:20:29  mbickel
//      AI is now automatically invoked (depending on gameoptions)
//      Some cleanup
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


#ifdef karteneditor
 #error the mapeditor should not need to use building* !
#endif


#ifndef building_controls_h
#define building_controls_h


enum tcontainermode { mbuilding, mtransport };
enum tunitmode { mnormal, mproduction, mloadintocontainer };


typedef  class ccontainercontrols*  pcontainercontrols;
typedef  class cbuildingcontrols*   pbuildingcontrols;
typedef  class ctransportcontrols*  ptransportcontrols;



class    ccontainercontrols
{
   public :
      class crepairanything
      {
         public:
            int      energycosts, materialcosts, fuelcosts;
            int      checkto  ( int olddamage, int newdamage, int energycost, int materialcost, int fuelcost, int effizienz );
      };

      class    crepairunit : public crepairanything
      {                          // REPAIRUNIT
         public :
            virtual int  checkto  (pvehicle eht, char newdamage) = 0;
            int          repairto (pvehicle eht, char newdamage);
            int          available ( pvehicle eht );
      };

      class    crefill
      {
         public :
            void     fuel (pvehicle eht, int newfuel);                     // der neue Werte darf durchaus ?ber dem Maximum liegen. Eine entsprechende Pr?fung wird durchgef?hrt.
            void     material (pvehicle eht, int newmaterial);
            void     ammunition (pvehicle eht, char weapon, int ammun );
            void     filleverything ( pvehicle eht );
            void     emptyeverything ( pvehicle eht );
      }
      refill;

      class   cmove_unit_in_container
      {
         public:
            int      moveupavail ( pvehicle eht );
            int      movedownavail ( pvehicle eht, pvehicle into );
            void     moveup ( pvehicle eht );
            void     movedown ( pvehicle eht, pvehicle into );
            pvehicle unittomove;
      }
      move_unit_in_container;

      ccontainercontrols (void);

      virtual char   getactplayer (void) = 0;

      virtual int    moveavail ( pvehicle eht ) = 0;


      virtual int    putenergy (int e, int abbuchen = 1 ) = 0;
      virtual int    putmaterial (int m, int abbuchen = 1 ) = 0;
      virtual int    putfuel (int f, int abbuchen = 1) = 0;
      virtual int    getenergy ( int need, int abbuchen ) = 0;
      virtual int    getmaterial ( int need, int abbuchen ) = 0;
      virtual int    getfuel ( int need, int abbuchen ) = 0;

      virtual int    putammunition (int  weapontype, int  ammunition, int abbuchen) = NULL;
      virtual int    getammunition ( int weapontype, int num, int abbuchen, int produceifrequired = 0 ) = 0;
      virtual int    ammotypeavail ( int type ) = 0;

      virtual int    getxpos (void) = 0;
      virtual int    getypos (void) = 0;

      virtual VehicleMovement*  movement ( pvehicle eht );
      virtual int    getHeight ( void ) = 0;
      virtual int    getLoadCapability ( void ) = 0;


      virtual int    getspecfunc ( tcontainermode mode ) = 0;

      virtual pvehicle getloadedunit (int num) = NULL;

      struct
      {
         int height;
         int movement;
         int attacked;
      }
      movementparams;

      ContainerBase* baseContainer;
};



class    cbuildingcontrols : public virtual ccontainercontrols
{
   private :

      char  getactplayer (void);


      int   getxpos (void);
      int   getypos (void);


   public :
      int    putenergy (int e , int abbuchen = 1);
      int    putmaterial (int m, int abbuchen = 1 );
      int    putfuel (int f, int abbuchen = 1 );
      int    getenergy ( int need, int abbuchen );
      int    getmaterial ( int need, int abbuchen );
      int    getfuel ( int need, int abbuchen );
      int    putammunition ( int weapontype, int  ammunition, int abbuchen);
      int    getspecfunc ( tcontainermode mode );
      int    getammunition ( int weapontype, int num, int abbuchen, int produceifrequired = 0 );
      int    ammotypeavail ( int type );
      int    getHeight ( void );
      int    getLoadCapability ( void );

      pbuilding   building;

      class   crepairunitinbuilding
               : public virtual ccontainercontrols::crepairunit
      {
         public:
            virtual int      checkto  (pvehicle eht, char newdamage);
      }
      repairunit;

      class    crecycling
      {                           // RECYCLING
         public :
            int      material, energy;
            void     resourceuse ( pvehicle eht );
            void     recycle (pvehicle eht);
      }
      recycling;

      class    ctrainunit
      {
         public :
            int  available  ( pvehicle eht );
            void trainunit ( pvehicle eht );
      }
      training;

      class    cproduceunit
      {                         // PRODUCEUNIT
         public :
            int         available ( pvehicletype fzt );
            pvehicle    produce ( pvehicletype fzt );
            pvehicle    produce_hypothetically ( pvehicletype fzt );
      }
      produceunit ;

      class    cdissectunit
      {
         public :
            int  available  ( pvehicle eht );
            void dissectunit ( pvehicle eht );
      }
      dissectunit;

      class    cnetcontrol
      {
         public:
            void setnetmode (  int category, int stat );  // stat  1: setzen    0: l�schen
            int  getnetmode ( int mode );
            void emptyeverything ( void );
            void reset ( void );
      }
      netcontrol;

      class    crepairbuilding
               : public ccontainercontrols::crepairanything
      {
         public :
            int      checkto  ( char newdamage);
            int      repairto ( char newdamage);
            int      available ( void );
      }
      repairbuilding;


      class    cproduceammunition
      {
         public:
            int baseenergyusage;
            int basematerialusage;
            int basefuelusage;
            int materialneeded;
            int energyneeded;
            int fuelneeded;
            int checkavail ( int weaptype, int num );
            void produce ( int weaptype, int num );
            cproduceammunition ( void );
      }
      produceammunition;


      void  removevehicle ( pvehicle *peht );
      int    moveavail ( pvehicle eht );

      pvehicle getloadedunit (int num);


      cbuildingcontrols (void);
      void  init (pbuilding bldng);
};

class    ctransportcontrols : public virtual ccontainercontrols
{
   private :

      char  getactplayer (void);


      int   getxpos (void);
      int   getypos (void);



   public :
      int    putenergy (int e, int abbuchen = 1 );
      int    putmaterial (int m, int abbuchen = 1 );
      int    putfuel (int f, int abbuchen = 1 );
      int    getenergy ( int need, int abbuchen );
      int    getmaterial ( int need, int abbuchen );
      int    getfuel ( int need, int abbuchen );
      int    putammunition ( int weapontype, int  ammunition, int abbuchen);
      int    getspecfunc ( tcontainermode mode );
      int    getammunition ( int weapontype, int num, int abbuchen, int produceifrequired = 0 );
      int    ammotypeavail ( int type );
      int    getHeight ( void );
      int    getLoadCapability ( void );


      pvehicle vehicle;

      class   crepairunitintransport
               : public virtual ccontainercontrols::crepairunit
      {
         public:
            virtual int      checkto  (pvehicle eht, char newdamage);
      }
      repairunit;


      void  removevehicle ( pvehicle *peht );
      int    moveavail ( pvehicle eht );

      pvehicle getloadedunit (int num);

      ctransportcontrols ( void );
      void  init ( pvehicle eht );
};

#endif // building_controls_h