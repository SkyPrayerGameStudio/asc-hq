//     $Id: pd.cpp,v 1.6 1999-12-30 20:30:37 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.5  1999/12/29 17:38:19  mbickel
//      Continued Linux port
//
//     Revision 1.4  1999/12/28 21:03:17  mbickel
//      Continued Linux port
//      Added KDevelop project files
//
//     Revision 1.3  1999/11/22 18:27:48  mbickel
//      Restructured graphics engine:
//        VESA now only for DOS
//        BASEGFX should be platform independant
//        new interface for initialization
//      Rewrote all ASM code in C++, but it is still available for the Watcom
//        versions
//      Fixed bugs in RLE decompression, BI map importer and the view calculation
//
//     Revision 1.2  1999/11/16 03:42:21  tmwilson
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

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>

#include "tpascal.inc"
#include "newfont.h"
#include "basegfx.h"
#include "keybp.h"
#include "mousehnd.h"
#include "pd.h"
#include "misc.h"


   pfont        pulldownfont;
   tfontsettings      savefont;

void tpulldown::init(void)
{
   int ml = 0;
   int mr = 0;
   int rlang;


   action2execute = -1;
   bkgcolor = lightgray;            
   rcolor1 = white;
   rcolor2 = darkgray;
   textcolor = black;             
   shortkeycolor = red;     
   pdb.pdbreite = 20;
   pdfieldtextdistance = 50;
   textstart = 10;
   righttextdifference = 45;

   pdfieldnr = 255;

   for (int i = 0; i < pdb.count; i++)
   {
      pdb.field[i].height = getpdfieldheight(i,pdb.field[i].count);
      pdb.field[i].xwidth = gettextwdth(pdb.field[i].name,pulldownfont) - 4;
      for (int j = 0; j < pdb.field[i].count; j++)
         { 
            getleftrighttext(pdb.field[i].button[j].name,lt,rt);
            int llang = gettextwdth(lt,pulldownfont);
            if (rt != NULL ) rlang = gettextwdth(rt,pulldownfont);
            else rlang = 0;
            if (llang > ml) ml = llang;
            if (rlang > mr) mr = rlang;
         } 
      pdb.field[i].xwidth = textstart + ml + textdifference + mr + textstart;
      pdb.field[i].rtextstart =textstart + ml + textdifference;
      ml = 0;
      mr = 0;
    } 
}

void tpulldown::setvars(void)
{
   activefontsettings.color = textcolor;
   activefontsettings.font = pulldownfont; 
   activefontsettings.background = bkgcolor;
   activefontsettings.length = 300;
   activefontsettings.markcolor = shortkeycolor;
   activefontsettings.justify = lefttext;

}

tpulldown :: tpulldown ( void )
{
   pdb.count = 0;
}

void tpulldown::addbutton ( char* name, int id )
{
   tpdbutton* btn = &pdb.field[ pdb.count-1 ].button[ pdb.field[pdb.count-1].count++ ];
   strcpy ( btn->name , name );
   btn->actionid = id;
}

void tpulldown::addfield ( char* name )
{
   pdb.field[ pdb.count ].count = 0;
   strcpy ( pdb.field[ pdb.count ].name , name );
   pdb.count++;
}


void tpulldown::run(void)
{
   mousestat = mouseparams.taste; 
   boolean cancel = false;
   do { 
      if (mouseparams.y <= pdb.pdbreite) {
         if ( (pdfieldnr < pdb.count-1 ) && (mouseparams.x > pdb.field[pdfieldnr+1].xstart)) {
            collategraphicoperations cgo;
            closepdfield();
            pdfieldnr++;
            openpdfield();
         } else if ( (pdfieldnr > 0 ) && (mouseparams.x < pdb.field[pdfieldnr].xstart) ) {
            collategraphicoperations cgo;
            closepdfield();
            pdfieldnr --;
            openpdfield();
         } /* endif */
      } else if (mouseparams.y <= pdb.pdbreite + pdb.field[pdfieldnr].height) {
         if ( (mouseparams.x >= anf ) && (mouseparams.x <= ende ) ) {
            if ( (buttonnr > 0 ) && (mouseparams.y < pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr) ) ) {
               collategraphicoperations cgo;
               hidebutton();
               buttonnr--;
               showbutton();
            } else if ( (buttonnr < pdb.field[pdfieldnr].count-1 ) && (mouseparams.y > pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr+1) ) ) {
               collategraphicoperations cgo;
               hidebutton();
               buttonnr++;
               showbutton();
            } /* endif */
         } /* endif */
      } /* endif */
      if (mousestat != mouseparams.taste) {
         if (mouseparams.y <= pdb.pdbreite) mousestat = mouseparams.taste;
         else cancel = true;
      }
      if (keypress() ) {
         tkey ch = r_key();
         switch (ch) {
         case ct_esc : {
               pdfieldnr = 255;
               buttonnr = 255;
               return;
            }
         case ct_up : {
                hidebutton();
                if ( buttonnr>0 ) buttonnr--;
                else buttonnr = pdb.field[pdfieldnr].count-1;
                if (strcmp(pdb.field[pdfieldnr].button[buttonnr].name,"seperator") == 0) buttonnr--;
                showbutton();
            }
            break;
            case ct_down : {
                hidebutton();
                if (buttonnr < pdb.field[pdfieldnr].count-1 ) buttonnr++;
                else buttonnr = 0;
                if (strcmp(pdb.field[pdfieldnr].button[buttonnr].name,"seperator") == 0) buttonnr++;
                showbutton();
            }
            break;
         case ct_left : {
               closepdfield();
               if (pdfieldnr > 0 ) pdfieldnr--;
               else pdfieldnr = pdb.count-1;
               openpdfield();
            }
            break;
         case ct_right : {
               closepdfield();
               if (pdfieldnr < pdb.count-1 ) pdfieldnr++;
               else pdfieldnr = 0;
               openpdfield();
            }
            break;
         case ct_enter: {
               return;
            }
         default : {
                for (int k = 0; k < pdb.field[pdfieldnr].count; k++) 
                    if (pdb.field[pdfieldnr].button[k].shortkey == ch ) {
                       buttonnr = k;
                       return;
                    }
            }
            break;
         } /* endswitch */
      } 
   }  while (cancel == false);
   if ( (mouseparams.y > pdb.pdbreite  + pdb.field[pdfieldnr].height ) || (mouseparams.x < anf ) || (mouseparams.x > ende ) ) { 
      pdfieldnr = 255;
      buttonnr = 255;
      action2execute = -1;
   } else
      action2execute = pdb.field [ pdfieldnr] .button [ buttonnr].actionid;
}

void tpulldown::getleftrighttext(char *qtext, char *ltext,char *rtext)
{
   strcpy ( ltext, qtext );
   rtext[0] = 0;
   for (int j = 0 ; j <= strlen(qtext) ; j++ ) {
      if ( (ltext[j] == rz) || (ltext[j] == rz2)){
         ltext[j]=0;
         strcpy ( rtext, &ltext[j+1] );
      } /* endif */
   } /* endfor */
}

void tpulldown::checkkeys(void)
{
   if (key > 1024) {
      key -=1024;
      for (int i = 0; i < pdb.count; i++) 
         if (key == pdb.field[i].shortkey ) {
            baron();
            pdfieldnr=i;
            openpdfield();
            run();
            done();
            key = 0;
            if (pdfieldnr == 255) baroff();
            return;
         }
   }

}


void tpulldown::checkpulldown(void)
{
   checkkeys();
   if (mouseparams.y <= pdb.pdbreite)  {
      baron();
      if (mouseparams.taste == 1 ) {
         pdfieldnr = 0;
         for (int i=0;i < pdb.count-1  ;i++ )
            if (mouseparams.x > pdb.field[i+1].xstart)
               pdfieldnr++;
         openpdfield();
         run();
         done();
      }
   } 
   else baroff(); 
}


void tpulldown::lines(int x1,int y1,int x2,int y2)
{ 
   line(x1,y1,x2,y1,rcolor1);
   line(x2,y1,x2,y2,rcolor2);
   line(x1,y2,x2,y2,rcolor2);
   line(x1,y1,x1,y2,rcolor1);
} 


void tpulldown::nolines(int x1,int y1,int x2,int y2)
{ 
   rectangle(x1,y1,x2,y2,bkgcolor);
} 


void tpulldown::done(void)
{
   closepdfield();
   activefontsettings = savefont; 
}

void tpulldown::baron(void)
{ 
   if (barstatus == false ) {
      collategraphicoperations cgo;

      savefont = activefontsettings; 
      setvars();
      mousevisible(false);

      barbackgrnd = asc_malloc (imagesize(0,0, agmp->resolutionx-1 ,pdb.pdbreite) );
      getimage(0,0, agmp->resolutionx-1 ,pdb.pdbreite,barbackgrnd);

      bar(0,0, agmp->resolutionx-1 ,pdb.pdbreite,bkgcolor);

      bar(0,0, agmp->resolutionx-1 ,pdb.pdbreite,bkgcolor);
      lines(0,0, agmp->resolutionx-1 ,pdb.pdbreite); 
      int x = textstart;
      for (int i = 0; i < pdb.count; i++) {
         activefontsettings.length = gettextwdth(pdb.field[i].name, pulldownfont);
         showtext3(pdb.field[i].name,x+pdfieldtextdistance /2,pdb.pdbreite / 2 - 6);
         pdb.field[i].xstart = x;
         x += gettextwdth(pdb.field[i].name,pulldownfont) + pdfieldtextdistance;
      } 
      mousevisible(true);
      barstatus = true;
   }
} 

void tpulldown::baroff(void)
{ 
   if (barstatus == true) {
      collategraphicoperations cgo;

      mousevisible(false);
      putimage(0,0,barbackgrnd);
      asc_free( barbackgrnd );
      mousevisible(true); 
      barstatus = false;
   }
} 


int tpulldown::getpdfieldheight(byte pdfieldnr,byte pos)
{ 
  int         i, j;

   j = 0; 
   for (i = 0; i < pos; i++)
      if (strcmp(pdb.field[pdfieldnr].button[i].name,"seperator") == 0) j = j + 7;
      else j = j + pulldownfont->height + 6;
   return j; 
} 


void         tpulldown::openpdfield(void)
{
   collategraphicoperations cgo;

   int zw;
   setvars();

   activefontsettings.length = pdb.field[pdfieldnr].xwidth;
   // boolean abbrch = false;
   anf = pdb.field[pdfieldnr].xstart + pdfieldtextdistance / 2 - pdfieldenlargement;
   ende =  anf + pdb.field[pdfieldnr].xwidth + pdfieldenlargement*2;
   boolean umbau = false;
   if (ende > agmp->resolutionx-4 ) 
      { 
         umbau = true; 
         zw = ende - (agmp->resolutionx-4);
         ende -= zw;
         anf -= zw;
      } 

   mousevisible(false);
   backgrnd = asc_malloc( imagesize(anf - 3, 0 ,ende + 3,pdb.pdbreite + 6 + pdb.field[pdfieldnr].height) );
   getimage(anf - 3, 0 ,ende + 3, pdb.pdbreite + 6 + pdb.field[pdfieldnr].height,backgrnd);

   bar(anf - 3,pdb.pdbreite,ende + 3,pdb.pdbreite + 6 + pdb.field[pdfieldnr].height,bkgcolor);
   lines(anf - 3,pdb.pdbreite,ende + 3,pdb.pdbreite + 6 + pdb.field[pdfieldnr].height);
   int lang = gettextwdth(pdb.field[pdfieldnr].name,pulldownfont) + 13;
   if (umbau == true)
      {
         line(anf - 2, pdb.pdbreite, anf + lang -1 + zw , pdb.pdbreite,bkgcolor);
         line(anf - 3,1,anf - 3,pdb.pdbreite + 1,rcolor1);
         line(anf - 3,1,anf + lang + zw ,1,rcolor1);
         line(anf +  lang + zw ,1,anf + lang + zw ,pdb.pdbreite ,rcolor2);
      }
   else
      {
         line(anf - 2, pdb.pdbreite, anf + lang -1, pdb.pdbreite,bkgcolor);
         line(anf - 3,1,anf - 3,pdb.pdbreite + 1,rcolor1);
         line(anf - 3,1,anf + lang,1,rcolor1);
         line(anf +  lang,1,anf + lang,pdb.pdbreite ,rcolor2);
      }
   for (int i = 0; i < pdb.field[pdfieldnr].count; i++)
      {
         if (strcmp(pdb.field[pdfieldnr].button[i].name,"seperator") != 0) {
            getleftrighttext(pdb.field[pdfieldnr].button[i].name,lt,rt);
            activefontsettings.justify = lefttext;
            activefontsettings.length = gettextwdth(lt,pulldownfont);
            showtext3( lt ,anf + textstart ,pdb.pdbreite + 7 + getpdfieldheight(pdfieldnr,i));
            if (rt[0]) {
               activefontsettings.justify = lefttext;
               activefontsettings.length = gettextwdth(rt,pulldownfont);
               showtext3(rt, anf + pdb.field[pdfieldnr].rtextstart,pdb.pdbreite + 7 + getpdfieldheight(pdfieldnr,i));
            }
         }
         else
            line(anf,pdb.pdbreite + 7 + getpdfieldheight(pdfieldnr,i),ende + 1,pdb.pdbreite + 7 + getpdfieldheight(pdfieldnr,i),rcolor2);
      }
   buttonnr = 0;
   showbutton();

   mousevisible(true); 
} 

void         tpulldown::closepdfield(void)
{
   collategraphicoperations cgo;

   mousevisible(false);
   putimage(anf - 3,0,backgrnd);
   asc_free(backgrnd); 
   mousevisible(true); 
}

void tpulldown::hidebutton(void)
{ 
   collategraphicoperations cgo;

   if (strcmp(pdb.field[pdfieldnr].button[buttonnr].name,"seperator") == 0) return;
   mousevisible(false); 
   nolines(anf,pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr),ende,pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr+1));
   mousevisible(true); 
} 

void tpulldown::showbutton(void)
{ 
   collategraphicoperations cgo;

   if (strcmp(pdb.field[pdfieldnr].button[buttonnr].name,"seperator") == 0) return;
   mousevisible(false); 
   lines(anf,pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr),ende,pdb.pdbreite + 4 + getpdfieldheight(pdfieldnr,buttonnr+1));
   mousevisible(true); 
} 

void tpulldown::setshortkeys(void)
{ 
  int j;

   for (int i = 0; i < pdb.count; i++)
      { 
         pdb.field[i].shortkey = 0;
         for (j = 0; j < strlen(pdb.field[i].name); j++)
            if (pdb.field[i].name[j] == '~') {
               pdb.field[i].shortkey = char2key( toupper ( pdb.field[i].name[j+1] ) );
               j = strlen(pdb.field[i].name); 
            } 
         for (int k = 0; k < pdb.field[i].count; k++) {
            pdb.field[i].button[k].shortkey = 0; 
            for (j = 0; j < strlen(pdb.field[i].button[k].name); j++) {
               if (pdb.field[i].button[k].name[j] == '~') { 
                  pdb.field[i].button[k].shortkey = char2key( toupper ( pdb.field[i].button[k].name[j+1] ) );
                  j = strlen(pdb.field[i].button[k].name); 
               } 
            } 
         } 
      }
   } 
