/*! \file loadpcxc.cpp
    \brief Loading and storing images in PCX format, written in C

    There is also an optimized assembler version for the DOS version around
    which is called loadpcx.cpp , but not used any more.
*/

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

#include <stdio.h>
#include <cstring>

#include "basestrm.h"
// #include "basegfx.h"
#include "misc.h"
#include "loadpcx.h"

#pragma pack(1)

struct tpcxheader{
           Uint8     manufacturer;
           Uint8     version     ;
           Uint8     encoding    ;
           Uint8     bitsperpixel;
           Uint16     xmin,ymin   ;
           Uint16     xmax,ymax   ;
           Uint16     hdpi,vdpi   ;
           Uint8     colormap[48];
           Uint8     reserved    ;
           Uint8     nplanes     ;
           Uint16     bytesperline;
           Uint16     paletteinfo ;
           Uint16     hscreensize ;
           Uint16     vscreensize ;
           Uint8     dummy[50]   ;
           int      size;            // patch to be able to read pcx files without seeking

          void read  ( tnstream* stream );
          void write ( tnstream* stream );
       };

void tpcxheader::read ( tnstream* stream )
{
   manufacturer = stream->readUint8();
   version      = stream->readUint8();
   encoding     = stream->readUint8();
   bitsperpixel = stream->readUint8();
   xmin         = stream->readWord();
   ymin         = stream->readWord();
   xmax         = stream->readWord();
   ymax         = stream->readWord();
   hdpi         = stream->readWord();
   vdpi         = stream->readWord();
   stream->readdata ( colormap, 48);
   reserved     = stream->readUint8();
   nplanes      = stream->readUint8();
   bytesperline= stream->readWord();
   paletteinfo = stream->readWord();
   hscreensize = stream->readWord();
   vscreensize = stream->readWord();
   stream->readdata ( dummy, 50 ) ;
   size = stream->readInt();            // patch to be able to read pcx files without seeking
}

void tpcxheader::write ( tnstream* stream )
{
   stream->writeUint8( manufacturer );
   stream->writeUint8( version );
   stream->writeUint8( encoding );
   stream->writeUint8( bitsperpixel );
   stream->writeWord( xmin        );
   stream->writeWord( ymin        );
   stream->writeWord( xmax        );
   stream->writeWord( ymax        );
   stream->writeWord( hdpi        );
   stream->writeWord( vdpi        );
   stream->writedata ( colormap, 48);
   stream->writeUint8( reserved    );
   stream->writeUint8( nplanes     );
   stream->writeWord( bytesperline);
   stream->writeWord( paletteinfo );
   stream->writeWord( hscreensize );
   stream->writeWord( vscreensize );
   stream->writedata ( dummy, 50 ) ;
   stream->writeInt( size );            // patch to be able to read pcx files without seeking
}

#pragma pack()

#if 0
int pcxGetColorDepth ( const ASCString& filename, int* width, int* height )
{
   tnfilestream stream ( filename.c_str(), tnstream::reading );

   tpcxheader header;
   header.read ( &stream );

   if ( width )
      *width = header.xmax - header.xmin + 1 ;

   if ( height )
      *height = header.ymax - header.ymin + 1;


   return header.nplanes * header.bitsperpixel;
}


Uint8 loadpcxxy( pnstream stream, int x, int y, bool setpalette, int* xsize, int* ysize )
{
   int read = 0;

   // int m = y * agmp-> scanlinelength * agmp->byteperpix + x;

   tpcxheader header;

   header.read( stream );

   read += sizeof(header);

   if ( !header.size ) {
      if ( stream->getSize() < 0 )
         return 12;
      else
         header.size = stream->getSize();
   }

   int width = header.xmax - header.xmin + 1 ;
   int height = header.ymax - header.ymin + 1;
   int pixels = width * height * header.nplanes;

   if ( xsize )
      *xsize = width;

   if ( ysize )
      *ysize = height;

   if ( header.manufacturer != 10 ||
        header.bitsperpixel != 8  || 
        header.xmax-header.xmin > agmp-> scanlinelength )
      return 11;

   int colors   = header.nplanes * header.bitsperpixel;

   int scanlineret[4];

   if (agmp->byteperpix > 1) {
      
      scanlineret[0] = agmp-> redfieldposition / 8;
      scanlineret[1] = header.bytesperline * agmp->byteperpix - agmp->greenfieldposition / 8 + agmp-> redfieldposition / 8;
      scanlineret[2] = header.bytesperline * agmp->byteperpix - agmp->bluefieldposition / 8 + agmp-> greenfieldposition / 8;
      scanlineret[3] = -(agmp-> scanlinelength  -
                       header.bytesperline* agmp->byteperpix + agmp->redfieldposition / 8 - agmp->bluefieldposition / 8);
                      
   } else {
      scanlineret[0]=0;
      scanlineret[1]= -(agmp->scanlinelength - header.bytesperline * agmp->byteperpix);
   }

   // int totalbytes = header.nplanes * header.bytesperline;


   int ttlbytes = 0;
   int xpos = 0;
   int planenum = 0;

   int byteperpix = agmp->byteperpix;


   // some simple buffering
   Uint8* buf = new Uint8[header.size];
   // int bufdata =
   stream->readdata ( buf, header.size - sizeof( header ));
   int actpos = 0;


   Uint8* dest = (Uint8*) (agmp->linearaddress + x * agmp->byteperpix + y * agmp->bytesperscanline + scanlineret[0]);
   while ( pixels ) {
	   Uint8 a;

      a = buf[actpos++];

      read++;

      int count;
      if ( a >= 192 ) {
         count = a ^192;

         a = buf[actpos++];

         read++;
      } else
         count = 1;

      while ( count ) {
         if ( xpos >= header.bytesperline ) {
            planenum++;
            dest -= scanlineret[planenum];
            if ( planenum >= header.nplanes ) {
               planenum = 0;
               ttlbytes = 0;
            }
            xpos = 0;
         }
         if ( xpos < width ) {
            *dest = a;
            pixels--;
         }
         dest+= byteperpix;
         xpos++;
         ttlbytes++;
         count--;
      } /* endwhile */

   } /* endwhile */

   if ( setpalette && colors == 8 ) {
      if ( header.size ) {
         int dataend = header.size - sizeof ( dacpalette256 ) - 1 - sizeof ( header );
         actpos = dataend;
         /*
         while ( read < dataend ) {
            Uint8 scratch[100];
            int datalength;
            if ( 100 > dataend - read )
               datalength = dataend - read;
            else
               datalength = 100;
            stream->readdata ( scratch, datalength );
            read+= datalength;;
         }
         */
      }    

      Uint8 c = buf[actpos++];
      if ( c == 12 ) {
         dacpalette256 pal;
         memcpy ( pal, &buf[actpos], sizeof ( pal ));
         // stream->readdata ( pal, sizeof ( pal ) );
         for ( int i = 0; i < 3; i++ )
            for ( int j = 0; j < 256; j++ )
               pal[j][i] >>= 2;
         // setvgapalette256 ( pal );
      }
   }

   delete[] buf;
   return 0;
}


Uint8 loadpcxxy ( const ASCString& name, bool setpal, int xpos, int ypos, int* xsize, int* ysize )
{
   tnfilestream s ( name, tnstream::reading );
   return loadpcxxy ( &s, xpos, ypos, setpal, xsize, ysize );
}
#endif

void writepcx ( const ASCString& name, int x1, int y1, int x2, int y2, dacpalette256 pal )
{
   tpcxheader header;
   memset ( &header, 0, sizeof (header ));
   header.manufacturer = 10;
   header.version = 5;
   header.encoding = 1;
   header.bitsperpixel = 8;
   header.xmin = 0;
   header.xmax = x2 - x1 ;
   header.ymin = 0;
   header.ymax = y2 - y1 ;
   header.hdpi = 0;
   header.vdpi = 0;
   if ( agmp->byteperpix == 1 )
      header.nplanes = 1;
   else
      header.nplanes = 3;

//   header.bytesperline = ((header.xmax - header.xmin) + 1) & 0xFFFE;
   header.bytesperline = x2 - x1 + 1 ;
   if ( header.bytesperline & 1 )
      header.bytesperline++;
   header.paletteinfo = 1;

   int fsize = 0;

   tn_file_buf_stream stream ( name, tnstream::writing );

   header.write ( &stream );

   fsize += sizeof ( header );

   for ( int y = y1; y <= y2; y++ )
      for ( int plane = 0; plane < header.nplanes; plane++ ) {
         int lastbyte = -1;
         int count = 0;
         for ( int x = x1; x < x1 + header.bytesperline; x++ ) {
        	Uint8 c = (getpixel( x, y ) >> ( plane * 8 )) & 0xff;

            if ( (lastbyte == c && count < 63) || lastbyte == -1 ) {
               count ++;
               lastbyte = c;
            }
            else {
               if ( count > 1 || lastbyte >= 192 ) {
            	  Uint8 d = 192 + count;
                  stream.writeUint8 ( d );
                  fsize += sizeof ( d );
               }
               Uint8 lstbyte = lastbyte;
               stream.writeUint8 ( lstbyte );
               fsize += sizeof ( lstbyte );
               count = 1;
               lastbyte = c;
            }
            
         }
         if ( count > 1 || lastbyte >= 192 ) {
        	Uint8 d = 192 + count;
            stream.writeUint8 ( d );
            fsize += sizeof ( d );
         }
         Uint8 lstbyte = lastbyte;
         stream.writeUint8 ( lstbyte );
         fsize += sizeof ( lstbyte );
   
      }

   if ( header.nplanes == 1 ) {
      Uint8 d = 12;
      stream.writeUint8 ( d );
      fsize += sizeof ( d );

      dacpalette256 pal2;

      for ( int i = 0; i < 3; i++ )
          for ( int j = 0; j < 256; j++ )
              pal2[j][i] = pal[j][i] << 2;

      stream.writedata ( &pal2, 768 ); // endian ok !!!
      fsize += 768 ;
   }
   stream.seek ( 0 );
   header.size = fsize;
   header.write ( &stream );
}


void writepcx ( const ASCString& name, const Surface& s )
{
   writepcx( name, s, SDLmm::SRect( SPoint(0,0), s.w(), s.h() ));
}

void writepcx ( const ASCString& name, const Surface& s, const SDLmm::SRect& rect )
{
   int x1 = rect.x;
   int y1 = rect.y;
   int x2 = rect.x + rect.w - 1;
   int y2 = rect.y + rect.h - 1;
   tpcxheader header;
   memset ( &header, 0, sizeof (header ));
   header.manufacturer = 10;
   header.version = 5;
   header.encoding = 1;
   header.bitsperpixel = 8;
   header.xmin = 0;
   header.xmax = x2 - y1 ;
   header.ymin = 0 ;
   header.ymax = y2 - y1;
   header.hdpi = 0;
   header.vdpi = 0;
   if ( s.GetPixelFormat().BitsPerPixel() == 8 )
      header.nplanes = 1;
   else
      header.nplanes = 3;

//   header.bytesperline = ((header.xmax - header.xmin) + 1) & 0xFFFE;
   header.bytesperline = x2 - x1 + 1 ;
   if ( header.bytesperline & 1 )
      header.bytesperline++;
   header.paletteinfo = 1;

   int fsize = 0;

   tn_file_buf_stream stream ( name, tnstream::writing );

   header.write ( &stream );

   fsize += sizeof ( header );

   int shift[3] = { 0, 0, };
   if ( header.nplanes > 1 ) {
      shift[0] = s.GetPixelFormat().Rshift();
      shift[1] = s.GetPixelFormat().Gshift();
      shift[2] = s.GetPixelFormat().Bshift();
   }
   
   for ( int y = y1; y <= y2; y++ )
      for ( int plane = 0; plane < header.nplanes; plane++ ) {
         int lastbyte = -1;
         int count = 0;
         for ( int x = x1; x < x1 + header.bytesperline; x++ ) {
            Uint8 c = (s.GetPixel( x, y ) >> shift[plane]) & 0xff;
   
            if ( (lastbyte == c && count < 63) || lastbyte == -1 ) {
               count ++;
               lastbyte = c;
            }
            else {
               if ( count > 1 || lastbyte >= 192 ) {
            	  Uint8 d = 192 + count;
                  stream.writeUint8 ( d );
                  fsize += sizeof ( d );
               }
               Uint8 lstbyte = lastbyte;
               stream.writeUint8 ( lstbyte );
               fsize += sizeof ( lstbyte );
               count = 1;
               lastbyte = c;
            }
               
         }
         if ( count > 1 || lastbyte >= 192 ) {
            Uint8 d = 192 + count;
            stream.writeUint8 ( d );
            fsize += sizeof ( d );
         }
         Uint8 lstbyte = lastbyte;
         stream.writeUint8 ( lstbyte );
         fsize += sizeof ( lstbyte );
      }

   if ( header.nplanes == 1 ) {
      Uint8 d = 12;
      stream.writeUint8 ( d );
      fsize += sizeof ( d );

      dacpalette256 pal2;

      for ( int i = 0; i < 3; i++ )
      for ( int j = 0; j < 256; j++ )
          pal2[j][i] = pal[j][i] << 2;

      stream.writedata ( &pal2, 768 ); // endian ok !!!
      fsize += 768 ;
   }
   stream.seek ( 0 );
   header.size = fsize;
   header.write ( &stream );
}
