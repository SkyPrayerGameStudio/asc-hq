//     $Id: sgstream.cpp,v 1.46 2000-12-27 22:23:15 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.45  2000/11/29 09:40:25  mbickel
//      The mapeditor has now two maps simultaneously active
//      Moved memorychecking functions to its own file: memorycheck.cpp
//      Rewrote password handling in ASC
//
//     Revision 1.44  2000/11/26 22:18:56  mbickel
//      Added command line parameters for setting the verbosity
//      Increased verbose output
//
//     Revision 1.43  2000/11/21 20:27:07  mbickel
//      Fixed crash in tsearchfields (used by object construction for example)
//      AI improvements
//      configure.in: added some debug output
//                    fixed broken check for libbz2
//
//     Revision 1.42  2000/11/14 20:36:41  mbickel
//      The AI can now use supply vehicles
//      Rewrote objecttype IO routines to make the structure independant of
//       the memory layout
//
//     Revision 1.41  2000/11/08 19:31:13  mbickel
//      Rewrote IO for the tmap structure
//      Fixed crash when entering damaged building
//      Fixed crash in AI
//      Removed item CRCs
//
//     Revision 1.40  2000/10/26 18:15:00  mbickel
//      AI moves damaged units to repair
//      tmap is not memory layout sensitive any more
//
//     Revision 1.39  2000/10/18 14:14:18  mbickel
//      Rewrote Event handling; DOS and WIN32 may be currently broken, will be
//       fixed soon.
//
//     Revision 1.38  2000/10/17 13:04:13  mbickel
//      New terrainaccess reading/writing
//      Added Win32 project files
//
//     Revision 1.37  2000/10/17 12:12:22  mbickel
//      Improved vehicletype loading/saving routines
//      documented some global variables
//
//     Revision 1.36  2000/10/14 14:16:08  mbickel
//      Cleaned up includes
//      Added mapeditor to win32 watcom project
//
//     Revision 1.35  2000/10/14 13:07:01  mbickel
//      Moved DOS version into own subdirectories
//      Win32 version with Watcom compiles and links ! But doesn't run yet...
//
//     Revision 1.34  2000/10/12 22:24:02  mbickel
//      Made the DOS part of the new platform system work again
//
//     Revision 1.33  2000/10/12 21:37:55  mbickel
//      Further restructured platform dependant routines
//
//     Revision 1.32  2000/10/12 19:51:45  mbickel
//      Added a stub program for generating a weapon guide
//      Added makefiles to compile this weaponguide with the free borland C++
//        compiler
//      Made some adjustments to basic IO file for compiling them with borland
//        C++
//
//     Revision 1.31  2000/10/11 14:26:47  mbickel
//      Modernized the internal structure of ASC:
//       - vehicles and buildings now derived from a common base class
//       - new resource class
//       - reorganized exceptions (errors.h)
//      Split some files:
//        typen -> typen, vehicletype, buildingtype, basecontainer
//        controls -> controls, viewcalculation
//        spfst -> spfst, mapalgorithm
//      bzlib is now statically linked and sources integrated
//
//     Revision 1.30  2000/09/20 15:05:10  mbickel
//      Better error handling on startup.
//
//     Revision 1.29  2000/08/21 17:51:02  mbickel
//      Fixed: crash when unit reaching max experience
//      Fixed: crash when displaying research image
//      Fixed: crash when events referenced a unit that has been shot down
//      Fixed: screenshot being written to wrong directory
//
//     Revision 1.28  2000/08/12 09:17:33  gulliver
//     *** empty log message ***
//
//     Revision 1.27  2000/08/08 13:22:09  mbickel
//      Added unitCategoriesLoadable property to buildingtypes and vehicletypes
//      Added option: showUnitOwner
//
//     Revision 1.26  2000/08/05 13:38:36  mbickel
//      Rewrote height checking for moving units in and out of
//        transports / building
//
//     Revision 1.25  2000/08/04 15:11:18  mbickel
//      Moving transports costs movement for units inside
//      refuelled vehicles now have full movement in the same turn
//      terrain: negative attack / defensebonus allowed
//      new mapparameters that affect damaging and repairing of building
//
//     Revision 1.24  2000/08/02 18:18:09  mbickel
//      Fixed broken Watcom Projectfiles for tools
//      Makebld can now save a buildings image
//
//     Revision 1.23  2000/08/02 17:27:50  mbickel
//      Renamed translation to transformation in unitset definition file
//
//     Revision 1.22  2000/08/02 15:53:01  mbickel
//      New unit set definition files
//      demount accepts now more than one container file
//      Unitset information dialog added
//
//     Revision 1.21  2000/08/02 10:28:27  mbickel
//      Fixed: generator vehicle not working
//      Streams can now report their name
//      Field information shows units filename
//
//     Revision 1.20  2000/08/01 10:39:14  mbickel
//      Updated documentation
//      Refined configuration file handling
//
//     Revision 1.19  2000/07/31 19:16:48  mbickel
//      Improved handing of multiple directories
//      Fixed: wind direction not displayed when cycling through wind heights
//      Fixed: oil rig not working
//      Fixed: resources becomming visible when checking mining station status
//      Fixed: division by zero when moving unit without fuel consumption
//
//     Revision 1.18  2000/07/31 18:02:54  mbickel
//      New configuration file handling
//      ASC searches its data files in all directories specified in ascrc
//      Renamed all tools so they begin with asc
//
//     Revision 1.17  2000/07/29 15:28:36  mbickel
//      Plaintext configfile runs now in Linux version too
//
//     Revision 1.16  2000/07/29 14:54:43  mbickel
//      plain text configuration file implemented
//
//     Revision 1.15  2000/07/28 10:15:29  mbickel
//      Fixed broken movement
//      Fixed graphical artefacts when moving some airplanes
//
//     Revision 1.14  2000/06/28 19:26:17  mbickel
//      fixed bug in object generation by building removal
//      Added artint.cpp to makefiles
//      Some cleanup
//
//     Revision 1.13  2000/06/28 18:31:02  mbickel
//      Started working on AI
//      Started making loaders independent of memory layout
//      Destroyed buildings can now leave objects behind.
//
//     Revision 1.12  2000/05/23 20:40:49  mbickel
//      Removed boolean type
//
//     Revision 1.11  2000/05/06 20:25:24  mbickel
//      Fixed: -recognition of a second mouse click when selection a pd menu item
//             -movement: fields the unit can only pass, but not stand on them,
//                        are marked darker
//             -intedit/stredit: mouseclick outside is like hitting enter
//
//     Revision 1.10  2000/04/27 16:25:26  mbickel
//      Attack functions cleanup
//      New vehicle categories
//      Rewrote resource production in ASC resource mode
//      Improved mine system: several mines on a single field allowed
//      Added unitctrl.* : Interface for vehicle functions
//        currently movement and height change included
//      Changed timer to SDL_GetTicks
//
//     Revision 1.9  2000/03/29 09:58:48  mbickel
//      Improved memory handling for DOS version
//      Many small changes I can't remember ;-)
//
//     Revision 1.8  2000/03/11 18:22:08  mbickel
//      Added support for multiple graphic sets
//
//     Revision 1.7  2000/02/03 20:54:41  mbickel
//      Some cleanup
//      getfiletime now works under Linux too
//
//     Revision 1.6  2000/01/24 17:35:46  mbickel
//      Added dummy routines for sound under DOS
//      Cleaned up weapon specification
//
//     Revision 1.5  1999/12/28 21:03:20  mbickel
//      Continued Linux port
//      Added KDevelop project files
//
//     Revision 1.4  1999/12/27 13:00:10  mbickel
//      new vehicle function: each weapon can now be set to not attack certain
//                            vehicles
//
//     Revision 1.3  1999/11/22 18:27:54  mbickel
//      Restructured graphics engine:
//        VESA now only for DOS
//        BASEGFX should be platform independant
//        new interface for initialization
//      Rewrote all ASM code in C++, but it is still available for the Watcom
//        versions
//      Fixed bugs in RLE decompression, BI map importer and the view calculation
//
//     Revision 1.2  1999/11/16 03:42:28  tmwilson
//      Added CVS keywords to most of the files.
//      Started porting the code to Linux (ifdef'ing the DOS specific stuff)
//      Wrote replacement routines for kbhit/getch for Linux
//      Cleaned up parts of the code that gcc barfed on (char vs unsigned char)
//      Added autoconf/automake capabilities
//      Added files used by 'automake --gnu'
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

                                               
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include <math.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>


#include "global.h"
#include "CLoadable.h"
#include "tpascal.inc"
#include "typen.h"
#include "buildingtype.h"
#include "vehicletype.h"
#include "basegfx.h"
#include "misc.h"
#include "sgstream.h"
#include "stack.h"
#include "basestrm.h"
#include "palette.h"
#include "gameoptions.h"

#ifndef converter
 #include "dlg_box.h"
 #include "dialog.h"
#endif

#ifdef HEXAGON
 #include "loadbi3.h"
#endif

#ifdef _DOS_
 #include "dos/memory.h"
#endif


const char* asc_EnvironmentName = "ASC_CONFIGFILE";


#ifdef logging
FILE* logfile = NULL;

#ifndef converter
void logtofile ( char* strng, ... )
{
   char buf[10000];
   va_list arglist;
   va_start ( arglist, strng );
   vsprintf ( buf, strng, arglist );

   if ( !logfile )
     logfile = fopen ( "SGLOG.TXT", "at+" );

#ifdef UseMemCheck
   if ( _heapchk() != _HEAPOK  )
     fprintf( logfile, "HEAP DAMAGED!!" );
#endif
   fprintf ( logfile, buf );
   fprintf ( logfile, "\n" );
   fflush ( logfile );
   va_end ( arglist );
}
#endif //converter
#endif //logging


#pragma pack(1)
struct tindexstruct {
                    char         filename[13]; 
                    char         archivename[31]; 
                    int      posstart, posstop; 
                 }; 



void         tsrlefilestream::readrlepict( char** pnter, char allocated, int* size)
{ 
  trleheader   hd; 
  word         w; 
  char*        q;

   if (status != 0) return;

   readdata( (char*)&hd, sizeof( hd ));

   if (hd.id == 16973) { 
      if (!allocated) 
        *pnter = (char*) asc_malloc ( hd.size + sizeof(hd) );
      memcpy( *pnter, &hd, sizeof(hd)); 
      q = *pnter + sizeof(hd);

      readdata( q, hd.size);
      *size = hd.size + sizeof(hd); 
   } 
   else { 
      w = (word) ( (hd.id + 1) * (hd.size + 1) + 4 ); 
      if (!allocated) 
        *pnter = (char*) asc_malloc ( w );
      memcpy ( *pnter, &hd, sizeof ( hd ));
      q = *pnter + sizeof(hd);
      readdata ( q, w - sizeof(hd) );
      *size = w; 
   } 
} 





void tsrlefilestream :: writerlepict ( void* buf )
{
   void* tempbuf = asc_malloc ( 0xffff );
   int   size    = compressrle ( buf, tempbuf );
   writedata ( (char*) tempbuf, size );
   asc_free ( tempbuf );
}



 /*  --------------- tstream -----------------  */ 



void tstream::writedata ( void* buf, int size ) {
   writedata ( (char*) buf, size );
}


void tstream::readdata  ( void* buf, int size ) {
   readdata ( (char*) buf, size );
}


void tstream::writedata ( char buf ) {
   writedata ( &buf, sizeof(char) );
}

void tstream::readdata  ( char* buf ) {
   readdata ( buf, 1 );
}


void tstream::writedata ( word buf ) {
   writedata ( (char*) &buf, sizeof( word ) );
}

void tstream::readdata  ( word* buf ) {
   readdata ( (char*) buf, sizeof ( word ) );
}


void tstream::writedata ( int buf ) {
   writedata ( (char*) &buf, sizeof ( int ) );
}

void tstream::readdata  ( int* buf ) {
   readdata ( (char*) buf, sizeof( int ) );
}
  /*  -------------- tbufstream -------------  */ 



char        tbufstream::getstatus(void)
{ 
   int s;
   s = status;
   status = 0;
   return (char) s;
} 


void         tbufstream::init(void)
{ 
   modus = 0; 
   int maxmemsize = 0xffff;

   devicename = "abstract"; 

  #ifdef UseMemAvail 
  if (memavail() > maxmemsize)
      memsize = maxmemsize; 
   else 
      memsize = memavail();
  #else
   memsize = maxmemsize; 
  #endif


   zeiger = (char*) asc_malloc( memsize );

   #ifdef dlg_box_h
   if (zeiger == NULL) 
      displaymessage("tbufstream::init \ncould not allocate memory !\nmemavail = %d \n",2,
      #ifdef UseMemAvail
       memavail()
      #else
       int(0)
      #endif
      );
   #endif

   datasize = 0; 
} 



void         tbufstream::readdata( char* buf, int size )
{ 
  int          s, actpos2;

   if (status != 0) return;
   actpos2 = 0;

   #ifdef dlg_box_h
   if (modus == 2)
      displaymessage("attempt to read in a file opened only for reading\nfile: %s\n" ,2, devicename) ;
   #endif

   while (actpos2 < size) {
      if (datasize == 0) { 

         #ifdef dlg_box_h
         displaymessage("data read error\nfile: %s\n",2,devicename) ;
         #endif
 
      } 
      s = datasize - actmempos; 
      if (s > size - actpos2) 
         s = size - actpos2; 

      memcpy ( buf + actpos2, zeiger + actmempos, s );

      actmempos = actmempos + s; 
      if (actmempos >= datasize) { 
         readbuffer();
         actmempos = 0; 
      } 
      actpos2 = actpos2 + s; 
   } 

} 


void         tbufstream::readpchar(char** pc)
{
  int          ms;
  char         *pch1, *pch2;

   if (status != 0)
      return;

   int actpos2 = 0;

   #ifdef dlg_box_h
   if (modus == 2)
      displaymessage("attempt to read pchar from a file opened only for writing\nfile: %s\n", 2,devicename);
   #endif

   pch1 = (char*) asc_malloc ( 0xffff );

   pch2 = pch1;
   pch2--;

   do {
     actpos2++;
     pch2++;

     readdata( pch2, 1 );

   } while (*pch2 != 0  ); /* enddo */

   if ( actpos2 > 1 ) {
      pch2 = (char*) asc_malloc ( actpos2 );

      memcpy ( pch2, pch1, actpos2 );
      *pc = pch2;

   } else
      *pc = NULL;
   asc_free( pch1 );

}

void         tbufstream::writepchar(char* pc)
{ 
  if ( pc ) {
     char         *pch1 = pc;
   
     pch1--;
     do {
        pch1++;
        writedata( pch1, 1 );
     } while ( *pch1 > 0  ); /* enddo */
  } else {
     char  a = 0;
     char* b = &a;
     writedata( b, 1 );
  }
} 





void         tbufstream::writedata( char* buf, int size )
{ 
  int         s, actpos2; 

   if (status != 0) return;

   #ifdef dlg_box_h
   if (modus == 1) 
      displaymessage("attempt to write to a file opened only for reading\nfile: %s\n", 2,devicename) ;
   #endif

   actpos2 = 0; 

   while (actpos2 < size) { 
      s = memsize - actmempos; 
      if (s > size - actpos2) 
         s = size - actpos2; 

      memcpy(  zeiger + actmempos, buf + actpos2, s );
      actmempos = actmempos + s; 
      if (actmempos == memsize) { 
         writebuffer();
         actmempos = 0; 
      } 
      else 
         if (actmempos > memsize) 

          #ifdef dlg_box_h
          displaymessage("severe error writing data\nfile: %s\n",2,devicename)
          #endif
          ;
      actpos2 = actpos2 + s; 
   } 
} 


void         tbufstream::done(void)
{ 
   if (modus == 2) {
      writebuffer();
   } /* endif */
   asc_free ( zeiger );
   // fclose(logfile);
} 



  /*  --------------- tfilestream ---------------------  */ 


void         tfilestream::init(void)
{ 
   tbufstream::init();
   actfilepos = 0; 
} 



int         tfilestream::getstreamsize(void)
{ 
   return filesize ( devicename );
}                 

void         tfilestream::openstream(char* name, char mode)
{ 
   if (modus != 0) 
      closestream(); 
   status = 0; 
   modus = mode; 


   // fprintf( logfile, "open file: %s \n", name );
/*
   if (mode == 1) {
      datei.open ( name, ios::in );
   } else {
      datei.open ( name, ios::out );
   }
*/
   if (mode == 1) {
      fp = fopen ( name, filereadmode );
   } else {
      fp = fopen ( name, filewritemode );
   }

   if (fp != NULL) {

     actfilepos = 0;
     datasize = 0;
     if (mode == 1)
       readbuffer();
                
     actmempos = 0;
     status = 0;
     devicename = name;
   } else {

      status = 1;
      #ifdef dlg_box_h
      displaymessage("Could not open file %s: ", 2, name );
      #endif

   } /* endif */
} 






void         tfilestream::seekstream( int newpos, int seekpos )
{ 
   if (status != 0) return;
   if (modus == 2) {
      writebuffer();
   } /* endif */ 
/*   datei.seekg( newpos ); */
   fseek( fp, newpos, seekpos );
   actmempos = 0; 
   actfilepos = newpos; 
   if (modus == 1) 
      readbuffer(); 
}           


void         tfilestream::readbuffer( void )
{ 
   if (status != 0) return;
   /* datasize = datei.read( zeiger, memsize ); */
   datasize = fread( zeiger, 1, memsize, fp);
   status = ferror ( fp );
   actfilepos = actfilepos + datasize;
} 



void         tfilestream::resetstream(void)
{ 
   if (modus == 2) {
      writebuffer();
   }             
   seekstream(0); 
} 



void         tfilestream::writebuffer()
{ 
 /*  result = datai.write (zeiger, actmempos); */
 fwrite( zeiger, 1, actmempos, fp );
 status = ferror ( fp );
 actmempos = 0;
} 


void         tfilestream::closestream(void)
{ 
   if (modus == 2) 
      writebuffer(); 
  
  /* datei.close(); */
//   fprintf( logfile, "close file \n");
  fclose( fp );
   modus = 0; 
   status = 0; 
} 


void         tfilestream::done(void)
{ 
   if (modus != 0) 
      closestream(); 
   tbufstream::done();
} 


  /*  ------------------ tsfilestream ------------------  */ 

/*

void         tsfilestream::seekstream(void)
{ 
   tfilestream::seekstream(startpos + newpos);
} 


void         tsfilestream::openstream( char* name; char mode  )
{ 


  tindexstruct is; 
  int          w; 

   if (modus != 0) 
      closestream(); 
   status = 0; 

   assign(datei,name); 
   reset(&datei,1); 
   if (ioresult != 0) { 
      name = upcasestr(name); 
      assign(datei,"index.dat"); 
      reset(&datei,1); 
      if (ioresult != 0) 
         displaymessage("fatal error 1 at tsfilestream.openstream:",scat("could find neither %s nor index.dat",name),2); 
      do { 
         blockread(datei,is, sizeof(is),w); 
         if (w != sizeof(is)) 
            strcpy(is.filename,""); 
      }  while (!(feof(datei) || (upcasestr(is.filename) == name))); 
      if (upcasestr(is.filename) != name) 
         displaymessage("fatal error 2 at tsfilestream.openstream:",scat("could find neither %s nor entry in index.dat",name),2); 
      startpos = is.posstart; 
      stoppos = is.posstop; 
      inherited(); openstream(is.archivename,mode); 
      seekstream(0); 
   } 
   else { 
      startpos = 0; 
      stoppos = filesize(datei); 
      fclose(datei); 
      inherited(); openstream(name,mode); 
   } 
   status = ioresult; 
   devicename = name; 

} 


void         tsfilestream::readbuffer(void)
{ 
   if (actfilepos + size > stoppos) 
      size = stoppos - actfilepos; 
  tfilestream.readbuffer();
} 


void         tsfilestream::getstreamsize(void)
{ 
   getstreamsize = stoppos - startpos; 
} 



*/



#ifndef converter
const double pi = 3.14159265;

void  rotatepict45 ( void *s, void *d )
{
    struct timg {
       int temp; 
       char pix[unitsizex+1][unitsizey+1];
    } ;
    timg* dest = (timg*) d;
    timg* sorc = (timg*) s;
   {
   
      dest->temp = sorc->temp;
      memset ( dest->pix, 0xff, sizeof ( dest->pix ));
   
      for (int x = 0; x <= unitsizex; x++) {
         double dx = x - 15;
         for (int y = 0; y <= unitsizey; y++ ) {
            if ( sorc->pix[ x][y] != 255 ) {
               double dy = y - 15;
               double alpha ;
               double radius = sqrt ( dx*dx + dy*dy );
               if ( dx > 0 ) {
                  if ( dy >= 0 )
                    alpha =  atan ( dy / dx );
                  else
                    if ( dy < 0 )
                      alpha =  pi * 2 - atan ( -dy / dx );
               } else
                  if ( dx < 0 ) {
                     if ( dy >= 0 )
                       alpha =  pi - atan ( dy / -dx );
                     if ( dy < 0 )
                       alpha =  pi + atan ( dy / dx );
                  } else {
                    if ( dy > 0 )
                       alpha = pi/2;
                    if ( dy < 0 )
                       alpha = 3*pi/2;
                    if ( dy  == 0 ) {
                       dest->pix[15][15] = sorc->pix[15][15];
                       break;
                    }   
                  }
               alpha -= pi/4;
               if ( alpha < 0  )
                  alpha+= pi*2;
       
               double ny;
               double nx;
               if ( alpha < pi/2 ) {
                  ny = radius * sin ( alpha );
                  nx = radius * cos ( alpha );
               } else
                 if ( alpha == pi/2 ) {
                    ny = radius;
                    nx = 0;
                 } else
                    if ( alpha < pi ) {
                       ny = radius * sin ( pi - alpha );
                       nx = -radius * cos ( pi - alpha );
                    } else
                       if ( alpha == pi/2 ) {
                          ny = radius;
                          nx = 0;
                       } else
                          if ( alpha < pi ) {
                             ny = radius * sin ( alpha );
                             nx = radius * cos ( alpha );
                          } else
                             if ( alpha == pi ) {
                                ny = 0;
                                nx = -radius;
                             } else
                                if ( alpha < 3*pi/2 ) {
                                   ny = -radius * sin ( alpha - pi );
                                   nx = -radius * cos ( alpha - pi );
                                } else
                                   if ( alpha == 3*pi/2 ) {
                                      ny = -radius;
                                      nx = 0;
                                   } else {
                                       ny = -radius * sin ( 2*pi - alpha  );
                                       nx = radius * cos ( 2*pi - alpha  );
                                   }
                if ( nx > 0 )
                   nx += 0.5;
                if ( nx < 0 )
                   nx -= 0.5;
                if ( ny > 0 )
                   ny += 0.5;
                if ( ny < 0 )
                   ny -= 0.5;
       
                int inx = (int)nx;
                int iny = (int)ny;
                if ( inx > 15 )
                   inx = 15;
                if ( inx < -15 )
                   inx = -15;
                if ( iny > 15 )
                   iny = 15;
                if ( iny < -15 )
                   iny = -15;
       
                dest->pix[ 15 + inx ][ 15 + iny ] = sorc->pix[ x][y];
            }
         } /* endfor */
      } /* endfor */
   }


   for ( int m = 0; m <2; m++ ) {
      
       int col[4];
       int pnum;
       for (int y = 0; y <= unitsizey; y++) 
          for (int x = 0; x <= unitsizex; x++) {
             if ( dest->pix[ x ][ y ] == 255 ) {
                pnum = 0;
                if ( y > 0 ) {
                   col[ pnum ] = dest->pix[ x ][ y-1 ];
                   if ( col[ pnum ] != 255 ) 
                      pnum++;
                 }
                if ( x > 0 ) {
                   col[ pnum ] = dest->pix[ x-1 ][ y ];
                   if ( col[ pnum ] != 255 ) 
                      pnum++;
                 }
                if ( y < unitsizey  ) {
                   col[ pnum ] = dest->pix[ x ][ y+1 ];
                   if ( col[ pnum ] != 255 ) 
                      pnum++;
                 }
                if ( x < unitsizex ) {
                   col[ pnum ] = dest->pix[ x+1 ][ y ];
                   if ( col[ pnum ] != 255 ) 
                      pnum++;
                 }
                 /* if (pnum == 1)
                    buffer[ getbufpos( x, y ) ] = col[0];
                 else
                    if (pnum == 2) 
                       dest->pix[ x ][ y ] = mix2colors ( col[0], col[1] );
                    else 
                       */
                      if (pnum == 3) 
                         dest->pix[ x ][ y ] = mix3colors ( col[0], col[1], col[2] );
                      else 
                         if (pnum == 4) 
                            dest->pix[ x ][ y ]= mix4colors ( col[0], col[1], col[2], col[3]  );
             }
          } /* endfor */
       if ( m == 0 ) 
          dest->pix[ 17 ][ 17 ] = dest->pix[ 16 ][ 16 ];

   }

}

union tpix {
  struct { char r,g,b,a; } s;
  int all;
};

typedef tpix timage[ fieldxsize ][ fieldysize ];

int getimagepixel ( void* image, int x, int y )
{
   int xs, ys;
   getpicsize ( image, xs, ys );


   y += ys/2;
   x += xs/2;
   if ( x < 0  || x >= xs || y < 0 || y >= ys )
      return -1;
   else {
      char* pc = (char*) image;
      return pc[ 4 + y * xs + x];
   }
}


char* rotatepict ( void* image, int organgle )
{
   float angle = ((float)organgle) / 360 * 2 * pi + pi;

   char* dst = new char[ imagesize ( 0, 0, fieldxsize, fieldysize ) ];
   dst[0] = fieldxsize-1;   
   dst[1] = 0;

   dst[2] = fieldysize-1;
   dst[3] = 0;
  
   char* pnt  = dst + 4;

   for ( int y = 0; y < fieldysize; y++ ) {
      for ( int x = 0; x < fieldxsize; x++ ) {
         int dx = x - fieldxsize/2 ;
         int dy = fieldysize/2 - y;
         float nx, ny;
         if ( organgle != 0 && organgle != -180 && organgle != 180) {
            float wnk ;
            if ( dx  ) 
               wnk = atan2 ( dy, dx );
            else
               if ( dy > 0 )
                  wnk = pi/2;
               else
                  wnk = -pi/2;
   
            wnk -= angle;
            float radius = sqrt ( dx * dx + dy * dy );
   
            nx = radius * cos ( wnk );
            ny = radius * sin ( wnk );
         } else 
            if ( organgle == 0 ) {
               nx = -dx;
               ny = -dy;
            } else
               if ( organgle == 180 || organgle == -180) {
                  nx = dx;
                  ny = dy;
               }
         

         int newpix = getimagepixel ( image, (int)-nx, (int)ny );
         if ( newpix == -1 )
            *pnt = 255;
         else
            *pnt = newpix;

         pnt++;
      }
   }

   return dst;
}


#endif



const int building_version = 3;
const int object_version = 1;
const int terrain_version = 2;
const int technology_version = 1;

void* leergui = NULL;
void* removegui = NULL;


void* generate_vehicle_gui_build_icon ( pvehicletype tnk )
{

   int wd;
   int hg;
   getpicsize ( leergui, wd, hg );

   int minx = 1000;
   int miny = 1000;
   int maxx = 0;
   int maxy = 0;

   tvirtualdisplay vdsp ( 500, 500 );

    bar ( 0, 0, 450, 450, 255 );
  

    minx = 0;
    miny = 0;
    putspriteimage ( 0, 0, tnk->picture[0] );
    maxx= fieldxsize;
    maxy= fieldysize;

   int sze = imagesize ( minx, miny, maxx, maxy );
   void* buf = new char [ sze ];
   getimage ( minx, miny, maxx, maxy, buf );

   while ( (maxx - minx + 1 > wd ) || ( maxy - miny + 1 > hg )) {
      void* temp = halfpict ( buf );

      char* dst = (char*) buf;
      char* src = (char*) temp;

      for ( int i = 0; i < sze; i++ )
         *(dst++) = *(src++);

      minx = 0;
      miny = 0;
      getpicsize ( buf, maxx, maxy );

   } /* endwhile */

   putimage ( 0, 0, leergui );
   putspriteimage ( (wd - maxx) / 2, (hg - maxy) / 2, buf );

   char* newbuildingpic = new char [ imagesize ( 0, 0, wd-1, hg-1 ) ];
   getimage ( 0, 0, wd-1, hg-1, newbuildingpic );
   delete[] buf;

   return newbuildingpic;
}



pvehicletype   loadvehicletype( char* name)
{
   displayLogMessage ( 5, " loading vehicle type %s ...", name );
   tnfilestream stream ( name, 1 );
   pvehicletype vt = loadvehicletype ( stream );
   displayLogMessage ( 5, " done\n");
   return vt;
}


pvehicletype   loadvehicletype( tnstream& stream )
{
   pvehicletype veh = new Vehicletype;
   veh->read( stream );
   return veh;
}





ptechnology       loadtechnology(char *       name)
{
   displayLogMessage ( 5, " loading technology %s ...", name );
   tnfilestream stream ( name, 1 );
   ptechnology t = loadtechnology ( &stream );
   displayLogMessage ( 5, " done\n");
   return t;
}


ptechnology       loadtechnology( pnstream stream )
{ 
   int          w;

   int version;
   stream->readdata2 ( version );
   if ( version == technology_version ) {

      ptechnology pt =  new ttechnology;
   
      stream->readdata ( pt, sizeof(*pt) ); 
      if ( pt->name )
         stream->readpchar( &pt->name );
      if ( pt->infotext )
         stream->readpchar( &pt->infotext );
      if ( pt->icon )
         stream->readrlepict ( &pt->icon,false,&w);
      if ( pt->pictfilename ) {
         stream->readpchar( &pt->pictfilename );

         char* pc = pt->pictfilename;
         while ( *pc ) {
            *pc = tolower( *pc );
            pc++;
         }
      }
   
      pt->lvl = -1;

      return pt; 
   } else
      return NULL;
} 

void writetechnology ( ptechnology tech, pnstream stream )
{
   stream->writeInt ( technology_version );

   stream->writedata2 ( *tech );
 
   if (tech->name)
      stream->writepchar( tech->name );
   if (tech->infotext)
      stream->writepchar( tech->infotext );
   if (tech->icon)
      stream->writerlepict( tech->icon );
   if ( tech->pictfilename )
      stream->writepchar( tech->pictfilename );
} 






void* generate_building_gui_build_icon ( pbuildingtype bld )
{

   int wd;
   int hg;
   getpicsize ( leergui, wd, hg );

   int minx = 1000;
   int miny = 1000;
   int maxx = 0;
   int maxy = 0;

   tvirtualdisplay vdsp ( 500, 500 );

    bar ( 0, 0, 450, 450, 255 );
  
    for (int y = 0; y <= 5; y++)
       for (int x = 0; x <= 3; x++)
          if (bld->getpicture(x,y) ) {
             int xp = fielddistx * x  + fielddisthalfx * ( y & 1);
             int yp = fielddisty * y ;
             if ( xp < minx )
                minx = xp;
             if ( yp < miny )
                miny = yp;
             if ( xp > maxx )
                maxx = xp;
             if ( yp > maxy )
                maxy = yp;

             putspriteimage ( xp, yp, bld->getpicture(x,y) );
          }
   maxx += fieldxsize;
   maxy += fieldysize;

   int sze = imagesize ( minx, miny, maxx, maxy );
   char* buf = new char [ sze ];
   getimage ( minx, miny, maxx, maxy, buf );

/*
   while ( (maxx - minx + 1 > wd ) || ( maxy - miny + 1 > hg )) {
      void* temp = halfpict ( buf );

      char* dst = (char*) buf;
      char* src = (char*) temp;

      for ( int i = 0; i < sze; i++ )
         *(dst++) = *(src++);

      minx = 0;
      miny = 0;
      getpicsize ( buf, maxx, maxy );

   } 
*/
   TrueColorImage* img = zoomimage ( buf, wd, hg, pal, 1 );
   delete[] buf;

   buf = convertimage ( img, pal );
   delete img;
   getpicsize ( buf, maxx, maxy );

   putimage ( 0, 0, leergui );
   putspriteimage ( (wd - maxx) / 2, (hg - maxy) / 2, buf );

   char* newbuildingpic = new char [ imagesize ( 0, 0, wd-1, hg-1 ) ];
   getimage ( 0, 0, wd-1, hg-1, newbuildingpic );
   delete[] buf;

   return newbuildingpic;


}



void* generate_object_gui_build_icon ( pobjecttype obj, int remove )
{

   int wd;
   int hg;
   getpicsize ( leergui, wd, hg );

   int minx = 1000;
   int miny = 1000;
   int maxx = 0;
   int maxy = 0;

   tvirtualdisplay vdsp ( 500, 500 );

    bar ( 0, 0, 450, 450, 255 );
  

    minx = 0;
    miny = 0;
    obj->display( 0, 0 );
    maxx= fieldxsize;
    maxy= fieldysize;

   int sze = imagesize ( minx, miny, maxx, maxy );
   void* buf = new char [ sze ];
   getimage ( minx, miny, maxx, maxy, buf );

   while ( (maxx - minx + 1 > wd ) || ( maxy - miny + 1 > hg )) {
      void* temp = halfpict ( buf );

      char* dst = (char*) buf;
      char* src = (char*) temp;

      for ( int i = 0; i < sze; i++ )
         *(dst++) = *(src++);

      minx = 0;
      miny = 0;
      getpicsize ( buf, maxx, maxy );

   } /* endwhile */

   putimage ( 0, 0, leergui );
   putspriteimage ( (wd - maxx) / 2, (hg - maxy) / 2, buf );

   if ( remove )
     putspriteimage ( (wd-18)/2, (hg-18)/2, removegui );

   void* newbuildingpic = new char [ imagesize ( 0, 0, wd-1, hg-1 ) ];
   getimage ( 0, 0, wd-1, hg-1, newbuildingpic );
   delete[] buf;

   return newbuildingpic;
}

void loadguipictures( void )
{
   if ( !leergui ) {
      tnfilestream stream ( "leergui.raw", 1 );
      int sze;
      stream.readrlepict ( &leergui, 0, &sze );
   }
   if ( !removegui ) {
      tnfilestream stream ( "guiremov.raw", 1 );
      int sze;
      stream.readrlepict ( &removegui, 0, &sze );
   }
}


pbuildingtype       loadbuildingtype(char *       name)
{
   displayLogMessage ( 5, " loading building type %s ...", name );
   tnfilestream stream ( name, 1 );
   pbuildingtype bt = loadbuildingtype ( &stream );
   displayLogMessage ( 5, " done\n");
   return bt;
}


pbuildingtype       loadbuildingtype( pnstream stream )
{ 
   int v, w, x, y;

   int version;
   stream->readdata2 ( version );
   if ( version <= building_version && version >= 1) {

      pbuildingtype pgbt = new Buildingtype;
   
      for ( v = 0; v < cwettertypennum; v++ )
         for ( w = 0; w < maxbuildingpicnum; w++ )
            for ( x = 0; x < 4; x++ )
               for ( y = 0; y < 6 ; y++ ) 
                   pgbt->w_picture[v][w][x][y] = (void*)stream->readInt( );

      for ( v = 0; v < cwettertypennum; v++ )
         for ( w = 0; w < maxbuildingpicnum; w++ )
            for ( x = 0; x < 4; x++ )
               for ( y = 0; y < 6 ; y++ ) 
                   pgbt->bi_picture[v][w][x][y] = stream->readInt( );
               
      pgbt->entry.x = stream->readInt( );
      pgbt->entry.y = stream->readInt( );
      pgbt->powerlineconnect.x = stream->readInt( );
      pgbt->powerlineconnect.y = stream->readInt( );
      pgbt->pipelineconnect.x = stream->readInt( );
      pgbt->pipelineconnect.y = stream->readInt( );

      pgbt->id = stream->readInt( );
      pgbt->name = (char*) stream->readInt( );
      pgbt->_armor = stream->readInt( );
      pgbt->jamming = stream->readInt( );
      pgbt->view = stream->readInt( );
      pgbt->loadcapacity = stream->readInt( );
      pgbt->loadcapability = stream->readChar( );
      pgbt->unitheightreq = stream->readChar( );
      pgbt->productionCost.material = stream->readInt( );
      pgbt->productionCost.fuel = stream->readInt( );
      pgbt->special = stream->readInt( );
      pgbt->technologylevel = stream->readChar( );
      pgbt->researchid = stream->readChar( );

      pgbt->terrainaccess.read ( *stream );

      pgbt->construction_steps = stream->readInt( );
      pgbt->maxresearchpoints = stream->readInt( );
      pgbt->_tank.energy = stream->readInt( );
      pgbt->_tank.material = stream->readInt( );
      pgbt->_tank.fuel = stream->readInt( );
      pgbt->maxplus.energy = stream->readInt( );
      pgbt->maxplus.material = stream->readInt( );
      pgbt->maxplus.fuel = stream->readInt( );
      pgbt->efficiencyfuel = stream->readInt( );
      pgbt->efficiencymaterial = stream->readInt( );
      pgbt->guibuildicon = (char*) stream->readInt( );
      pgbt->terrain_access = (pterrainaccess) stream->readInt( );

      pgbt->_bi_maxstorage.energy = stream->readInt( );
      pgbt->_bi_maxstorage.material = stream->readInt( );
      pgbt->_bi_maxstorage.fuel = stream->readInt( );

      pgbt->buildingheight = stream->readInt( );
      pgbt->buildingheight = 1 << log2 (pgbt->buildingheight); // to make sure just one bit is set
      pgbt->unitheight_forbidden = stream->readInt( );
      pgbt->externalloadheight = stream->readInt( );

      if ( version >= 3)
         pgbt->vehicleCategoriesLoadable = stream->readInt();
      else
         pgbt->vehicleCategoriesLoadable = -1;


      if ( version >= 2 ) {
         for ( x = 0; x < 4; x++ )
            for ( y = 0; y < 6; y++ )
                pgbt->destruction_objects[x][y] = stream->readInt( );
      } else {
         for ( w = 0; w < 9; w++ )
             stream->readInt( );     // dummy

         for ( x = 0; x < 4; x++ )
            for ( y = 0; y < 6; y++ )
                pgbt->destruction_objects[x][y] = 0;
      }

      if ( pgbt->name )
         stream->readpchar ( &pgbt->name );
   
      for ( int k = 0; k < maxbuildingpicnum ; k++)
         for ( int j = 0; j <= 5; j++) 
            for ( int i = 0; i <= 3; i++) 
               for ( int w = 0; w < cwettertypennum; w++ )
                 if ( pgbt->w_picture[w][k][i][j] ) 
                    if ( pgbt->bi_picture[w][k][i][j] == -1 ) {
                       int sz;
                       stream->readrlepict ( &pgbt->w_picture[w][k][i][j], false, &sz ); 
                     } else 
                        loadbi3pict_double ( pgbt->bi_picture[w][k][i][j],
                                                                                &pgbt->w_picture[w][k][i][j],
                                                                                CGameOptions::Instance()->bi3.interpolate.buildings );
                     
                  
       pgbt->terrain_access = &pgbt->terrainaccess; 
   
     #ifdef converter
      pgbt->guibuildicon = NULL;
     #else
      pgbt->guibuildicon = generate_building_gui_build_icon ( pgbt );
     #endif

      return pgbt; 
   } else
      return NULL;
} 


void writebuildingtype ( pbuildingtype bld, pnstream stream )
{
   int v,w,x,y;

   stream->writedata2 ( building_version );

   for ( v = 0; v < cwettertypennum; v++ )
      for ( w = 0; w < maxbuildingpicnum; w++ )
         for ( x = 0; x < 4; x++ )
            for ( y = 0; y < 6 ; y++ ) 
                stream->writeInt ( (int) bld->w_picture[v][w][x][y] );

   for ( v = 0; v < cwettertypennum; v++ )
      for ( w = 0; w < maxbuildingpicnum; w++ )
         for ( x = 0; x < 4; x++ )
            for ( y = 0; y < 6 ; y++ ) 
                stream->writeInt ( bld->bi_picture[v][w][x][y] );
            
   stream->writeInt ( bld->entry.x );
   stream->writeInt ( bld->entry.y );
   stream->writeInt ( bld->powerlineconnect.x );
   stream->writeInt ( bld->powerlineconnect.y );
   stream->writeInt ( bld->pipelineconnect.x );
   stream->writeInt ( bld->pipelineconnect.y );

   stream->writeInt ( bld->id );
   stream->writeInt ( (int) bld->name );
   stream->writeInt ( bld->_armor );
   stream->writeInt ( bld->jamming );
   stream->writeInt ( bld->view );
   stream->writeInt ( bld->loadcapacity );
   stream->writeChar ( bld->loadcapability );
   stream->writeChar ( bld->unitheightreq );
   stream->writeInt ( bld->productionCost.material );
   stream->writeInt ( bld->productionCost.fuel );
   stream->writeInt ( bld->special );
   stream->writeChar ( bld->technologylevel );
   stream->writeChar ( bld->researchid );

   bld->terrainaccess.write ( *stream );

   stream->writeInt ( bld->construction_steps );
   stream->writeInt ( bld->maxresearchpoints );
   stream->writeInt ( bld->_tank.energy );
   stream->writeInt ( bld->_tank.material );
   stream->writeInt ( bld->_tank.fuel );
   stream->writeInt ( bld->maxplus.energy );
   stream->writeInt ( bld->maxplus.material );
   stream->writeInt ( bld->maxplus.fuel );
   stream->writeInt ( bld->efficiencyfuel );
   stream->writeInt ( bld->efficiencymaterial );
   stream->writeInt ( (int) bld->guibuildicon );
   stream->writeInt ( (int) bld->terrain_access );

   stream->writeInt ( bld->_bi_maxstorage.energy );
   stream->writeInt ( bld->_bi_maxstorage.material );
   stream->writeInt ( bld->_bi_maxstorage.fuel );

   stream->writeInt ( bld->buildingheight );
   stream->writeInt ( bld->unitheight_forbidden );
   stream->writeInt ( bld->externalloadheight );

   stream->writeInt ( bld->vehicleCategoriesLoadable );

   for ( x = 0; x < 4; x++ )
      for ( y = 0; y < 6; y++ )
          stream->writeInt ( bld->destruction_objects[x][y] );

   if ( bld->name )
      stream->writepchar ( bld->name );

    for (int k = 0; k < maxbuildingpicnum; k++)
       for (int j = 0; j <= 5; j++)
          for (int i = 0; i <= 3; i++)
             for ( int w = 0; w < cwettertypennum; w++ )
                if (bld->w_picture[w][k][i][j] ) 
                   if ( bld->bi_picture[w][k][i][j] == -1 )
                       stream->writedata( bld->w_picture[w][k][i][j],fieldsize);
}


void generatedirecpict ( void* orgpict, void* direcpict )
{
  int t, u;

   tvirtualdisplay vfb ( 100, 100 );
   putspriteimage ( 10, 10, orgpict );

   char *b = (char*) direcpict;

   for (t = 1; t < 20; t++) {
       for (u = 20-t; u < 20+t; u++) {
          *b = getpixel(  10 + u, 9 + t);
          b++;
       }
    }
    
    for (t = 20; t > 0; t-- ) {
       for (u =20-t; u<= 19 + t; u++) {
          *b = getpixel(  10 + u, 10 + 39 - t );
          b++;
       }
    }

}



extern dacpalette256 pal;

#define sqr(a) (a)*(a)


void generateaveragecolprt ( int x1, int y1, int x2, int y2, void* buf, char* pix1 )
{
   word *w = (word*) buf;
   // int size = ( w[0] + 1) * (w[1] + 1);


   char *c ; // = (char*) buf;
             // c+=4;

   int pixnum = 0;

   int i,j;
   int r=0, g=0, b=0;
   for (j=y1; j< y2 ; j++ ) {
       for (i=x1; i< x2 ; i++ ) {
          c = (char*) buf + j * ( w[0] + 1) + i + 4;
          if (*c < 255) {
             r+=pal[*c][0];
             g+=pal[*c][1];
             b+=pal[*c][2];
             pixnum ++;
          }
      } /* endfor */
   } /* endfor */

   if (pixnum) {
      int r1 = r / pixnum;
      int g1 = g / pixnum;
      int b1 = b / pixnum;
   
   
      int diff = 0xFFFFFFF;
      int actdif;

   
      for (i=0;i<256 ;i++ ) {
         actdif = sqr( pal[i][0] - r1 ) + sqr( pal[i][1] - g1 ) + sqr( pal[i][2] - b1 );
         if (actdif < diff) {
            diff = actdif;
            *pix1 = i;
         }
      } 
   
/*
      int sml = ( r1 >> 2) + (( g1 >> 2) << 6) + (( b1 >> 2) << 12);
      *pix1 = truecolor2pal_table[sml];
*/
   
   } else {
      *pix1 = 255;
   }
}



pquickview generateaveragecol ( pwterraintype bdn )
{
   pquickview qv;
   qv = new ( tquickview );
   for ( int dir = 0; dir < sidenum; dir++ ) 
      if ( bdn->picture[dir] ) {

         char* c = (char*) &qv->dir[dir].p1 ;
         // int dx,dy;

         for ( int i=1; i<6 ;i+=2 ) {
            // dx = fieldxsize / i;
            // dy = fieldysize / i;
            for ( int k=0;k<i ; k++) {
               for ( int j=0 ; j<i ; j++) {
                   generateaveragecolprt ( k * fieldxsize / i, j * fieldysize / i, (k+1) * fieldxsize / i, (j+1) * fieldysize / i, bdn->picture[dir],  c );
                   c++;
               } /* endfor */
            } /* endfor */
         } /* endfor */
      }

   return qv;
}


pterraintype      loadterraintype(char *       name)
{
   displayLogMessage ( 5, " loading terrain type %s ...", name );
   tnfilestream stream ( name, 1 );
   pterraintype tt = loadterraintype ( &stream );
   displayLogMessage ( 5, " done\n" );
   return tt;
}

pterraintype      loadterraintype( pnstream stream )
{ 
   int version;
   stream->readdata2 ( version );
   if ( version == terrain_version || version == 1) {

      pwterraintype pgbt; 

      pterraintype bbt = new (tterraintype);
   
      bbt->name = (char*) stream->readInt();
      bbt->id   = stream->readInt();
      for ( int ww = 0; ww < cwettertypennum; ww++ )
         bbt->weather[ww] = (pwterraintype) stream->readInt();
      for ( int nf = 0; nf < 8; nf++ )
         bbt->neighbouringfield[nf] = stream->readInt();

      stream->readpchar( &bbt->name );
   
      for ( int i=0; i<cwettertypennum ;i++ ) {
         if (bbt->weather[i] ) {
            bbt->weather[i] = new ( twterraintype );
            pgbt = bbt->weather[i];

            int j;

            for ( j = 0; j < 8; j++ )
               pgbt->picture[j] = (void*) stream->readInt();

            for ( j = 0; j < 8; j++ )
               pgbt->direcpict[j] = (void*) stream->readInt();

            if ( version == 1 ) {
               stream->readInt(); //dummy1
               pgbt->defensebonus = stream->readWord();
               pgbt->attackbonus = stream->readWord();
               pgbt->basicjamming = stream->readChar();
            } else {
               pgbt->defensebonus = stream->readInt();
               pgbt->attackbonus = stream->readInt();
               pgbt->basicjamming = stream->readInt();
            }
            pgbt->movemaluscount = stream->readChar();
            pgbt->movemalus = (char*) stream->readInt();
            pgbt->terraintype = (pterraintype) stream->readInt();
            pgbt->quickview = (pquickview) stream->readInt();

            for ( j = 0; j < 6; j++ )
               pgbt->bi_picture[j] = stream->readInt();

            stream->readdata2 ( pgbt->art );

            #ifndef converter
             char mmcount = cmovemalitypenum;
             if (mmcount < pgbt->movemaluscount )
                mmcount = pgbt->movemaluscount;
            #else
             char mmcount = pgbt->movemaluscount ;
            #endif
   
            bbt->weather[i]->movemalus = new ( char[mmcount ]);
   
            for (j=0; j< mmcount ; j++ ) {
               if (j < pgbt->movemaluscount)
                  stream->readdata ( pgbt->movemalus+j, 1);
               else
                  pgbt->movemalus[j] = pgbt->movemalus[0];
   
               if ( pgbt->movemalus[j] == 0) {
                  if (j == 0)
                     pgbt->movemalus[j] = minmalq;
                  else
                     pgbt->movemalus[j] = pgbt->movemalus[0];
               }
            }
            pgbt->movemaluscount = mmcount;
   
   
            for ( j=0; j<8 ;j++ ) 
               if ( pgbt->picture[j] ) 
                  if ( pgbt->bi_picture[j] == -1 ) {
                     pgbt->picture[j] = asc_malloc ( fieldsize );
                     stream->readdata ( pgbt->picture[j], fieldsize );
                   } else 
                      loadbi3pict_double ( pgbt->bi_picture[j], 
                                                                                        &pgbt->picture[j], 
                                                                                        CGameOptions::Instance()->bi3.interpolate.terrain );
   
            pgbt->terraintype = bbt;
            if ( pgbt->quickview ) {
               pgbt->quickview = new ( tquickview );
               stream->readdata ( pgbt->quickview, sizeof ( *pgbt->quickview ));
            } 
            #ifndef converter
            else
               pgbt->quickview = generateaveragecol ( pgbt );
            #endif
         } else 
            bbt->weather[i] = NULL;
   
      } /* endfor */
   
      return bbt; 
   } else
      return NULL;
} 


void writeterrain ( pterraintype bdt, pnstream stream )
{
   int m;

   stream->writedata2 ( terrain_version );
   stream->writeInt ( int(bdt->name) );
   stream->writeInt ( bdt->id );
   for ( m = 0; m < cwettertypennum; m++ )
      stream->writeInt ( int( bdt->weather));

   for ( m = 0; m < 8; m++ )
      stream->writeInt ( bdt->neighbouringfield[m] );

   stream->writepchar( bdt->name );
   for (int i=0;i<cwettertypennum ;i++ ) {
     if ( bdt->weather[i] ) {
        for ( m = 0; m < 8; m++ )
           stream->writeInt ( int( bdt->weather[i]->picture[m] ));

        for ( m = 0; m < 8; m++ )
           stream->writeInt ( int( bdt->weather[i]->direcpict[m] ));

        stream->writeInt ( bdt->weather[i]->defensebonus );
        stream->writeInt ( bdt->weather[i]->attackbonus );
        stream->writeInt ( bdt->weather[i]->basicjamming );
        stream->writeChar ( bdt->weather[i]->movemaluscount );
        stream->writeInt ( int( bdt->weather[i]->movemalus ));
        stream->writeInt ( int( bdt->weather[i]->terraintype ));
        stream->writeInt ( int( bdt->weather[i]->quickview ));
        stream->writedata2 ( bdt->weather[i]->art );
         
        for ( m = 0; m< 6; m++ )
           stream->writeInt ( bdt->weather[i]->bi_picture[6] );

        stream->writedata ( bdt->weather[i]->movemalus, bdt->weather[i]->movemaluscount );

        for ( int j = 0; j < 8; j++ ) 
           if ( bdt->weather[i]->picture[j] && bdt->weather[i]->bi_picture[j] == -1 ) 
              stream->writedata ( ( char*) bdt->weather[i]->picture[j], fieldsize );
           
        
        if ( bdt->weather[i]->quickview )
           stream->writedata ( ( char*) bdt->weather[i]->quickview, sizeof ( *bdt->weather[i]->quickview ));
     }
   }
}



pobjecttype streetobjectcontainer = NULL;
pobjecttype pathobject = NULL;
pobjecttype railroadobject = NULL;
pobjecttype runwayobject = NULL;
pobjecttype eisbrecherobject = NULL;
pobjecttype fahrspurobject = NULL;



pobjecttype   loadobjecttype(char *       name)
{
   displayLogMessage ( 5, " loading object type %s ...", name );
   tnfilestream stream ( name, 1 );
   pobjecttype ot = loadobjecttype ( &stream );
   displayLogMessage ( 5, " done\n" );
   return ot;
/*
   pobjecttype fztn = loadobjecttype ( &stream );
   for ( int i = 0 ; i < cmovemalitypenum; i++) {
       if ( fztn->movemalus_abs[i] > 0  && fztn->movemalus_abs[i] < 10 )
          fprintf(stderr, "The object %s has invalid movemali\n", name );
   }
   return fztn;
*/
}



pobjecttype   loadobjecttype( pnstream stream )
{
   pobjecttype fztn = new tobjecttype;
   fztn->read ( *stream );

   if ( fztn->id == 9 )
      pathobject = fztn;

   if ( fztn->id == 1 )
      streetobjectcontainer = fztn;

   if ( fztn->id == 2 )
      railroadobject = fztn;

   if ( fztn->id == 6 )
      eisbrecherobject = fztn;

   if ( fztn->id == 7 )
      fahrspurobject = fztn;

   #ifndef converter
    fztn->buildicon = generate_object_gui_build_icon ( fztn, 0 );
    fztn->removeicon = generate_object_gui_build_icon ( fztn, 1 );
   #else
    fztn->buildicon = NULL;
    fztn->removeicon = NULL;
   #endif
   
    return fztn;
}

void writeobject ( pobjecttype object, pnstream stream, int compressed )
{
   object->write ( *stream );
}



void loadpalette ( void )
{
   if ( ! asc_paletteloaded ) {
      displayLogMessage ( 4, "loading palette ... " );

      tnfilestream stream ("palette.pal",1);
      stream.readdata( & pal, sizeof(pal));
      colormixbuf = (pmixbuf) new char [ sizeof ( tmixbuf ) ];
      stream.readdata( colormixbuf,  sizeof ( *colormixbuf ));

      stream.readdata( &xlattables.a.dark05, sizeof ( xlattables.a.dark05 ));
      xlattables.a.dark05[255] = 255;

      stream.readdata( &xlattables.a.dark1,  sizeof ( xlattables.a.dark1 ));
      xlattables.a.dark1[255] = 255;

      stream.readdata( &xlattables.a.dark2,  sizeof ( xlattables.a.dark2 ));
      xlattables.a.dark2[255] = 255;

      stream.readdata( &xlattables.a.dark3,  sizeof ( xlattables.a.dark3 ));
      xlattables.a.dark3[255] = 255;

      stream.readdata( &xlattables.light,  sizeof ( xlattables.light ));
      xlattables.light[255] = 255;

      stream.readdata( &truecolor2pal_table,  sizeof ( truecolor2pal_table ));
      #ifdef HEXAGON
      stream.readdata( &bi2asc_color_translation_table,  sizeof ( bi2asc_color_translation_table ));
      #endif

      xlatpictgraytable = (ppixelxlattable) asc_malloc( sizeof(*xlatpictgraytable) );
      // xlatpictgraytable = new tpixelxlattable;
      generategrayxlattable(xlatpictgraytable,160,16,&pal);

      (*xlatpictgraytable)[255] = 255;

      asc_paletteloaded = 1;
      displayLogMessage ( 4, "done\n" );
   }

}



t_carefor_containerstream :: t_carefor_containerstream ( void ) 
{ 
   opencontainer ( "*.con" );
}


bool makeDirectory ( const char* path )
{
   char tmp[10000];
   constructFileName( tmp, 0, path, NULL );

   int existence = directoryExist ( tmp );

   if ( !existence ) {
      int res = createDirectory( tmp ); 
      if ( res ) {
         fprintf(stderr, "could neither access nor create directory %s\n", tmp );
         return false;
      }
   }

   return true;
}


// CLoadableGameOptions* loadableGameOptions = NULL;

char* configFileNameUsed = NULL;
char* configFileNameToWrite = NULL;

char* getConfigFileName ( char* buffer )
{
   if ( buffer ) {
      if ( configFileNameUsed )
         strcpy ( buffer, configFileNameUsed );
      else
         strcpy ( buffer, "-none- ; default values used" );
   }
   return buffer;
}


CLoadableGameOptions* loadableGameOptions =     NULL;

int readgameoptions ( const char* filename )
{
   displayLogMessage ( 4, "loading game options ... " );

   const char* fn;
   if ( filename )
      fn = filename;
   else
      if ( getenv ( asc_EnvironmentName )) 
         fn = getenv ( asc_EnvironmentName );
      else
         fn = asc_configurationfile;

   char completeFileName[10000];
   constructFileName ( completeFileName, -3, NULL, fn );

   configFileNameToWrite = strdup ( completeFileName );

   if ( exist ( completeFileName )) {
      configFileNameUsed = strdup ( completeFileName );

      if ( !loadableGameOptions )
         loadableGameOptions = new CLoadableGameOptions (CGameOptions::Instance());

      std::ifstream is( completeFileName );
      loadableGameOptions->Load(is);    
   } else {
      CGameOptions::Instance()->setChanged(); // to generate a configuration file
      if ( exist ( "sg.cfg" ) ) {
         tnfilestream stream ( "sg.cfg", 1);
         int version = stream.readInt ( );
         if ( version == 102 ) {
            CGameOptions::Instance()->fastmove = stream.readInt();
            CGameOptions::Instance()->visibility_calc_algo = stream.readInt();
            CGameOptions::Instance()->movespeed = stream.readInt();
            CGameOptions::Instance()->endturnquestion = stream.readInt();
            CGameOptions::Instance()->smallmapactive = stream.readInt();
            CGameOptions::Instance()->units_gray_after_move = stream.readInt();
            CGameOptions::Instance()->mapzoom = stream.readInt();
            CGameOptions::Instance()->mapzoomeditor = stream.readInt();
            CGameOptions::Instance()->startupcount = stream.readInt();
            CGameOptions::Instance()->dontMarkFieldsNotAccessible_movement = stream.readInt();
            CGameOptions::Instance()->attackspeed1 = stream.readInt();
            CGameOptions::Instance()->attackspeed2 = stream.readInt();
            CGameOptions::Instance()->attackspeed3 = stream.readInt();
            CGameOptions::Instance()->disablesound = stream.readInt();
            for ( int i = 0; i < 9; i++ )
               stream.readInt();  // dummy

            CGameOptions::Instance()->mouse.scrollbutton = stream.readInt();
            CGameOptions::Instance()->mouse.fieldmarkbutton = stream.readInt();
            CGameOptions::Instance()->mouse.smallguibutton = stream.readInt();
            CGameOptions::Instance()->mouse.largeguibutton = stream.readInt();
            CGameOptions::Instance()->mouse.smalliconundermouse = stream.readInt();
            CGameOptions::Instance()->mouse.centerbutton = stream.readInt();
            CGameOptions::Instance()->mouse.unitweaponinfo = stream.readInt();
            CGameOptions::Instance()->mouse.dragndropmovement = stream.readInt();
            for ( int j = 0; j < 7; j++ )
               stream.readInt();

            CGameOptions::Instance()->container.autoproduceammunition = stream.readInt();
            CGameOptions::Instance()->container.filleverything = stream.readInt();
            CGameOptions::Instance()->container.emptyeverything = stream.readInt();
            for ( int k = 0; k < 10; k++ )
               stream.readInt();

            CGameOptions::Instance()->onlinehelptime = stream.readInt();
            CGameOptions::Instance()->smallguiiconopenaftermove = stream.readInt();
            CGameOptions::Instance()->defaultPassword.setName ( strrr ( stream.readInt() ));
            CGameOptions::Instance()->replayspeed = stream.readInt();
            int bi3dir = stream.readInt();
            CGameOptions::Instance()->bi3.interpolate.terrain = stream.readInt();
            CGameOptions::Instance()->bi3.interpolate.units = stream.readInt();
            CGameOptions::Instance()->bi3.interpolate.objects = stream.readInt();
            CGameOptions::Instance()->bi3.interpolate.buildings = stream.readInt();

            if ( bi3dir ) {
               char* tmp;
               stream.readpchar ( &tmp );
               CGameOptions::Instance()->bi3.dir.setName( tmp );
               delete[] tmp;
            }

         }
      }
   }
   #ifdef sgmain
   if ( CGameOptions::Instance()->startupcount < 10 ) {
      CGameOptions::Instance()->startupcount++;
      CGameOptions::Instance()->setChanged();
   }
   #endif

   makeDirectory ( CGameOptions::Instance()->getSearchPath(0) );

   displayLogMessage ( 4, "done\n" );
   return 0;
}

int writegameoptions ( void )
{
   if ( CGameOptions::Instance()->isChanged() && configFileNameToWrite ) {
      char buf[10000];
      if ( makeDirectory ( extractPath ( buf, configFileNameToWrite ))) {
         if ( !loadableGameOptions )
            loadableGameOptions = new CLoadableGameOptions (CGameOptions::Instance());
         std::ofstream os( configFileNameToWrite );
         loadableGameOptions->Save(os);
         return 1;
      }
   }
   return 0;
}

void checkFileLoadability ( const char* filename )
{
   try {
      tnfilestream strm ( filename, 1 );
      strm.readChar();
   }
   catch ( ASCexception ) {
      char temp[10000];
      temp[0] = 0;
      for ( int i = 0; i < 5; i++ )
         if ( CGameOptions::Instance()->getSearchPath(i)        ) {
            strcat ( temp, "  " );
            strcat ( temp, CGameOptions::Instance()->getSearchPath(i) );
            strcat ( temp, "\n" );
         }

      char temp3[1000];
      temp3[0] = 0;
      if ( !configFileNameUsed ) {
         CGameOptions::Instance()->setChanged();
         if ( writegameoptions())
            sprintf(temp3, "A configuration file has been written to %s\n", configFileNameToWrite );
      }

      char temp5[10000];
      char temp2[1000];
      sprintf ( temp5, "Unable to access %s\n"
                       "Make sure the data file 'main.con' is in one of the search paths specified\n"
                       "in your config file !\n"
                       "The configuration file that is used is: %s \n%s"
                       "These pathes are being searched:\n%s\n"
                       "If you don't have a file 'main.con' , get and install the data package from\n"
                       "http://www.asc-hq.org\n",
                       filename, getConfigFileName(temp2), temp3, temp );

     #ifndef converter
      displaymessage ( temp5, 2 );
     #else
      fprintf(stderr, temp5 );
      exit(1);
     #endif
   }
   catch ( ... ) {
      const char* msg = "checkFileLoadability threw an unspecified exception\n";
      #ifndef converter
       displaymessage ( msg, 2 );
      #else
       fprintf( stderr, msg );
       exit(1);
      #endif
   }
}

void initFileIO ( const char* configFileName )
{
   readgameoptions( configFileName );

   for ( int i = 0; i < CGameOptions::Instance()->getSearchPathNum(); i++ )
      if ( CGameOptions::Instance()->getSearchPath(i)   ) {
         displayLogMessage ( 3, "adding search patch %s\n", CGameOptions::Instance()->getSearchPath(i));
         addSearchPath ( CGameOptions::Instance()->getSearchPath(i) );
      }
   try {
     opencontainer ( "*.con" );
   }
   catch ( tfileerror err ) {
      const char* msg = "a fatal IO error occured while mounting the container file %s\n"
                        "It is probably damaged, try getting a new one.\n";
      #ifndef converter
       displaymessage ( msg, 2, err.filename );
      #else
       fprintf( stderr, msg, err.filename );
       exit(1);
      #endif
   }
   catch ( tcompressionerror err ) {
      const char* msg = "a fatal error occured while decompressing a container file.\n"
                        "If you have several *.con files in your ASC directory, try removing all but main.con.\n"
                        "If the error still occurs then, get a new data package from www.asc-hq.org\n";

      #ifndef converter
       displaymessage ( msg, 2 );
      #else
       fprintf( stderr, msg );
       exit(1);
      #endif
   }
   catch ( ASCexception err ) {
      const char* msg = "a fatal error occured while mounting the container files \n";
      #ifndef converter
       displaymessage ( msg, 2 );
      #else
       fprintf( stderr, msg );
       exit(1);
      #endif
   }
   catch ( ... ) {
       const char* msg = "loading of game failed during pre graphic initializing";
      #ifndef converter
       displaymessage ( msg, 2 );
      #else
       fprintf( stderr, msg );
       exit(1);
      #endif
   }

   checkFileLoadability ( "palette.pal" );
}


//===================================================================================


int SingleUnitSet :: isMember ( int id )
{
   for ( int i = 0; i < ids.size(); i++ )
     if ( id >= ids[i].from && id <= ids[i].to )
        return 1;
   return 0;
}

/*
         class IdRangeVector {
                public:
                   vector<IdRange> idRange;
                   void parseString ( const char* s );
         };
*/

void SingleUnitSet::parseIDs ( const char* s )
{
   char buf[10000];

   if ( s && s[0] ) {

      strcpy ( buf, s);

      char* piclist = strtok ( buf, ";\r\n" );

      char* pic = strtok ( piclist, "," );
      while ( pic ) {
         int from, to;
         if ( strchr ( pic, '-' )) {
            char* a = strchr ( pic, '-' );
            *a = 0;
            from = atoi ( pic );
            to = atoi ( ++a );
         } else
            from = to = atoi ( pic );

         IdRange ir;
         ir.from = from;
         ir.to = to;
         ids.push_back ( ir );

         pic = strtok ( NULL, "," );
      }
   }
}

void SingleUnitSet::TranslationTable::parseString ( const char* s )
{
   if ( s && s[0] && strchr ( s, ';' )) {
      char buf[10000];
      if ( s[0] == '#' )
         strcpy ( buf, s+1 );
      else
         strcpy ( buf, s );

      char* tname = strtok ( buf, ";\n\r");
      if ( tname )
         name = tname;

      char* xl = strtok ( NULL, ";\n\r" );
      while ( xl ) {
         if ( strchr ( xl, ',' )) {
            char* a = strchr ( xl, ',' );
            *a = 0;
            IdRange ir;
            ir.from = atoi ( xl );
            ir.to = atoi ( ++a );

            translation.push_back ( ir );

         }
         xl = strtok ( NULL, ";\n\r" );
      }

   }
}


void SingleUnitSet::read ( pnstream stream )
{
   if ( !stream )
      return;
   const char separator = '=';
   std::string s;
   int data = stream->readTextString ( s );
   if ( s == "#V2#" ) {
      while ( data ) {
         std::string s2;
         data = stream->readTextString ( s2 );

         int seppos = s2.find_first_of ( separator );
         if ( seppos >= 0 ) {
            std::string b = s2.substr(0, seppos);
            std::string e = s2.substr( seppos+1 );
            if ( b == "NAME" )
               name = e;

            if ( b == "ACTIVE" )
               active = atoi ( e.c_str() );


            if ( b == "TRANSFORMATION" ) {
               TranslationTable* tt = new TranslationTable;
               tt->parseString ( e.c_str() );
               transtab.push_back ( tt );
            }

            if ( b == "MAINTAINER" )
               maintainer = e;

            if ( b == "INFORMATION" )
               information = e;

            if ( b == "FILTERBUILDINGS" )
               filterBuildings = atoi ( e.c_str() );

            if ( b == "ID" )
               parseIDs ( e.c_str() );

         }
      }
   } else {
      int seppos = s.find_first_of ( ';' );
      if ( seppos >= 0 ) {
                  std::string b = s.substr(0, seppos);
                  std::string e = s.substr( seppos+1 );
         name = b;
         parseIDs ( e.c_str() );

         while ( data ) {
                         std::string s2;
            data = stream->readTextString ( s2 );
            if ( s2.length() ) {
               TranslationTable* tt = new TranslationTable;
               tt->parseString ( s2.c_str() );
               transtab.push_back ( tt );
            }
         }
      }
   }
}


void loadUnitSets ( void )
{
   displayLogMessage ( 4, "loading unit set definition files\n" );
   tfindfile ff ( "*.set" );
   char* n = ff.getnextname();
   while ( n ) {
      displayLogMessage ( 5, " loading unit set definition file %s ... ",n );
      tnfilestream stream ( n, 1 );

      SingleUnitSet* set = new SingleUnitSet;
      set->read ( &stream );
      unitSets.push_back ( set );

      n = ff.getnextname();
      displayLogMessage ( 5, "done\n" );
   } /* endwhile */
}

std::vector<SingleUnitSet*> unitSets;



void displayLogMessage ( int msgVerbosity, char* message, ... )
{
   va_list arglist;
   va_start ( arglist, message );
   if ( msgVerbosity <= verbosity ) {
      char buf[10000];
      vsprintf ( buf, message, arglist );

      fprintf ( stdout, buf );
      fflush ( stdout );
   }
   va_end ( arglist );
}