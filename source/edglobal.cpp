//     $Id: edglobal.cpp,v 1.2 1999-11-16 03:41:33 tmwilson Exp $
//
//     $Log: not supported by cvs2svn $
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

#include <stdarg.h>

#include "edmisc.h"
#include "loadbi3.h"
#include "edevents.h"
#include "edgen.h"
#include "edselfnt.h"
#include "edglobal.h"
#include "timer.h"
   
mc_check mc;


   const char* execactionnames[execactionscount] = {"End MapEdit","Help","Goto EditMode","Select terrain","Select unit",
      "Select color","Select building","Select special object","Select mine","Select weather","Setup alliances",
      "Toggle ResourceMode","Change UnitDirection","Asc-Resource Mode","Write Map2PCX","Load map",
      "Change players","New map","Goto PolygonMode","Rebuild Display","Open UnitInfo","View map","About",
      "Change GlobalDirection","Create resources","View/Change cargo","View/Change resources","Change TerrainDirection",
      "View/Change Events","Toggle Fillmode","Mapgenerator","Use active field values as selection","Delete active thing","Show palette",
      "View/Change minestrength","View/Change mapvalues","View/Change production","Save map","View/Change UnitValues",
      "Mirror CX-Pos","Mirror CY-Pos","Place terrain","Place Unit","Place building","Place special object","Place mine",
      "Place active thing","Delete Unit","Delete building","Delete special object","Delete mine","AboutBox","Save map as ...",
      "End PolygonMode","Smooth coasts","Import BI-Map","SEPERATOR","BI-Resource Mode","Insert BI map", "Set zoom level", 
      "Move Building", "set weather of whole map", "set map parameters", "terrain info" };


// �S Infomessage

int infomessage( char* formatstring, ... )
{
   char stringtooutput[200];
   char* b;
   char* a = formatstring;
   char* c = new char[200];
   char* d;
   int linenum = 0;
   int i;

   memset (stringtooutput, 0, sizeof ( stringtooutput ));

   b = stringtooutput;

   va_list paramlist;
   va_start ( paramlist, formatstring );

   while (*a != 0) {
      if (*a == '%' ) {
         switch (a[1]) {
         case 'c': 
            *b = va_arg ( paramlist, char );
            b++;
            break;
         case 'd':
         case 'i':
         case 'u':
            i = va_arg ( paramlist, int );
            itoa ( i, c, 10 );
            i=0;
            while (c[i]) {
               *b = c[i];
               b++;
               i++;
            } /* endwhile */
            break;
         case 'o':
            i = va_arg ( paramlist, int );
            itoa ( i, c, 8 );
            i=0;
            while (c[i]) {
               *b = c[i];
               b++;
               i++;
            } /* endwhile */
            break;
         case 'p':
         case 'x':
            i = va_arg ( paramlist, int );
            itoa ( i, c, 16 );
            i=0;
            while (c[i]) {
               *b = c[i];
               b++;
               i++;
            } /* endwhile */
            break;
         case 'X':
            i = va_arg ( paramlist, int );
            itoa ( i, c, 16 );
            strupr ( c );
            i=0;
            while (c[i]) {
               *b = c[i];
               b++;
               i++;
            } /* endwhile */
            break;
         case 's':
            d = va_arg ( paramlist, char* );
            while (*d) {
               *b = *d;
               b++;
               d++;
            } /* endwhile */
            break;
         } /* endswitch */
         a+=2;
      } else {
         *b = *a;
         b++;
         a++;
      } /* endif */
   } /* endwhile */
   *b = 0;

   va_end ( paramlist );



   npush ( activefontsettings );
   activefontsettings.justify = lefttext;
   activefontsettings.font = schriften.smallarial;
   activefontsettings.color = lightgray;
   activefontsettings.markcolor = red;
   activefontsettings.background = 0;
   activefontsettings.length = agmp->resolutionx - ( 640 - 387);

   int yy = agmp->resolutiony - ( 480 - 450 );
   setinvisiblemouserectanglestk ( 37, yy, 37 + activefontsettings.length, 450 + activefontsettings.font->height );
   showtext3c( stringtooutput, 37, yy );
   getinvisiblemouserectanglestk();

   npop( activefontsettings );

   if ( formatstring == NULL  ||  formatstring[0] == 0 )
      lastdisplayedmessageticker = 0xffffff;
   else
      lastdisplayedmessageticker = ticker;
   

   delete ( c );

   return ++actdisplayedmessage;
}

//�S MC_CHeck

void mc_check::on(void)
{
   if (mycursor.an == false ) {
      int ms = getmousestatus(); 
      if (ms == 1) { 		//mouse off
         if (mstatus == 0) mousevisible(true);
         else mstatus++;
      } else { 			//mouse on
         mstatus++;
      }
      if (cursor.an == false) {  //cursor off
         if (cstatus == 0) cursor.show();
         else cstatus++;
      } else { 			//cursor on
         cstatus++;
      } /* endif */
   }
}

void mc_check::off(void)
{
   if (mycursor.an == false ) {
      int ms = getmousestatus(); 
      if (ms == 1) { 		//mouse off
         mstatus--;
      } else { 			//mouse on
         if (mstatus == 0) mousevisible(false);
         else mstatus--;
      }
      if (cursor.an == false) {  	//cursor off
         cstatus--;
      } else { 			//cursor on
         if (cstatus == 0) cursor.hide();
         else cstatus--;
      } /* endif */
   }
}



class  GetString : public tdialogbox {
          public :
              byte action;
              char* buf;
              void init(char* _title);
              virtual void run(void);
              virtual void buttonpressed(byte id);
           };

void         GetString::init(char* _title)
{ 
   tdialogbox::init(); 
   title = _title; 
   x1 = 120; 
   xsize = 400; 
   y1 = 150; 
   ysize = 140; 
   action = 0; 

   windowstyle = windowstyle ^ dlg_in3d; 

   addbutton("~D~one",20,ysize - 40,100,ysize - 20,0,1,1,true); 
   addkey(1,ct_enter); 
   addbutton("~C~ancel",120,ysize - 40,200,ysize - 20,0,1,2,true); 
   addkey(2, ct_esc );

   addbutton("",20,60,xsize - 20,80,1,1,3,true); 
   addeingabe(3,buf,0,1000);

   buildgraphics(); 

   mousevisible(true); 
} 


void         GetString::run(void)
{ 
   pbutton pb = firstbutton;
   while ( pb &&  (pb->id != 3)) 
      pb = pb->next;

   if ( pb )
      if ( pb->id == 3 )
         execbutton( pb , false );

   do { 
      tdialogbox::run(); 
   }  while ( !action ); 
} 


void         GetString::buttonpressed(byte         id)
{ 
   tdialogbox::buttonpressed(id); 
   switch (id) {
      
      case 1:   
      case 2:   action = id; 
   break; 
   } 
} 


char*    getstring( char*  title, char* orgval )

{ 
   GetString     gi;

   gi.buf = orgval;
   gi.init( title );
   gi.run();
   gi.done();
   if ( gi.action == 2 )
      return NULL;
   else
      return gi.buf;
} 

void appendbackslash ( char* string )
{
   if ( strlen ( string ) && string[strlen ( string ) -1] != '\\' )
      strcat ( string, "\\");
}

char* getbipath ( void )
{
   char* path = getbi3path();
   char filename[1000];
   strcpy ( filename, path );

   char filename2[1000];
   strcpy ( filename2, filename );
   appendbackslash( filename2 );
   strcat ( filename2, "mis\\");
   strcat ( filename2, "*.dat");

   while ( !exist ( filename2 )) {
      char* res = getstring("enter Battle Isle path", filename );
      if ( res == NULL )
         return NULL;

      strcpy ( filename2, filename );
      appendbackslash( filename2 );
      strcat ( filename2, "mis\\");
      strcat ( filename2, "*.dat");
   }
   appendbackslash( filename );
   char* buf = strdup ( filename );
   if ( gameoptions.bi3.dir ) {
      if ( stricmp ( gameoptions.bi3.dir, filename ) != 0 ) {
         asc_free ( gameoptions.bi3.dir );
         gameoptions.bi3.dir = strdup ( filename );
         gameoptions.changed = 1;
      }

   } else {
      gameoptions.bi3.dir = strdup ( filename );
      gameoptions.changed = 1;
   }
   return buf;
}

// �S ExecAction

void execaction(int code)
{
   switch(code) {
    case act_help :   if ( polyfieldmode ) help ( 1040 );
                       else help(1000);
       break;
    case act_selbodentyp : {
                        ch = 0;
                        cursor.hide();
                        selterraintype( ct_f3 );
                        cursor.show();
                     }
       break;
    case act_selunit : {
                       ch = 0;
                       cursor.hide();
                       selvehicletype( ct_f4 );
                       cursor.show();
                     }
       break;
    case act_selcolor : {
                       ch = 0;
                       cursor.hide();
                       selcolor( ct_f5 );
                       cursor.show();
                     }
       break;
    case act_selbuilding : {
                       ch = 0;
                       cursor.hide();
                       selbuilding( ct_f6);
                       cursor.show();
                     }
       break;
    case act_selobject : {
                       ch = 0;
                       cursor.hide();
                       selobject( ct_f7 );
                       cursor.show();
                     }
       break;
    case act_selmine : {
                       ch = 0;
                       cursor.hide();
                       selmine( ct_f8 );
                       cursor.show();
                     }
       break;
    case act_selweather : {
                       ch = 0;
                       cursor.hide();
                       selweather( ct_f9  );   // !!!!!         // Test (Boolean) Testet, ob das wetter auch verf�gbar ist f�r bodentyp
                       cursor.show();                           // True : WIRD getestet / false : kein Test
                     }
       break;
    case act_setupalliances :  setupalliances();
       break;
    case act_toggleresourcemode :  {
                      if (showresources < 2) showresources++;
                      else showresources = 0;
                      displaymap();
       }
       break;
    case act_changeglobaldir : {
                                  auswahld++;
                                  if (auswahld > sidenum-1) auswahld = 0;
                                 //// if ( selectnr > cselcolor) selectnr = cselbodentyp; ???????
                                  showallchoices();
                                }
       break;
    case act_asc_resource :   {
                                  actmap->resourcemode = false;
                                  displaymessage ( "ASC Resource mode enabled", 3 );
                                }
       break;
    case act_bi_resource :   {
                                  actmap->resourcemode = true;
                                  displaymessage ( "Battle Isle Resource mode enabled", 3 );
                               }
       break;
    case act_maptopcx : writemaptopcx ();  
       break;
    case act_loadmap :   {
                            if (mapsaved == false )
                               if (choice_dlg("Map not saved ! Save now ?","~y~es","~n~o") == 1) 
                                  k_savemap(false);

                             k_loadmap();   
 
                             actmap->player[8].firstvehicle = NULL;
 
                             pdbaroff(); 
                          } 
       break;
    case act_changeplayers : playerchange();
       break;
    case act_newmap :   newmap();
       break;
    case act_polymode :   {
          getpolygon(&pfpoly);
          if (pfpoly != NULL ) {
             tfill = false;
             polyfieldmode = true;

             tchangepoly cp;

             cp.poly = pfpoly;
             cp.setpolytemps(1);
             cursor.gotoxy(1,1);
             
             displaymap();
             pdbaroff();
          }
       }
       break;            
    case act_repaintdisplay :   repaintdisplay();
       break;
    case act_unitinfo :  vehicle();
       break;
    case act_viewmap :  
             {
             while (mouseparams.taste != 0);             
             cursor.hide();
             showmap ();
             displaymap();
             cursor.show();
             }
       break;
    case act_about :  {
                                  activefontsettings.length = 530;
                                  showtext2(kgetaboutmessage(),100,435);
                                 }
       break;
    case act_changeunitdir : {
                      pf = getactfield();
                      if ( (pf != NULL) && (pf->vehicle != NULL ) ){
                         pf->vehicle->direction++;
                         if (pf->vehicle->direction>sidenum-1) pf->vehicle->direction = 0;
                         mapsaved = false;
                         displaymap();
                      } 
                   }
       break;
    case act_changeresources :   changeresource();
       break;
    case act_createresources : { 
                           tputresourcesdlg prd;
                           prd.init();
                           prd.run();
                           prd.done();
                         }
       break;
    case act_changecargo :   {
                 cursor.hide(); 
                 if ( getactfield()->building )                    
                    building_cargo( getactfield()->building );
                 else 
                    if ( getactfield()->vehicle )
                       unit_cargo( getactfield()->vehicle );
                 cursor.show(); 
              } 
       break;
    case act_changeterraindir : {
                      pf = getactfield();
                      if (pf != NULL) {
                         pf->direction++;
                         if (pf->direction>sidenum-1) pf->direction = 0;
                         mapsaved = false;
                         displaymap();
                      } 
                   }
       break;
    case act_events :   event();
       break;
    case act_fillmode :   if ( polyfieldmode == false ) {   
                 if (tfill == true) tfill = false; 
                 else tfill = true; 
                 fillx1 = cursor.posx + actmap->xpos; 
                 filly1 = cursor.posy + actmap->ypos; 
                 pdbaroff(); 
              } 
       break;
    case act_mapgenerator : mapgenerator();
       break;
    case act_setactivefieldvals : {
                  pfield fld = getactfield();
                  if ( fld->vehicle ) {
                     auswahlf = fld->vehicle->typ;
                     altefarbwahl = farbwahl;
                     farbwahl = fld->vehicle->color/8;
                     lastselectiontype = cselunit;
                     setnewvehicleselection ( auswahlf );
                  } else 
                  if ( fld->object && fld->object->objnum ) {
                     actobject = fld->object->object[ fld->object->objnum -1 ]->typ ;
                     lastselectiontype = cselobject;
                     setnewobjectselection ( actobject );
                  } else {
                     auswahld = fld->direction;
                     auswahl = fld->typ->terraintype;
                     lastselectiontype = cselbodentyp;
                     setnewterrainselection ( auswahl );
                  }
                  showallchoices();
               }
       break;
       
    case act_deletething : {
                         pf = getactfield();
                         mapsaved = false;
                         if (pf != NULL) {
                            if (pf->vehicle != NULL) removevehicle(&pf->vehicle);
                            else if (pf->building != NULL) removebuilding(&pf->building); 
                            else {
                                     pf->removeobject( actobject );
                                     pf->removemine();
                                   }
                            mapsaved = false;
                            displaymap();
                         }
                      } 
        break;
    case act_deleteunit : {
                         pf = getactfield();
                         if (pf != NULL) 
                            if (pf->vehicle != NULL) {
                               removevehicle(&pf->vehicle);
                               mapsaved = false;
                               displaymap();
                            }
                         }
        break;
     case act_deletebuilding : {
                         pf = getactfield();
                         if (pf != NULL) 
                            if (pf->building != NULL) { 
                               removebuilding(&pf->building); 
                               mapsaved = false;
                               displaymap();
                            }
                      }
        break;
     case act_deleteobject : {
                         pf = getactfield();
                         if (pf != NULL) {
                            mapsaved = false;
                            pf->removeobject( actobject );
                            displaymap();
                         }
                      }
        break;
     case act_deletemine : {
                         pf = getactfield();
                         if (pf != NULL) {
                            mapsaved = false; 
                            pf->removemine();
                            displaymap();
                         }
                      }
        break;
    case act_showpalette : showpalette();
       break;
    case act_changeminestrength : changeminestrength();
       break;
    case act_changemapvals :   changemapvalues();
       break;                                        
    case act_changeproduction :   if ( getactfield()->building ) building_production( getactfield()->building );
       break;
    case act_savemap :  k_savemap(false);
       break;
    case act_savemapas :  k_savemap(true);
       break;
    case act_changeunitvals :   {
                 pf = getactfield();
                 if (pf != NULL ) {
                    if (pf->vehicle != NULL ) {
                       changeunitvalues(pf->vehicle); 
                       displaymap();
                    }
                    else if (pf->building != NULL ) {
                       gbde = getactfield()->building;
                       changebuildingvalues(&gbde);
                    } /* endif */
                 } /* endif */
              } 
              break;
    case act_mirrorcursorx :   {
                    cursor.gotoxy ( actmap->xsize-getxpos(), getypos() );
                    int tmp = farbwahl;
                    farbwahl = altefarbwahl;
                    altefarbwahl = tmp;
                    showallchoices();
                    showcoordinates();
                 }

       break;
    case act_mirrorcursory :   {
                    cursor.gotoxy ( getxpos(), actmap->ysize-getypos() );
                    int tmp = farbwahl;
                    farbwahl = altefarbwahl;
                    altefarbwahl = tmp;
                    showallchoices();
                    showcoordinates();
                 }
       break;
    case act_placebodentyp : placebodentyp();
       break;
    case act_placeunit : placeunit();
       break;
    case act_placebuilding : placebuilding(farbwahl,auswahlb,true);
       break;
    case act_placeobject : placeobject();
       break;
    case act_placemine : placemine();
       break;
    case act_placething : putactthing();
       break;
    case act_endpolyfieldmode : {
          if (polyfieldmode) {
             polyfieldmode = false;
             tchangepoly cp;

             cp.poly = pfpoly;
             cp.setpolytemps(0);
             
             displaymap();
             pdbaroff();
             ch = 255;
          }
       }
       break;
    case act_end : {  
       if (choice_dlg("Do you really want to quit ?","~y~es","~n~o") == 2) ch = ct_invvalue; 
          else
               {
                ch = ct_esc; //Exit MapEdit
                if (mapsaved == false )
                   if (choice_dlg("Map not saved ! Save now ?","~y~es","~n~o") == 1) k_savemap(false);
               } 
       }
       break;
    case act_aboutbox : {
         help(1020);
         activefontsettings.length = 530;
         showtext2(kgetaboutmessage(),100,agmp->resolutiony - 30 );
      }
      break;
   case act_smoothcoasts : {
         #ifdef HEXAGON
            smooth ( 6 );
            displaymap();
         #endif
      }
      break;
   case act_import_bi_map : {
         #ifdef HEXAGON
         char filename2[260];
         char* path = getbipath();
         if ( !path )
            break;
            
         strcpy ( filename2, path );
         strcat ( filename2, "mis\\");
         strcat ( filename2, "*.dat");

         char filename[260];
         fileselectsvga ( filename2, filename,1 );
         if ( filename[0] ) {
            strcpy ( filename2, path );
            strcat ( filename2, "mis\\");
            strcat ( filename2, filename);
            pwterraintype t = auswahl->weather[auswahlw];
            if ( !t )
               t = auswahl->weather[0];
            importbattleislemap ( path, filename, t );
            displaymap();
         }
         #endif
      }
      break;
   case act_insert_bi_map : {
         #ifdef HEXAGON

         char filename2[260];

         char* path = getbipath();
         if ( !path )
            break;
            
         strcpy ( filename2, path );
         strcat ( filename2, "mis\\");
         strcat ( filename2, "*.dat");

         char filename[260];
         fileselectsvga ( filename2, filename,1 );
         if ( filename[0] ) {
            strcpy ( filename2, path );
            strcat ( filename2, "mis\\");
            strcat ( filename2, filename);
            insertbattleislemap ( getxpos(), getypos(), path, filename );
            displaymap();
         }
         #endif
      }
      break;
   case act_resizemap : cursor.hide();
                        resizemap();
                        cursor.checkposition ( actmap->xpos + cursor.posx, actmap->ypos + cursor.posy );
                        cursor.show();
      break;
   case act_movebuilding: movebuilding();
      break;
   case act_setactweatherglobal: setweatherall ( auswahlw, 1 );
                                 displaymap();
      break;
   case act_setmapparameters: setmapparameters();
      break;
   case act_terraininfo: viewterraininfo();
      break;
  #ifdef FREEMAPZOOM    
   case act_setzoom : choosezoomlevel();
      break;
  #endif

    }
}

