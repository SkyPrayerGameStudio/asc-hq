
#include <string.h>
#include "basegfx.h"
#include "newfont.h"
#include "misc.h"

#include "vesa.h"


tgraphmodeparameters *agmp = (tgraphmodeparameters *) & activegraphmodeparameters;
tgraphmodeparameters *hgmp = (tgraphmodeparameters *) & hardwaregraphmodeparameters;


int xlatbuffersize = 66000;

#ifdef _DOS_
 #define CheckForDirectScreenAccess 
#else
 #define CheckForDirectScreenAccess if (agmp->directscreenaccess == 0) copy2screen();
#endif

void generategrayxlattable( ppixelxlattable tab, byte offset, byte size)
{
	for ( int b = 0; b <= 255; b++) {
//		(*tab)[b] = (char) (offset + size - 1 - ((*activepalette256)[b][0] + (*activepalette256)[b][1] + (*activepalette256)[b][2]) * size / 192);
		(*tab)[b] = (char) (offset + size - 1 - ( 0.299 * (*activepalette256)[b][0] + 0.587 * (*activepalette256)[b][1] + 0.114 * (*activepalette256)[b][2]) * size / 64 );
	}
}

void getpicsize( void* hd2, int &width, int &height)
{
   trleheader* hd = (trleheader*) hd2;
   if (hd->id == 16973) {
      width = hd->x;
      height = hd->y;
   } else {
     width = hd->id + 1;
     height = hd->size + 1;
   }
}

int getpicsize2( void* hd2 )
{
   trleheader* hd = (trleheader*) hd2;
   int width; 
   int height;
   if (hd->id == 16973) 
      return hd->size + sizeof(*hd);
   else {
     width = hd->id + 1;
     height = hd->size + 1;
     return height*width+4;
   }
}




void 
rahmen(boolean invers,
       integer x1,
       integer y1,
       integer x2,
       integer y2)
{
	byte            col;

	if (invers == false)
		col = white;
	else
		col = darkgray;
	line(x1, y1, x1, y2, col);
	line(x1, y1, x2, y1, col);
	if (invers == true)
		col = white;
	else
		col = darkgray;
	line(x2, y1, x2, y2, col);
	line(x1, y2, x2, y2, col);
    CheckForDirectScreenAccess
}

void tdrawline :: start ( int x1, int y1, int x2, int y2 )
{ 
   int      i, deltax, deltay, numpixels, d, dinc1, dinc2, x, xinc1, xinc2, y, yinc1, yinc2; 


    /*  calculate deltax and deltay for initialisation  */ 
    
  deltax = x2 - x1; 
  if ( deltax < 0 )
     deltax = -deltax;

  deltay = y2 - y1; 
  if ( deltay < 0 )
     deltay = -deltay;

    /*  initialize all vars based on which is the independent variable  */ 
  if (deltax >= deltay) 
    { 

        /*  x is independent variable  */ 
      numpixels = deltax + 1; 
      d = (2 * deltay) - deltax; 
      dinc1 = deltay << 1; 
      dinc2 = (deltay - deltax) << 1; 
      xinc1 = 1; 
      xinc2 = 1; 
      yinc1 = 0; 
      yinc2 = 1; 
    } 
  else 
    { 

        /*  y is independent variable  */ 
      numpixels = deltay + 1; 
      d = (2 * deltax) - deltay; 
      dinc1 = deltax << 1; 
      dinc2 = (deltax - deltay) << 1; 
      xinc1 = 0; 
      xinc2 = 1; 
      yinc1 = 1; 
      yinc2 = 1; 
    } 

    /*  make sure x and y move in the right directions  */ 
  if (x1 > x2) 
    { 
      xinc1 = -xinc1; 
      xinc2 = -xinc2; 
    } 
  if (y1 > y2) 
    { 
      yinc1 = -yinc1; 
      yinc2 = -yinc2; 
    } 

    /*  start drawing at <x1, y1>  */ 
  x = x1; 
  y = y1; 

    /*  draw the pixels  */ 
  for (i = 1; i <= numpixels; i++) 
    { 
      putpix( x, y ); 


      if (d < 0) 
        { 
          d = d + dinc1; 
          x = x + xinc1; 
          y = y + yinc1; 
        } 
      else 
        { 
          d = d + dinc2; 
          x = x + xinc2; 
          y = y + yinc2; 
        } 
    } 
} 


void 
line(int  x1,
     int  y1,
     int  x2,
     int  y2,
     char actcol)
{
	float           m, b;
	int             w;
	float           yy1, yy2, xx1, xx2;



    if ( x1 == x2) {
        for (w=y1;w<=y2 ;w++ ) 
           putpixel(x1, w, actcol );

    } else {
       if ( y1 == y2) {
          for (w=x1;w<=x2 ;w++ ) 
               putpixel(w, y1, actcol );
       } else {
        	yy1 = y1;
        	yy2 = y2;
        	xx1 = x1;
        	xx2 = x2;
        	m = (yy2 - yy1) / (xx2 - xx1);
        	b = y1 - m * x1;
        	if ((m <= 1) && (m >= -1)) {
        		if (x2 < x1) {
        			w = x2;
        			x2 = x1;
        			x1 = w;
        			w = y2;
        			y2 = y1;
        			y1 = w;
        		}
        		for (w = x1; w <= x2; w++) 
        			putpixel(w, (int) (m * w + b), actcol);
        		
        	} else {
        		if (y2 < y1) {
        			w = x2;
        			x2 = x1;
        			x1 = w;
        			w = y2;
        			y2 = y1;
        			y1 = w;
        		}
        		for (w = y1; w <= y2; w++) {
        			putpixel((int) ((w - b) / m), w, actcol);
        		}
        
        	}
         } /* endif */
     }

     CheckForDirectScreenAccess
}

void 
xorline(int  x1,
     int  y1,
     int  x2,
     int  y2,
     char actcol)
{
	float           m, b;
	int             w;
	float           yy1, yy2, xx1, xx2;

    if ( x1 == x2) {
        for (w=y1;w<=y2 ;w++ ) {
		putpixel( x1, w, getpixel ( x1, w ) ^ actcol);
        } /* endfor */
    } else {
	yy1 = y1;
	yy2 = y2;
	xx1 = x1;
	xx2 = x2;
	m = (yy2 - yy1) / (xx2 - xx1);
	b = y1 - m * x1;
	if ((m <= 1) && (m >= -1)) {
		if (x2 < x1) {
			w = x2;
			x2 = x1;
			x1 = w;
			w = y2;
			y2 = y1;
			y1 = w;
		}
		for (w = x1; w <= x2; w++) {
			putpixel(w, (int) (m * w + b), getpixel( w, (int) (m * w + b) ) ^ actcol);
		}
	} else {
		if (y2 < y1) {
			w = x2;
			x2 = x1;
			x1 = w;
			w = y2;                  
			y2 = y1;
			y1 = w;
		}
		for (w = y1; w <= y2; w++) {
			putpixel((int) ((w - b) / m), w, getpixel ( (int) ((w - b) / m), w ) ^ actcol);             
		}

	}
     }

     CheckForDirectScreenAccess
}



void 
rectangle(int x1,
	  int y1,
	  int x2,
	  int y2,
	  byte color)
{
	line(x1, y1, x1, y2, color);
	line(x1, y1, x2, y1, color);
	line(x2, y1, x2, y2, color);
	line(x1, y2, x2, y2, color);

   CheckForDirectScreenAccess
}


void xorrectangle(int x1,
	     int y1,
	     int x2,
	     int y2,
	     byte color)
{
          xorline(x1,y1,x1,y2,color); 
          xorline(x1,y1,x2,y1,color);
          xorline(x2,y1,x2,y2,color); 
          xorline(x1,y2,x2,y2,color);

   CheckForDirectScreenAccess
}

/*
void         xorrectangle( tmouserect r, byte         color)
{
   xorrectangle ( r.x1, r.y1, r.x2, r.y2, color );
}

  */


void* halfpict ( void* vbuf )
{
   char* buf = (char*) vbuf;

   word* wp = (word*) xlatbuffer;
   char* dest = (char*) xlatbuffer;

   trleheader*   hd = (trleheader*) vbuf; 

   if ( hd->id == 16973 ) { 
      wp[0] = hd->x / 2;
      wp[1] = hd->y / 2;

      if ( ( wp[0] + 1 ) * ( wp[1] + 1 ) + 4 >= xlatbuffersize )
         throw fatalgraphicserror ( "halfpict : picture larger than buffer ! " );

      dest += 4;

      buf += sizeof ( *hd );


      int linecount = 0;
      int rowcount = 0;

      for ( int c = 0; c < hd->size; c++ ) {
         if ( *buf == hd->rle ) {
            for ( int i = buf[1]; i > 0; i-- ) {
               if ( !(linecount & 1)  &&  !(rowcount & 1)) {
                  *dest = buf[2];
                  dest++;
               }
               rowcount++;
               if ( rowcount > hd->x ) {
                  rowcount = 0;
                  linecount++;
               }
            }

            buf += 3;
            c += 2;

         } else {
            if ( !(linecount & 1)  &&  !(rowcount & 1)) {
               *dest = *buf;
               dest++;
            }
            buf++;
            rowcount++;
            if ( rowcount > hd->x ) {
               rowcount = 0;
               linecount++;
            }
         }
         if ( (int)dest - (int)xlatbuffer > xlatbuffersize )
            throw fatalgraphicserror ( "halfpict : picture larger than buffer ! " );

      }
   } else {
      int linecount = 0;
      int rowcount = 0;

      word* wp2 = (word*) vbuf;

      wp[0] = wp2[0] / 2;
      wp[1] = wp2[1] / 2;

      if ( ( wp[0] + 1 ) * ( wp[1] + 1 ) + 4 >= xlatbuffersize )
         throw fatalgraphicserror ( "halfpict : picture larger than buffer ! " );

      dest += 4;
      buf += 4;

      for ( int c = (wp2[0] + 1) * (wp2[1] + 1); c > 0; c-- ) {
         if ( !(linecount & 1)  &&  !(rowcount & 1)) {
            *dest = *buf;
            dest++;
         }
         buf++;
         rowcount++;
         if ( rowcount > wp2[0] ) {
            rowcount = 0;
            linecount++;
         }
      }

   }
   return xlatbuffer;
} 


void putshadow ( int x1, int y1, void* ptr, ppixelxlattable xl )
{
   word* w = (word*) ptr;
   char* c = (char*) ptr + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 + agmp->linearaddress);
      for ( int y = w[1] + 1; y > 0; y-- ) {
         for ( int x = w[0]+1; x > 0; x-- ) {
            if ( *c != 255 )
               *buf = (*xl)[*buf];
            buf++;
            c++;
         }
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void putpicturemix ( int x1, int y1, void* ptr, int rotation, char* mixbuf )
{
   word* w = (word*) ptr;
   char* c = (char*) ptr + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 + agmp->linearaddress);
      for ( int y = w[1] + 1; y > 0; y-- ) {
         for ( int x = w[0]+1; x > 0; x-- ) {
            if ( *c != 255 ) {
               int o = *buf << 8;
               if ( *c >= 16  && *c < 24 )
                  *buf = mixbuf[o + *c + rotation];
               else
                  *buf = mixbuf[o + *c];
            }
            buf++;
            c++;
         }
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}


void putinterlacedrotimage ( int x1, int y1, void* ptr, int rotation )
{
   word* w = (word*) ptr;
   char* c = (char*) ptr + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 + agmp->linearaddress);
      for ( int y = w[1] + 1; y > 0; y-- ) {
         for ( int x = w[0]+1; x > 0; x-- ) {
            if ( *c != 255 )
               if ( ((int)(buf+y)) & 1 )
                  if ( *c >= 16  && *c < 24 )
                     *buf = *c + rotation;
                  else
                     *buf = *c;
            buf++;
            c++;
         }
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}


void rotatepict90 ( void* s, void* d )
{
   word* sw = (word*) s;
   char* sc = (char*) s + 4;
   
   word* dw = (word*) d;
   char* dc = (char*) d + 4;

   dw[0] = sw[1];
   dw[1] = sw[0];

   int dl = dw[0]+1;
   int dh = dw[1]+1;

   int sl = sw[0]+1;
   int sh = sw[1]+1;

   for (int y = 0; y <= dw[1]; y++) 
     for (int x = 0; x <= dw[0]; x++) 
         dc[ y * dl + x] = sc[ ( sh - x - 1 ) * sl + y];
}

void flippict ( void* s, void* d, int dir )
{
   word* sw = (word*) s;
   char* sc = (char*) s + 4;
   
   word* dw = (word*) d;
   char* dc = (char*) d + 4;

   dw[1] = sw[1];
   dw[0] = sw[0];

   int sl = sw[0]+1;
   int sh = sw[1]+1;

   if ( dir == 1 ) {
      for (int y = 0; y <= dw[1]; y++) 
        for (int x = 0; x <= dw[0]; x++) 
            dc[ y * sl + x] = sc[ y * sl + ( sl - 1 - x ) ];
   } else {
      for (int y = 0; y <= dw[1]; y++) 
        for (int x = 0; x <= dw[0]; x++) 
            dc[ y * sl + x] = sc[ ( sh - 1 - y)  * sl + x ];
   }
}






void putpixel(int x1, int y1, int color)
{
   if ( agmp->byteperpix == 1 )
      putpixel8 ( x1, y1, color );
   else {
      if ( agmp->windowstatus == 100 ) {
         char* pc = (char*) ( agmp->linearaddress + x1 * agmp->byteperpix + y1 * agmp->scanlinelength );
         int alpha = color >> 24;
         if ( alpha == 0 ) {
            pc[ agmp->redfieldposition/8 ] = color & 0xff;
            pc[ agmp->greenfieldposition/8 ] = (color >> 8) & 0xff;
            pc[ agmp->bluefieldposition/8 ] = (color >> 16) & 0xff;
         } else {
            pc[ agmp->redfieldposition/8 ] = pc[ agmp->redfieldposition/8 ] * alpha / alphabase + (color & 0xff) * (alphabase - alpha ) / alphabase;
            pc[ agmp->greenfieldposition/8 ] = pc[ agmp->greenfieldposition/8 ] * alpha / alphabase + ((color >> 8 ) & 0xff) * (alphabase - alpha ) / alphabase;
            pc[ agmp->bluefieldposition/8 ] = pc[ agmp->bluefieldposition/8 ] * alpha / alphabase + ((color >> 16) & 0xff) * (alphabase - alpha ) / alphabase;
         }
      } else {
        /*
         int pos = x1 * agmp->byteperpix + y1 * agmp->scanlinelength;
         int page = pos >> 16;
         if ( hgmp->actsetpage != page )
            setvirtualpagepos ( page );

         char* pc = (char*) ( agmp->linearaddress + (pos & 0xffff) );

         int alpha = color >> 24;
         if ( alpha == 0 ) {
            pc[ agmp->redfieldposition/8 ] = color & 0xff;
            pc[ agmp->greenfieldposition/8 ] = (color >> 8) & 0xff;
            pc[ agmp->bluefieldposition/8 ] = (color >> 16) & 0xff;
         } else {
            pc[ agmp->redfieldposition/8 ] = pc[ agmp->redfieldposition/8 ] * alpha / alphabase + (color & 0xff) * (alphabase - alpha ) / alphabase;
            pc[ agmp->greenfieldposition/8 ] = pc[ agmp->greenfieldposition/8 ] * alpha / alphabase + ((color >> 8 ) & 0xff) * (alphabase - alpha ) / alphabase;
            pc[ agmp->bluefieldposition/8 ] = pc[ agmp->bluefieldposition/8 ] * alpha / alphabase + ((color >> 16) & 0xff) * (alphabase - alpha ) / alphabase;
         }
        */ 
      }
   }

   CheckForDirectScreenAccess
}

int getpixel(int x1, int y1)
{
   if ( agmp->byteperpix == 1 )
      return getpixel8( x1, y1 );
   else {
      if ( agmp->windowstatus == 100 ) {
         char* pc = (char*) ( agmp->linearaddress + x1 * agmp->byteperpix + y1 * agmp->scanlinelength );
         return pc[ agmp->redfieldposition/8 ] + 
              ( pc[ agmp->greenfieldposition/8 ] << 8 ) + 
              ( pc[ agmp->bluefieldposition/8 ] << 16 );
      } else {
        #ifndef _NOASM_
        /*
         int pos = x1 * agmp->byteperpix + y1 * agmp->scanlinelength;
         int page = pos >> 16;
         if ( hgmp->actsetpage != page )
            setvirtualpagepos ( page );

         char* pc = (char*) ( agmp->linearaddress + (pos & 0xffff) );
         return pc[ agmp->redfieldposition/8 ] + 
              ( pc[ agmp->greenfieldposition/8 ] << 8 ) + 
              ( pc[ agmp->bluefieldposition/8 ] << 16 );
        */
        #endif
        return -1;
        
      }
   }
}




TrueColorImage :: TrueColorImage ( int x, int y )
{
   pix = new trgbpixel[x*y];
   xsize = x;
   ysize = y;
}

TrueColorImage :: ~TrueColorImage (  )
{
   if ( pix ) {
      delete[] pix;
      pix = NULL;
   }
}


int getpixelfromimage ( void* buf, int x, int y )
{
   word* wp = (word*) buf;
   if ( x > wp[0]  ||  y > wp[1] || x < 0 || y < 0 )
      return -1;

   char* pc = (char*) buf;
   return pc[4 + x + y * (wp[0]+1) ];
}

void TrueColorImage :: setpix ( int x, int y, int r, int g, int b, int alpha )
{
   trgbpixel* p = & pix[ x + y * xsize ];
   p->channel.r = r;
   p->channel.g = g;
   p->channel.b = b;
   p->channel.a = alpha;
}

trgbpixel TrueColorImage :: getpix ( int x, int y )
{
   return pix[ x + y * xsize ];
}


int TrueColorImage :: getxsize( void )
{
   return xsize;
}

int TrueColorImage :: getysize( void )
{
   return ysize;
}




int f2i ( float f )
{
   
//   int b = f;

   int a; 
   f -= 0.4999;
   float2int ( &f, &a );
/*   if ( a != b )
      return -1; */
   return a;
}

int f2i2 ( float f )
{
   int a; 
   float2int ( &f, &a );
   return a;
}


int newpalgenerated = 0;
dacpalette256 ppal;




TrueColorImage* zoomimage ( void* buf, int xsize, int ysize, dacpalette256 pal, int interpolate, int proportional )
{
   if ( !newpalgenerated ) 
      for ( int i = 0; i < 256; i++ )
         for ( int j = 0; j<3; j++ )
            ppal[i][j] = pal[i][j] << 2;



   int orgx;
   int orgy;
   getpicsize ( buf, orgx, orgy );

   {
      float ratiox = (float)xsize / (float)orgx;
      float ratioy = (float)ysize / (float)orgy;
      if ( proportional ) {
         if ( ratiox > ratioy )
            xsize = ratioy * orgx;
         else
            ysize = ratiox * orgy;
      }
   }

   TrueColorImage* img = new TrueColorImage ( xsize, ysize );
   

   float dx = (float)orgx / (float)xsize;
   float dy = (float)orgy / (float)ysize;

   float oy = 0;
   for ( int y = 0; y < ysize; y++, oy+=dy ) {

      int oyi = f2i ( oy );
      float oyif = oyi;

      float ox = 0;
      for ( int x = 0; x < xsize; x++,ox+=dx ) {

         float r = 0;
         float g = 0;
         float b = 0;
         float alpha = 0;

         if ( interpolate ) {
            float d;

            int oxi = f2i ( ox );
            float oxif = oxi;
   
            float dist = 0;
            int alphanum = 0;

            // pixel links oben
            int px1 = getpixelfromimage ( buf, oxi, oyi );
            int px2 = getpixelfromimage ( buf, oxi+1, oyi );
            int px3 = getpixelfromimage ( buf, oxi, oyi+1 );
            int px4 = getpixelfromimage ( buf, oxi+1, oyi+1 );
            if ( px1 == 255 && px2 == 255 && px3 == 255 && px4 == 255 ) {
                r = 0;
                g = 0;
                b = 0;
                alpha = alphabase;
            } else {

               if ( px1 >= 0 ) {
                  d = ( 1-(ox - oxif)) * ( 1-( oy - oyif ));
      
                  if ( px1 == 255 ) {
                     alpha += ((float)alphabase) * d;
                     alphanum++;
                  }
                  else {
                     dist += d;
                     r += ppal[px1][0] * d ;
                     g += ppal[px1][1] * d ;
                     b += ppal[px1][2] * d ;
                  }
               }
                  
               // pixel rechts oben
               if ( px2 >= 0 ) {
                  d = (ox - oxif) * ( 1 -  ( oy - oyif ));
      
                  if ( px2 == 255 ) {
                     alpha += ((float)alphabase) * d;
                     alphanum++;
                  }
                  else {
                     dist += d;
                     r += ppal[px2][0] * d ;
                     g += ppal[px2][1] * d ;
                     b += ppal[px2][2] * d ;
                  }
               }
   
   
               // pixel links unten
               if ( px3 >= 0 ) {
                  d = ( 1- (ox - oxif)) *  (oy - oyif);
      
                  if ( px3 == 255 ) {
                     alpha += ((float)alphabase) * d;
                     alphanum++;
                  }
                  else {
                     dist += d;
                     r += ppal[px3][0] * d ;
                     g += ppal[px3][1] * d ;
                     b += ppal[px3][2] * d ;
                  }
               }
   
               // pixel rechts unten
               if ( px4 >= 0 ) {
                  d = (ox - oxif) *  (oy - oyif);
      
                  if ( px4 == 255 ) {
                     alpha += ((float)alphabase) * d;
                     alphanum++;
                  }
                  else {
                     dist += d;
                     r += ppal[px4][0] * d ;
                     g += ppal[px4][1] * d ;
                     b += ppal[px4][2] * d ;
                  }
               }
   
               if ( alphanum <= 2 ) 
                  alpha = 0;
   
               if ( dist ) {
                  r /= dist;
                  g /= dist;
                  b /= dist;
                  alpha /= dist;
               } else {
                  r = 0;
                  g = 0;
                  b = 0;
                  alpha = alphabase;
               }
            }
         } else {
             int px = getpixelfromimage ( buf, f2i2(ox), f2i2(oy) );
             if ( px == 255 ) 
                alpha += alphabase ;
             else {
                r += ppal[px][0] ;
                g += ppal[px][1] ;
                b += ppal[px][2] ;
             }

         }
         img->setpix ( x, y, f2i(r), f2i(g), f2i(b), f2i(alpha) );
      }
   }
   
   return img;
}

#define sqr(a) (a)*(a)
#define cub(a) abs ((a)*(a)*(a))


#ifndef minimal
 #include "basestrm.h"
#endif


TrueColorImage* convertimage2tc ( void* buf, dacpalette256 pal )
{
   if ( !buf ) 
      return NULL;

   int x, y;
   getpicsize ( buf, x, y );
   return zoomimage ( buf, x,y , pal, 0, 0 );
}


char* convertimage ( TrueColorImage* img, dacpalette256 pal )
{
  #ifndef minimal
   if ( truecolor2pal_table[0] == 255 ) {
      tfindfile ff ( "tc2pal.dat" );
      if ( ff.getnextname() ) {
         tnfilestream stream ( "tc2pal.dat", 1 );
         stream.readdata ( truecolor2pal_table, sizeof ( truecolor2pal_table ));
      } else {
         for ( int r = 0; r < 64; r++ )
            for ( int g = 0; g < 64; g++ )
               for ( int b = 0; b < 64; b++ ) {
                     int sml = r  + ( g << 6) + ( b << 12 );
      
                     int diff = 0xFFFFFFF;
                     int pix1;
            
                     for ( int k=0;k<256 ;k++ ) {
                        int actdif = sqr( pal[k][0]  - r ) + sqr( pal[k][1]  - g ) + sqr( pal[k][2]  - b );
            
                        if (actdif < diff) {
                           diff = actdif;
                           pix1 = k;
                        }
                     } 
                    truecolor2pal_table[sml] = pix1;
               }
       /*
         tnfilestream stream ( "tc2pal.dat", 2 );
         stream.writedata ( truecolor2pal_table, sizeof ( truecolor2pal_table ));
       */
      }
   }
  #endif
   int size = imagesize ( 1, 1, img->getxsize(), img->getysize() );
   char* newimg = new char[ size ] ;
   char* start = newimg;
   word* wp = (word*) newimg;
   wp[0] = img->getxsize()-1;
   wp[1] = img->getysize()-1;

   newimg+= 4;

   for ( int y = 0; y <= wp[1]; y++ )
      for ( int x = 0; x <= wp[0]; x++ ) {
         trgbpixel p  = img->getpix ( x, y );


         if ( p.channel.a <= alphabase*2/3 ) {
            int sml = ( p.channel.r >> 2) + (( p.channel.g >> 2) << 6) + (( p.channel.b >> 2) << 12);

            *newimg = truecolor2pal_table[sml];

         } else {
           *newimg = 255;
         }
         newimg++;
      }

   if ( newimg - start > size ) 
      printf("\a");
   return (char*)wp;
}


fatalgraphicserror :: fatalgraphicserror ( char* strng ) {
   strcpy ( st, strng );
}

fatalgraphicserror :: fatalgraphicserror ( void ) {
   st[0] = 0;
}
                 
void putmask ( int x1, int y1, void* vbuf, int newtransparence )
{
      int linecount = 0;
      int rowcount = 0;

      word* wp = (word*) vbuf;

      char* basemembuf = (char*) (agmp->linearaddress + y1 * agmp->bytesperscanline + x1 * agmp->byteperpix );
      char* img = (char*) vbuf + 4;

      for ( int y = 0; y <= wp[1]; y++ ) {
         char* membuf = basemembuf + y * agmp->bytesperscanline;

         for ( int x = 0; x <= wp[0]; x++ ) {
            if ( *img != newtransparence )
               *membuf = *img;
            membuf += agmp->byteperpix;
            img++;
         }
      }
}

int graphicinitialized = 0;



void* uncompress_rlepict ( void* pict )
{
   trleheader* hd = (trleheader*) pict;
   if (hd->id == 16973) {

      int w, h;
      getpicsize( pict, w, h );
      w++;
      h++;
      void* newbuf = asc_malloc ( w * h + 4 );
      word* wp = (word*) newbuf;
      char* dest = (char*) newbuf;
      wp[0] = w-1;
      wp[1] = h-1;
      dest +=4;
   
      char* buf = (char*) pict;
      buf += sizeof ( *hd );
   
      for ( int c = 0; c < hd->size; c++ ) {
         if ( *buf == hd->rle ) {
            for ( int i = buf[1]; i > 0; i-- ) {
               *dest = buf[2];
               dest++;
            }
            buf += 3;
            c += 2;
   
         } else {
            *dest = *buf;
            dest++;
            
            buf++;
         }
      }
      return newbuf;
   } else
      return NULL;
}

void setvgapalette256 ( dacpalette256 pal )
{
   if ( graphicinitialized )
      set_vgapalette256( pal );
   else
      memcpy ( activepalette256, pal, sizeof ( *activepalette256 ));
}


void ellipse ( int x1, int y1, int x2, int y2, int color, float tolerance )
{
   int midx = (x1 + x2) / 2;
   int midy = (y1 + y2) / 2;
   float xr = x2 - x1;
   float yr = y2 - y1;

   tolerance = tolerance / (xr+yr) *  80;

   xr= (xr/2)*(xr/2);
   yr= (yr/2)*(yr/2);

   for ( int y = y1; y <= y2; y++ )
      for ( int x = x1; x <= x2; x++ ) {
         float dx = x - midx;
         float dy = y - midy;
         float tmp = dx*dx/xr + dy*dy/yr;
//         float tmp = dx*dx*yr + dy*dy*xr;
         if (  tmp <= 1 + tolerance && tmp >= 1 - tolerance )
            putpixel ( x, y, color );
      }

   CheckForDirectScreenAccess

}



char truecolor2pal_table[262144];


dacpalette256* activepalette256;


class tinitgfxengine {
        public:
          tinitgfxengine ( void );
       } initgfx;

tinitgfxengine::tinitgfxengine ( void )
{
   memset ( truecolor2pal_table, 255, sizeof ( truecolor2pal_table ));
   hgmp = (tgraphmodeparameters *) & hardwaregraphmodeparameters;
   agmp = (tgraphmodeparameters *) & activegraphmodeparameters;
   activepalette256 = (dacpalette256*) &activepalette;
   xlatbuffer = asc_malloc ( xlatbuffersize );
}


void tvirtualdisplay :: init ( int x, int y, int color )
{
   agmp = (tgraphmodeparameters *) & activegraphmodeparameters;
   oldparams = *agmp ;
   char* cbuf = (char*) asc_malloc ( x * y );
   if ( !cbuf )
      throw fatalgraphicserror ( "could not allocate memory !");

   buf = cbuf;
   for ( int i = 0; i < (x*y); i++ )
      cbuf[i] = color;

   agmp->resolutionx   = x   ;
   agmp->resolutiony   = y   ;
   agmp->windowstatus  = 100 ;
   agmp->scanlinelength  = x ;
   agmp->scanlinenumber  = y ;
   agmp->bytesperscanline  = x;
   agmp->byteperpix   = 1    ;
   agmp->linearaddress = (int) buf ;
   agmp->bitperpix          = 8;
   agmp->directscreenaccess = 1;

}

tvirtualdisplay :: tvirtualdisplay ( int x, int y )
{
   init ( x, y, 0 );
}

tvirtualdisplay :: tvirtualdisplay ( int x, int y, int color )
{
   init ( x, y, color );
}


tvirtualdisplay :: ~tvirtualdisplay ( )
{
   asc_free (  buf ) ;
   *agmp = oldparams;
}



#ifdef _NOASM_

 tgraphmodeparameters activegraphmodeparameters;
 tgraphmodeparameters hardwaregraphmodeparameters;
 
 int dpmscapabilities;
 int actdpmsmode;
 
 dacpalette256  activepalette;
 int       palette16[256][4];
 void*     xlatbuffer;
 // dacpalette256  *activepalette256;




void putpixel8 ( int x1, int y1, int color )
{
    char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
    *buf = color;

   CheckForDirectScreenAccess
}

int getpixel8 ( int x1, int y1 )
{
    char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
    return *buf;

   CheckForDirectScreenAccess
}


void bar(int x1, int y1, int x2, int y2, char color)
{
   if ( agmp->windowstatus == 100 ) {
      int spacelength = agmp->scanlinelength - (x2-x1) - 1;
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      for ( int y = y1; y <= y2; y++ ) {
         for ( int x = x1; x <= x2; x++ ) 
            *(buf++) = color;

         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}


void getimage(int x1, int y1, int x2, int y2, void *buffer)
{
   if ( agmp->windowstatus == 100 ) {
      int spacelength = agmp->scanlinelength - (x2-x1) - 1;
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);

      char* cb = (char*) buffer;
      word* wb = (word*) buffer;
   
      wb[0] = x2 - x1;
      wb[1] = y2 - y1;
      cb += 4;

      for ( int y = y1; y <= y2; y++ ) {
         for ( int x = x1; x <= x2; x++ ) 
            *(cb++) = *(buf++);

         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void putimage ( int x1, int y1, void* img )
{
   char* src = (char*) img;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);

      trleheader*   hd = (trleheader*) img;
   
      if ( hd->id == 16973 ) { 
   
         int spacelength = agmp->scanlinelength - hd->x - 1;

         src  += sizeof ( *hd );

         int x = 0;
         for ( int c = 0; c < hd->size; c++ ) {
            if ( *src == hd->rle ) {
               x += src[1];
               for ( int i = src[1]; i > 0; i-- )
                  *(buf++) = src[2];
   
               src += 3;
               c+=2;

            } else {
               *(buf++) = *(src++);
               x++;
            }

            if ( x > hd->x ) {
               buf += spacelength;
               x = 0;
            }
         }
      } else {
         word* w = (word*) img;
         int spacelength = agmp->scanlinelength - *w - 1;
         src += 4;
         for ( int y = w[1] + 1; y > 0; y-- ) {
            for ( int x = w[0]+1; x > 0; x-- ) 
               *(buf++) = *(src++);
            
            buf+=spacelength;
         }
      }
   }

   CheckForDirectScreenAccess
}

void putxlatfilter ( int x1, int y1, void* pic, char* xlattables )
{
   char* src = (char*) pic;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      trleheader*   hd = (trleheader*) pic;
   
      if ( hd->id == 16973 ) { 
         int spacelength = agmp->scanlinelength - hd->x - 1;
         src  += sizeof ( *hd );

         int x = 0;
         for ( int c = 0; c < hd->size; c++ ) {
            if ( *src == hd->rle ) {
               x += src[1];
               for ( int i = src[1]; i > 0; i-- ) {
                  *buf = xlattables[ src[2] * 256 + *buf ];
                  buf++;
               }

               src += 3;
               c+=2;
   
            } else {
               *buf = xlattables[ *(src++) * 256 + *buf ];
               buf++;
               x++;
            }
            
            if ( x > hd->x ) {
               buf += spacelength;
               x = 0;
            }
         }
      } else {
         word* w = (word*) pic;
         int spacelength = agmp->scanlinelength - *w - 1;

         src += 4;
         for ( int y = w[1] + 1; y > 0; y-- ) {
            for ( int x = w[0]+1; x > 0; x-- ) {
               *buf = xlattables[ *(src++) * 256 + *buf ];
               buf++;
            }
   
            buf+=spacelength;
         }
      }
   }

   CheckForDirectScreenAccess
}


void putspriteimage ( int x1, int y1, void* pic )
{
   char* src = (char*) pic;

   if ( agmp->windowstatus == 100 ) {
      trleheader*   hd = (trleheader*) pic;
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      if ( hd->id == 16973 ) { 
         int spacelength = agmp->scanlinelength - hd->x - 1;
         src  += sizeof ( *hd );

         int x = 0;
         for ( int c = 0; c < hd->size; c++ ) {
            if ( *src == hd->rle ) {
               x += src[1];
               if ( src[2] != 255 ) {
                  for ( int i = src[1]; i > 0; i-- )
                     *(buf++) = src[2];
               } else
                  buf += src[1];
               src += 3;
               c+=2;
   
            } else {
               if ( *src != 255 )
                  *buf = *src;
               buf++;
               src++;
               x++;
            }

            if ( x > hd->x ) {
               buf += spacelength;
               x = 0;
            }
         }
      } else {
         word* w = (word*) pic;
         int spacelength = agmp->scanlinelength - *w - 1;
         src += 4;
         for ( int y = w[1] + 1; y > 0; y-- ) {
            for ( int x = w[0]+1; x > 0; x-- ) {
               char d = *(src++);
               if ( d != 255 )
                  *buf = d;
               buf++;
            }
            
            buf+=spacelength;
         }
      }
   }

   CheckForDirectScreenAccess
}

void putrotspriteimage(int x1, int y1, void *pic, int rotationvalue)
{
   char* src = (char*) pic;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      trleheader*   hd = (trleheader*) pic;
   
      if ( hd->id == 16973 ) { 
   
         int spacelength = agmp->scanlinelength - hd->x - 1;
         src  += sizeof ( *hd );

         int x = 0;
         for ( int c = 0; c < hd->size; c++ ) {
            if ( *src == hd->rle ) {
               x += src[1];
               char d = src[2];
               if ( d != 255 ) {
                  if ( d >= 16 && d < 24 )
                     d += rotationvalue;
                  for ( int i = src[1]; i > 0; i-- )
                     *(buf++) = d;
               } else
                  buf += src[1];
   
               src += 3;
               c+=2;
   
            } else {
               char d = *(src++);
               if ( d != 255 ) {
                  if ( d >= 16 && d < 24 )
                     d += rotationvalue;
                  *(buf++) = d;
               } else
                 buf++;

               x++;
            }

            if ( x > hd->x ) {
               buf += spacelength;
               x = 0;
            }
         }
      } else {
         word* w = (word*) pic;
         int spacelength = agmp->scanlinelength - *w - 1;
         src += 4;
         for ( int y = w[1] + 1; y > 0; y-- ) {
            for ( int x = w[0]+1; x > 0; x-- ) {
               char d = *(src++);
               if ( d != 255 ) {
                  if ( d >= 16 && d < 24 )
                     d += rotationvalue;
   
                  *buf = d;
               }
               buf++;
            }
            
            buf+=spacelength;
         }
      }
   }

   CheckForDirectScreenAccess
}

void putrotspriteimage90(int x1, int y1, void *pic, int rotationvalue)
{
   word* w = (word*) pic;
   char* c = (char*) pic + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      for ( int y = 0; y <= w[1] ; y++ ) {
         for ( int x = 0; x <= w[0]; x++ ) {
            int d = getpixelfromimage ( pic, y, w[0] - x );
            if ( d != 255 && d != -1) {
               if ( d >= 16 && d < 24 )
                  d += rotationvalue;

               *buf = d;
            }
            buf++;
         }
         
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void putrotspriteimage180(int x1, int y1, void *pic, int rotationvalue)
{
   word* w = (word*) pic;
   char* c = (char*) pic + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      for ( int y = 0; y <= w[1] ; y++ ) {
         for ( int x = 0; x <= w[0]; x++ ) {
            int d = getpixelfromimage ( pic, w[0] - x, w[1] - y );
            if ( d != 255 && d != -1) {
               if ( d >= 16 && d < 24 )
                  d += rotationvalue;

               *buf = d;
            }
            buf++;
         }
         
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void putrotspriteimage270(int x1, int y1, void *pic, int rotationvalue)
{
   word* w = (word*) pic;
   char* c = (char*) pic + 4;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      for ( int y = 0; y <= w[1] ; y++ ) {
         for ( int x = 0; x <= w[0]; x++ ) {
            int d = getpixelfromimage ( pic, w[1] - y, x );
            if ( d != 255 && d != -1) {
               if ( d >= 16 && d < 24 )
                  d += rotationvalue;

               *buf = d;
            }
            buf++;
         }
         
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void puttexture ( int x1, int y1, int x2, int y2, void *texture )
{
   char* c = (char*) texture;
   int spacelength = agmp->scanlinelength - (x2 - x1) - 1;

   if ( agmp->windowstatus == 100 ) {
      int offset = agmp->scanlinelength * y1 + x1;
      char* buf = (char*) agmp->linearaddress;
      for ( int y = y1 ; y <= y2; y++ ) {
         for ( int x = x1; x <= x2 ; x++ ) {
            buf[offset] = c[offset];
            offset++;
         }
         
         offset+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}


void putspritetexture ( int x1, int y1, int x2, int y2, void *texture )
{
   char* c = (char*) texture;
   int spacelength = agmp->scanlinelength - (x2 - x1) - 1;

   if ( agmp->windowstatus == 100 ) {
      int offset = agmp->scanlinelength * y1 + x1;
      char* buf = (char*) agmp->linearaddress;
      for ( int y = y1 ; y <= y2; y++ ) {
         for ( int x = x1; x <= x2 ; x++ ) {
            char d = c[offset];
            if ( d != 255 )
               buf[offset] = d;
            offset++;
         }
         
         offset+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void putimageprt ( int x1, int y1, int x2, int y2, void *texture, int dx, int dy )
{
   word* w = (word*) texture;
   int spacelength = agmp->scanlinelength - *w - 1;

   if ( agmp->windowstatus == 100 ) {
      char* buf = (char*) (agmp->scanlinelength * y1 + x1 * agmp->byteperpix + agmp->linearaddress);
      for ( int y = 0; y <= w[1] ; y++ ) {
         for ( int x = 0; x <= w[0]; x++ ) {
            int p = getpixelfromimage ( texture, x + dx, y + dy );
            if ( p != -1 ) 
               *buf = p;

            buf++;
         }
         buf+=spacelength;
      }
   }

   CheckForDirectScreenAccess
}

void copybuf2displaymemory(int size, void *buf)
{
   memcpy ( (void*) agmp->linearaddress, buf, size );

   CheckForDirectScreenAccess
}         


void* xlatpict ( ppixelxlattable xl, void* vbuf )
{

   char* buf = (char*) vbuf;

   word* wp = (word*) xlatbuffer;
   char* dest = (char*) xlatbuffer;

   trleheader*   hd = (trleheader*) vbuf; 

   if ( hd->id == 16973 ) { 
      memcpy ( xlatbuffer, vbuf, sizeof ( *hd ));

      trleheader* desthead = (trleheader*) xlatbuffer; 


      dest += sizeof ( *hd );
      buf  += sizeof ( *hd );

      for ( int c = 0; c < hd->size; c++ ) 
         if ( *buf == hd->rle ) {
            *(dest++) = *(buf++);
            *(dest++) = *(buf++);
            *(dest++) = (*xl)[ *(buf++) ];

            c += 2;

         } else {
            int newpix = (*xl)[ *(buf++) ];
            if ( newpix == hd->rle ) {
               *(dest++) = hd->rle;
               *(dest++) = 1;
               *(dest++) = hd->rle;
               desthead->size += 2;
            } else
               *(dest++) = newpix;
         }
      
   } else {
      word* wp2 = (word*) vbuf;

      wp[0] = wp2[0];
      wp[1] = wp2[1];
/*
      if ( ( wp[0] + 1 ) * ( wp[1] + 1 ) + 4 >= xlatbuffersize )
         throw fatalgraphicserror ( "halfpict : picture larger than buffer ! " );
*/
      dest += 4;
      buf += 4;

      for ( int c = (wp2[0] + 1) * (wp2[1] + 1); c > 0; c-- ) {
         *dest = (*xl)[ *buf ];
         dest++;
         buf++;
      }

   }
   return xlatbuffer;
}

int loga2 ( int a )
{
   int l = 0;
   if ( a ) 
      while ( ! (a & 1)) {
         a >>= 1;
         l++;
      }
   return l;
}


void float2int ( float* fp, int* ip )
{
   *ip = *fp;
}



void showtext ( const char* text, int x, int y, int textcol )
{
   if ( !activefontsettings.font )
     return;

   if ( !text )
     return;
 
   char* fb = (char*)(x * agmp->byteperpix + y * agmp->scanlinelength + agmp->linearaddress);
   int negkernwidth = 0;
   int fontheight; 
   int extraheight = 0;
   if ( activefontsettings.height == 0 )
      fontheight = activefontsettings.font->height;
   else
      if ( activefontsettings.height > activefontsettings.font->height ) {
         fontheight = activefontsettings.font->height;
         extraheight = activefontsettings.height - activefontsettings.font->height;
      } else 
         fontheight = activefontsettings.height;

    const char* t = text;
    int length = 0;

    char* characterpointer[1024];
    int    characterwidth[1024];
    int    characterdist[1024];
    int ps = 0;
    while ( *t ) {
       if ( activefontsettings.font->character[*t].width ) {
          characterwidth[ps] = activefontsettings.font->character[*t].width;
          characterpointer[ps] = activefontsettings.font->character[*t].memposition + 2;
          if ( t[1] )
             characterdist[ps] = activefontsettings.font->kerning[t[1]][t[0]] + 2;
          else
             characterdist[ps] = 0;
          length +=activefontsettings.font->character[*t].width;
          length += characterdist[ps];
          ps++;
       }
       t++;
    }
    int leftextralength;
    int rightextralength;

    if ( !activefontsettings.length )
       leftextralength = rightextralength = 0;
    else {
       if ( activefontsettings.justify == 0 ) {
          leftextralength = 0;
          if ( activefontsettings.length > length )
             rightextralength = activefontsettings.length - length;
          else {
             rightextralength = 0;
             length = activefontsettings.length;
          }
       } else
       if ( activefontsettings.justify == 1 ) {
          if ( activefontsettings.length > length ) {
             leftextralength = (activefontsettings.length - length)/2;
             rightextralength = activefontsettings.length - length - leftextralength;
          } else {
             rightextralength = 0;
             leftextralength = 0;
             length = activefontsettings.length;
          }
       } else
       if ( activefontsettings.justify == 2 ) {
          rightextralength = 0;
          if ( activefontsettings.length > length )
             leftextralength = activefontsettings.length - length;
          else {
             leftextralength = 0;
             length = activefontsettings.length;
          }
       } 
       
    }
    int suppressbkgr = 0;
    int spacelength = agmp->scanlinelength - (length + leftextralength + rightextralength);
    if ( activefontsettings.background == 255 ) {
       spacelength += leftextralength + rightextralength;
       fb += leftextralength;
       leftextralength = rightextralength = 0;
       extraheight = 0;
    }
    for ( int yl = 0; yl < fontheight; yl++ ) {
       if ( leftextralength ) 
          for ( int i = 0; i < leftextralength; i++ )
             *(fb++) = activefontsettings.background;

       int x = 0;
       ps = 0;
       while ( x < length ) {
          for (int cx = characterwidth[ps]; cx > 0 && x < length; cx--) {
             int pix = *(characterpointer[ps]++);
             if ( pix )
                if ( textcol != -1 )
                   *fb = textcol;
                else
                   *fb = pix;
             else
                if ( activefontsettings.background != 255 ) 
                   if ( suppressbkgr )
                      suppressbkgr--;
                   else
                     *fb = activefontsettings.background;
             fb++;
             x++;
          } /* endfor */
          if ( x < length ) {
             if ( characterdist[ps] > 0 ) {
                for (int i = characterdist[ps]; i && x < length; i-- ) {
                   if ( activefontsettings.background != 255 )
                     *fb = activefontsettings.background;
                   fb++;
                   x++;
                } /* endfor */
             } else {
                fb += characterdist[ps];
                x += characterdist[ps];
                suppressbkgr = -characterdist[ps];
             }
          }
          ps++;
       }

       if ( rightextralength )
          for ( int i = 0; i < rightextralength; i++ )
             *(fb++) = activefontsettings.background;

       fb += spacelength;
    }
    if ( extraheight )
       for ( int yl = extraheight; yl > 0; yl-- ) {
          for ( int x = length + leftextralength + rightextralength; x > 0; x-- )
             *(fb++) = activefontsettings.background;
          fb += spacelength;
       }

   CheckForDirectScreenAccess
}

void showtext2 ( const char* text, int x, int y )
{
   showtext ( text, x, y, activefontsettings.color );
}

void showtext2c ( const char* text, int x, int y )
{           
   showtext ( text, x, y, -1 );
}


#endif

