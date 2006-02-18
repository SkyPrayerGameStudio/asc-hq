/*! \file edmain.cpp
    \brief The map editor's main program 
*/

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

#include <algorithm>
#include <memory>
#include <SDL_image.h>
#include <signal.h>

#include "edmisc.h"
#include "loadbi3.h"
#include "edselfnt.h"
#include "edglobal.h"
#include "errors.h"
#include "gameoptions.h"
#include "mapdisplay.h"
#include "itemrepository.h"
#include "loadimage.h"
#include "graphicset.h"
#include "paradialog.h"
#include "soundList.h"
#include "maped-mainscreen.h"
#include "cannedmessages.h"
#include "stdio-errorhandler.h"

#ifdef WIN32
# include "win32/win32-errormsg.h"
# include  "win32/msvc/mdump.h"
 MiniDumper miniDumper( "asc1mapeditor" );
#endif



// #define MEMCHK
#include "memorycheck.cpp"

pfont load_font(char* name)
{
   tnfilestream stream ( name, tnstream::reading );
   return loadfont ( &stream );
}

void loaddata( void ) 
{
   loadmessages();

   dataLoaderTicker();

   loadguipictures();

   dataLoaderTicker();

   GraphicSetManager::Instance().loadData();
      
   registerDataLoader ( new PlayListLoader() );
   registerDataLoader ( new BI3TranslationTableLoader() );

   loadAllData();

   dataLoaderTicker();

   loadUnitSets();

   schriften.smallarial = load_font("smalaril.fnt");
   schriften.large = load_font("usablack.fnt");
   schriften.arial8 = load_font("arial8.fnt");
   schriften.smallsystem = load_font("msystem.fnt");
   schriften.monogui = load_font("monogui.fnt");

}

void buildemptymap ( void )
{
   if ( terrainTypeRepository.getObject_byID(30) )
      generatemap(terrainTypeRepository.getObject_byID(30)->weather[0], 10, 20);
   else
      generatemap(terrainTypeRepository.getObject_byPos(0)->weather[0], 10, 20);
}


int mapeditorMainThread ( void* _mapname )
{
   const char* mapname = (const char*) _mapname;
   loadpalette();

  
   try {
      StartupScreen sus( "title_mapeditor.jpg", dataLoaderTicker );
      
      GraphicSetManager::Instance().loadData();
      loaddata();

      if ( mapname && mapname[0] ) {
         if( patimat ( savegameextension, mapname )) {
            if( validatesavfile( mapname ) == 0 )
               fatalError ( "The savegame %s is invalid. Aborting.", mapname );

            try {
               loadgame( mapname );
            } catch ( tfileerror ) {
               fatalError ( "%s is not a legal savegame. ", mapname );
            }
         } else
            if( patimat ( mapextension, mapname )) 
               actmap = mapLoadingExceptionChecker( mapname, MapLoadingFunction( tmaploaders::loadmap )); 
            else
               fatalError ( "%s is not an accepted file format", mapname );
      } else
         buildemptymap();

      mapSwitcher.toggle();
      if ( exist ( "palette.map" ))
         actmap = mapLoadingExceptionChecker( "palette.map", MapLoadingFunction( tmaploaders::loadmap )); 
      else
         buildemptymap();

      actmap->preferredFileNames.mapname[0] = "";

      mapSwitcher.toggle();

   } /* end try */
   catch ( ParsingError err ) {
      displaymessage ( "Error parsing text file " + err.getMessage(), 2 );
   }
   catch ( tfileerror err ) {
      displaymessage ( " error loading file %s ",2, err.getFileName().c_str() );
   } /* end catch */

   pulldownfont = schriften.smallarial;

   activefontsettings.font = schriften.arial8;
   activefontsettings.color =lightblue ;
   activefontsettings.background =3;
   activefontsettings.length =100;
   activefontsettings.justify =lefttext;


   setstartvariables();

   mainScreenWidget = new Maped_MainScreenWidget( getPGApplication());
   mainScreenWidget->Show();
   
   mousevisible(true);

   getPGApplication().Run();
   
   if ( actmap ) {
      delete actmap;
      actmap = NULL;
   }
   return 0;
}

// including the command line parser, which is generated by genparse
#include "clparser/mapedit.cpp"

void setSaveNotification()
{
   mapsaved = false;
}

int main(int argc, char *argv[] )
{
   // setenv( "DISPLAY", "192.168.0.61:0", 1 );
   
   StdIoErrorHandler stdIoErrorHandler;
   
   Cmdline* cl = NULL;
   auto_ptr<Cmdline> apcl ( cl );
   try {
      cl = new Cmdline ( argc, argv );
   }
   catch ( string s ) {
      cerr << s;
      exit(1);
   }

   if ( cl->next_param() < argc ) {
      cerr << "invalid command line parameter\n";
      exit(1);
   }

   if ( cl->v() ) {
      ASCString msg = kgetstartupmessage();
      printf( msg.c_str() );
      exit(0);
   }

   MessagingHub::Instance().setVerbosity( cl->r() );
   MessagingHub::Instance().exitHandler.connect( SigC::bind( SigC::slot( exit_asc ), -1 ));

   #ifdef logging
    logtofile ( kgetstartupmessage() );
    logtofile ( "\n new log started \n ");
   #endif

#ifdef WIN32
   Win32IoErrorHandler* win32ErrorDialogGenerator = new Win32IoErrorHandler;
#endif


   initFileIO( cl->c() );

   signal ( SIGINT, SIG_IGN );

   fullscreen = !CGameOptions::Instance()->mapeditWindowedMode;
   if ( cl->f() )
      fullscreen = 1;
   if (  cl->w() )
      fullscreen = 0;

   checkDataVersion();

   // determining the graphics resolution
   int xr  = CGameOptions::Instance()->mapeditor_xresolution;
   if ( cl->x() != 800 )
      xr = cl->x();

   int yr  = CGameOptions::Instance()->mapeditor_yresolution;
   if ( cl->y() != 600 )
      yr = cl->y();

   ASC_PG_App app ( "asc2_dlg" );
   
   int flags = SDL_SWSURFACE;
   if ( fullscreen )
      flags |= SDL_FULLSCREEN;
   
   app.InitScreen( xr, yr, 32, flags);
#ifdef WIN32
   delete win32ErrorDialogGenerator;
#endif
      
   #ifdef pbpeditor
   setWindowCaption ( "Advanced Strategic Command : PBP Editor ");
   #else
   setWindowCaption ( "Advanced Strategic Command : Map Editor ");
   #endif

   virtualscreenbuf.init();

   mapChanged.connect( SigC::hide<GameMap*>( repaintMap.slot() ) );
   mapChanged.connect( SigC::hide<GameMap*>( updateFieldInfo.slot() ) );
   mapChanged.connect( SigC::hide<GameMap*>( SigC::slot( setSaveNotification) ));
   
   char* buf = new char[cl->l().length()+10];
   strcpy ( buf, cl->l().c_str() );
   initializeEventHandling ( mapeditorMainThread, buf  );
   delete[] buf;

   writegameoptions ();
   

   return 0;
}

