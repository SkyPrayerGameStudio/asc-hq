/*! \file soundList.cpp
    \brief The collection of all sounds used by ASC
*/

/*
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-2010  Martin Bickel  

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
#include <stdlib.h>

#ifndef _DOS_
 #include "global.h"
 #include <SDL.h>
#endif

#include "soundList.h"
#include "basestrm.h"
#include "sgstream.h"
#include "stringtokenizer.h"
#include "textfiletags.h"

SoundList* SoundList::instance = NULL;


SoundLoopManager :: SoundLoopManager ( Sound* snd, bool _active )
                  : sound( snd ), active ( _active )
{
	displayLogMessage ( 10, " Instantiating SoundLoopManager \n" ); // : sound = %x, active = %d \n", int(snd), int(_active) );
}


void SoundLoopManager :: activate ( int dummy )
{
   if ( !active && sound ) {
      sound->playLoop();
      displayLogMessage ( 10, " SoundLoopManager: playing sound\n" );
   }
   active = true;
}
  

SoundList& SoundList::getInstance()
{
   if ( !instance )
      fatalError("SoundList::getInstance() - Soundlist not initialized");

   return *instance;
}


void SoundList::init( )
{
   if ( instance )
      fatalError("SoundList::init() - Soundlist already initialized");

   static SoundList myList;
   instance = &myList;
   displayLogMessage ( 4, "  SoundList::init() : starting initialize ..." );
   instance->initialize ( );
   displayLogMessage ( 4, "  completed\n" );

}

void SoundList::readLine( PropertyContainer& pc, const ASCString& name, SoundList::Sample sample, int subtype )
{
   vector<ASCString> labels;
   vector<ASCString> files;
   if ( pc.find ( name + ".files" ))
      pc.addStringArray ( name + ".files", files );
   if ( pc.find ( name + ".labels" ))
      pc.addStringArray ( name + ".labels", labels );

   SoundAssignment s;
   if ( files.size() && !files[0].empty() )
      s.defaultSound = getSound( files[0] );
   else
      s.defaultSound = NULL;

   s.sample = sample;
   s.subType = subtype;

   for( int i = 0; i < labels.size() && i < files.size() ; i++ )
      s.snd[ copytoLower(labels[i]) ] = getSound( files[i] );

   soundAssignments.push_back ( s );
}

void SoundList::initialize(  )
{
   TextPropertyGroup* tpg = NULL;
   {
      tnfilestream s ( "sounds.asctxt", tnstream::reading );

      TextFormatParser tfp ( &s );
      tpg = tfp.run();
   }
   auto_ptr<TextPropertyGroup> atpg ( tpg );

   PropertyReadingContainer pc ( "sounds", tpg );

   pc.openBracket("shoot");
   dataLoaderTicker();
    readLine( pc, "MACHINEGUN", SoundList::shooting, 6 );
   dataLoaderTicker();
    readLine( pc, "CRUISEMISSILE", SoundList::shooting, 0 );
   dataLoaderTicker();
    readLine( pc, "MINE", SoundList::shooting, 1 );
   dataLoaderTicker();
    readLine( pc, "BOMB", SoundList::shooting, 2 );
   dataLoaderTicker();
    readLine( pc, "LARGEMISSILE", SoundList::shooting, 3 );
   dataLoaderTicker();
    readLine( pc, "SMALLMISSILE", SoundList::shooting, 4 );
   dataLoaderTicker();
    readLine( pc, "TORPEDO", SoundList::shooting, 5 );
   dataLoaderTicker();
    readLine( pc, "CANNON", SoundList::shooting, 7 );
   dataLoaderTicker();
    readLine( pc, "LASER", SoundList::shooting, 10);
   pc.closeBracket();

   dataLoaderTicker();
   pc.openBracket("move");
    readLine( pc, "default", SoundList::moving, 0 );
   dataLoaderTicker();
    readLine( pc, "LIGHT_TRACKED_VEHICLE", SoundList::moving, 1 );
   dataLoaderTicker();
    readLine( pc, "MEDIUM_TRACKED_VEHICLE", SoundList::moving, 2 );
   dataLoaderTicker();
    readLine( pc, "HEAVY_TRACKED_VEHICLE", SoundList::moving, 3 );
   dataLoaderTicker();
    readLine( pc, "LIGHT_WHEELED_VEHICLE", SoundList::moving, 4 );
   dataLoaderTicker();
    readLine( pc, "MEDIUM_WHEELED_VEHICLE", SoundList::moving, 5 );
   dataLoaderTicker();
    readLine( pc, "HEAVY_WHEELED_VEHICLE", SoundList::moving, 6 );
   dataLoaderTicker();
    readLine( pc, "TROOPER", SoundList::moving, 7 );
   dataLoaderTicker();
    readLine( pc, "RAIL_VEHICLE", SoundList::moving, 8 );
   dataLoaderTicker();
    readLine( pc, "MEDIUM_AIRCRAFT", SoundList::moving, 9 );
   dataLoaderTicker();
    readLine( pc, "MEDIUM_SHIP", SoundList::moving, 10 );
   dataLoaderTicker();
    readLine( pc, "TURRET", SoundList::moving, 11 );
   dataLoaderTicker();
    readLine( pc, "LIGHT_AIRCRAFT", SoundList::moving, 12 );
   dataLoaderTicker();
    readLine( pc, "HEAVY_AIRCRAFT", SoundList::moving, 13 );
   dataLoaderTicker();
    readLine( pc, "LIGHT_SHIP", SoundList::moving, 14 );
   dataLoaderTicker();
    readLine( pc, "HEAVY_SHIP", SoundList::moving, 15 );
   dataLoaderTicker();
    readLine( pc, "HELICOPTER", SoundList::moving, 16 );
   dataLoaderTicker();
   pc.closeBracket();

   pc.openBracket("UserInterface");
    readLine( pc, "ACKNOWLEDGE", SoundList::menu_ack );
   pc.closeBracket();
   dataLoaderTicker();
   readLine( pc, "CONQUER_BUILDING", SoundList::conquer_building );
   dataLoaderTicker();
   readLine( pc, "UNIT_EXPLODES", SoundList::unitExplodes );
   dataLoaderTicker();
   readLine( pc, "BUILDING_COLLAPSES", SoundList::buildingCollapses );
   dataLoaderTicker();
   readLine( pc, "REFUEL", SoundList::refuel );
   dataLoaderTicker();
   readLine( pc, "REPAIR", SoundList::repair );
   dataLoaderTicker();
   readLine( pc, "JUMPDRIVE", SoundList::jumpdrive );

}

Sound* SoundList::getSound( const ASCString& filename )
{
   if ( SoundSystem::getInstance()->isOff() )
      return NULL;

   displayLogMessage ( 5, " SoundList::getSound(1) : trying to acquire handle for sound %s \n", filename.c_str() );

   if ( soundFiles.find ( filename ) == soundFiles.end() ) {
     displayLogMessage ( 5, " Sound has not been loaded ...\n" );

     Sound* s;
     if( filename.find(':') != ASCString::npos ) {
        ASCString primary = filename.substr( 0, filename.find(':'));
        ASCString secondary = filename.substr( filename.find(':')+1 );
        displayLogMessage ( 8, " this is a multipart sound\n" );
        s = new Sound( primary, secondary );
     } else {
        s = new Sound ( filename );
     }

      dataLoaderTicker();


     soundFiles[filename] = s;
     if ( s != NULL )
        displayLogMessage ( 5, " loading sound completed\n" );
     else
        displayLogMessage ( 5, " loading sound failed\n" );

     return s;
  } else
     return soundFiles[filename];
}


Sound* SoundList::getSound( Sample snd, int subType, const ASCString& label, int height )
{
   if ( SoundSystem::getInstance()->isOff() )
      return NULL;

   if ( label.find ( "." ) != ASCString::npos ) {
      return getSound ( label );
   } else {
      ASCString newlabel = copytoLower(label);

      for ( vector<SoundAssignment>::iterator i = soundAssignments.begin(); i != soundAssignments.end(); i++ )
         if ( snd == i->sample && subType == i->subType ) {
            if ( height >= 0 && height <= 7 )
               if ( i->snd.find( heightTags[height] ) != i->snd.end() ) {
	               displayLogMessage ( 10, ASCString(" SoundList::getSound(2) : heightlabel ") + heightTags[height] + " found, returning matching sound \n" );
                  return i->snd[heightTags[height]];
               }

            if ( newlabel.empty() || i->snd.find( newlabel ) == i->snd.end() ) {
            	displayLogMessage ( 10, " SoundList::getSound(2) : label " + label + " not found, returning default sound \n" );
               if ( !i->defaultSound )
               	displayLogMessage ( 10, "  SoundList::getSound(2) : no default sound registered !!!! \n" );
               return i->defaultSound;
            } else {
	            displayLogMessage ( 10, " SoundList::getSound(2) : label " + label + " found, returning matching sound \n" );
               return i->snd[newlabel];
            }
         }
   }

	displayLogMessage ( 10, " SoundList::getSound(2) : sound not found, returning NULL \n" );
   return NULL;
}

Sound* SoundList::playSound( Sample snd, int subType , bool looping, const ASCString& label  )
{
   Sound* sound = getSound ( snd, subType, label );
   if ( !sound )
      return NULL;

   if ( looping )
      sound->playLoop();
   else
      sound->play();

   return sound;
}

SoundList::~SoundList()
{
   for ( SoundFiles::iterator i = soundFiles.begin(); i != soundFiles.end(); i++ )
      delete i->second;
}

