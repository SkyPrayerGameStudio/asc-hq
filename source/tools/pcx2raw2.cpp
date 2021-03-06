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
#include <dos.h> 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include <math.h>


#include "tpascal.inc"
#include "typen.h"
#include "vesa.h"
#include "misc.h"
#include "sgstream.h"
#include "stack.h"
#include "loadpcx.h"
#include "sgstream.h"
#include "mousehnd.h"
#include <conio.h>


dacpalette256 pal;

main(int argc, char *argv[], char *envp[])
{
   if ( argc <= 1 ) {
      printf("\nmissing parameter. filename expected.");
      return 1;
   }
      
   
   pavailablemodes avm = searchformode ( 800, 600, 8 );
   if (avm->num > 0) 
      initsvga( avm->mode[0].num );
   else {
      avm = searchformode ( 640, 480, 8 );
      if (avm->num > 0) 
         initsvga( avm->mode[0].num );
      else {
         printf("no vesa modes available !\n" );
         return 1;
      }

   }


   bar( 0, 0, agmp->resolutionx-1, agmp->resolutiony-1, 255 );
   loadpcxxy( argv[1], 1, 20, 20 );

   void* p = malloc ( 1000000 );

   tn_file_buf_stream mainstream ("actimage.raw",2);
      /*
      if ( mouseparams.taste == 1 ) {
        mousevisible(false);
        int x = mouseparams.x;
        int y = mouseparams.y;
        int x1 = x;
        while ( getpixel ( x1-1, y ) != 255 )
           x1--;

        int x2 = x;
        while ( getpixel ( x2+1, y ) != 255 )
           x2++;

        int y1 = y;
        while ( getpixel ( x, y1-1 ) != 255 )
           y1--;

        int y2 = y;
        while ( getpixel ( x, y2+1 ) != 255 )
           y2++;
        
        getimage( x1,y1,x2,y2, p );

        mainstream.writerlepict( p );

        rectangle( x1,y1,x2,y2, 14 );

        while ( mouseparams.taste );
      */
        getimage( 20,20,20+18,20+18, p );

        mainstream.writerlepict( p );

   getch();
   settextmode(3);
   return 0;

}
