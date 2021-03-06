
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "global.h"

#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <new>
#include <cstdlib>
#include <ctype.h>
#include <algorithm>
#include <memory>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <pgpopupmenu.h>
#include <pgmenubar.h>

#include "paradialog.h"

#include "misc.h"
#include "iconrepository.h"
#include "dashboard.h"
#include "asc-mainscreen.h"
#include "guiiconhandler.h"
#include "guifunctions.h"
#include "graphics/blitter.h"
#include "graphics/drawing.h"
#include "overviewmappanel.h"
#include "ai/ai.h"
#include "itemrepository.h"
#include "mapdisplay.h"
#include "sg.h"
#include "gameoptions.h"
#include "dialog.h"
#include "widgets/textrenderer-addons.h"
#include "spfst-legacy.h"

#include "weaponrangelayer.h"
#include "dialogs/fileselector.h"

#include "actions/moveunitcommand.h"

ASC_MainScreenWidget*  mainScreenWidget = NULL ;


MainScreenWidget* getMainScreenWidget()
{
   return mainScreenWidget;
}

class Menu : public PG_MenuBar {

    PG_PopupMenu* currentMenu;
    typedef list<PG_PopupMenu*> Categories;
    Categories categories;

   public:
      Menu ( PG_Widget *parent, const PG_Rect &rect=PG_Rect::null);
      
   protected:
      void setup();   
      bool execAction  (PG_PopupMenu::MenuItem* menuItem );

   private:
      void addbutton(const char* name, int id );
      void addfield ( const char* name );
     
};








bool Menu::execAction  (PG_PopupMenu::MenuItem* menuItem )
{
   executeUserAction( tuseractions( menuItem->getId() ) );

   return true;
}


#define HIGHLIGHT

void Menu::addfield( const char* name )
{
   currentMenu = new PG_PopupMenu( NULL, -1, -1, "" );

#ifdef HIGHLIGHT
   categories.push_back ( currentMenu );
   Add ( name, currentMenu );
#else
   ASCString s = name;
   while ( s.find ( "~") != ASCString::npos )
      s.erase( s.find( "~"),1 );
   Add ( s, currentMenu );
#endif

   currentMenu->sigSelectMenuItem.connect( sigc::mem_fun( *this, &Menu::execAction ));
}

void Menu::addbutton( const char* name, int id )
{
#ifdef HIGHLIGHT
   currentMenu->addMenuItem( name, id );
#else
   ASCString s = name;
   while ( s.find ( "~") != ASCString::npos )
      s.erase( s.find( "~"),1 );

   currentMenu->addMenuItem( s, id );
#endif

}


void Menu::setup()
{
  
   addfield ( "Glo~b~al" );
   addbutton ( "~O~ptions", ua_gamepreferences );
   addbutton ( "~S~ound options", ua_soundDialog );
   addbutton ( "~E~mail options", ua_emailOptions );
   currentMenu->addSeparator();
   addbutton ( "~M~ain menu\tESC", ua_mainmenu );
   currentMenu->addSeparator();
   addbutton ( "~Q~uit\tctrl-q", ua_exitgame );


   addfield ("~G~ame");
   addbutton ( "~N~ew Game\tctrl-n", ua_newGame );
   currentMenu->addSeparator();
   addbutton ( "~L~oad game\tctrl-l", ua_loadgame );
   addbutton ( "Load most recent game\tctrl-shift-l", ua_loadrecentgame );
   addbutton ( "~S~ave game\tctrl-s", ua_savegame );
   currentMenu->addSeparator();
   addbutton ( "Continue network game\tF3", ua_continuenetworkgame);
   addbutton ( "Most recent network game\tShift-F3", ua_continuerecentnetworkgame);
   // addbutton ( "setup Net~w~ork", ua_setupnetwork );
   addbutton ( "Change Passw~o~rd", ua_changepassword );
   addbutton ( "supervise network game\tF4", ua_networksupervisor );
   currentMenu->addSeparator();
   addbutton ( "~D~iplomacy", ua_setupalliances);
   addbutton ( "transfer ~U~nit control", ua_giveunitaway );
   addbutton ( "~T~ransfer resources", ua_settribute);
   addbutton ( "Choose Technology", ua_chooseTechnology );
   addbutton ( "~C~ancel Research", ua_cancelResearch );
   currentMenu->addSeparator();
   addbutton ( "~E~dit Player Data", ua_editPlayerData);


   addfield ( "~A~ction" );
   addbutton ( "~U~ndo\tctrl-z", ua_undo );
   addbutton ( "~R~edo\tctrl-shift-z", ua_redo );
   addbutton ( "~M~anage Actions", ua_actionManager );
   addbutton ( "Manage ~P~ending Tasks", ua_taskManager );
   addbutton ( "Run all pending tasks\tctrl-t", ua_runOpenTasks );
   currentMenu->addSeparator();
   addbutton ( "Save Script", ua_writeLuaCommands );
   addbutton ( "Run Script", ua_runLuaCommands );


   addfield ( "~I~nfo" );
   addbutton ( "~V~ehicle types", ua_vehicleinfo );
   addbutton ( "Unit ~w~eapon range\t3", ua_viewunitweaponrange );
   addbutton ( "Unit ~m~ovement range\t4", ua_viewunitmovementrange );
   addbutton ( "~G~ame Time", ua_GameStatus );
   addbutton ( "unit ~S~et information", ua_UnitSetInfo );
   addbutton ( "unit guide dialog", ua_unitGuideDialog );
   addbutton ( "~T~errain\t7", ua_viewterraininfo );
   addbutton ( "~U~nit weight\t8", ua_unitweightinfo );
   currentMenu->addSeparator();
   addbutton ( "~R~esearch", ua_researchinfo );
   addbutton ( "~P~lay time", ua_showPlayerSpeed );
   addbutton ( "~C~argo Summary\tctrl-c", ua_cargosummary );
   addbutton ( "Unit Summary\tctrl-f", ua_unitsummary );
   currentMenu->addSeparator();
   addbutton ( "Unit Production Analysis", ua_unitproductionanalysis );
   addbutton ( "Mining Station Analysis", ua_showMiningPower );
	
	// addbutton ( "~R~esearch status", ua_showResearchStatus );
	
	// addbutton ( "vehicle ~I~mprovement\tF7", ua_dispvehicleimprovement);
	
	
	// addfield ( "~S~tatistics" );
	
	//   addbutton ( "~R~esources ", ua_statisticdialog );
	// addbutton ( "seperator");
   	// addbutton ( "~H~istory");

   addfield ( "~M~essage");
   addbutton ( "~n~ew message", ua_newmessage );
   addbutton ( "new reminder\tctrl-r", ua_createReminder );
   addbutton ( "view ~q~ueued messages", ua_viewqueuedmessages );
   addbutton ( "view ~s~ent messages", ua_viewsentmessages );
   addbutton ( "view ~r~eceived messages", ua_viewreceivedmessages);
   currentMenu->addSeparator();
   addbutton ( "view ~j~ournal", ua_viewjournal );
   addbutton ( "~a~ppend to journal", ua_editjournal );

   addfield ( "~T~ools" );
   addbutton ( "save ~M~ap as PNG", ua_writemaptopcx );
   addbutton ( "save ~S~creen as PCX", ua_writescreentopcx );
   addbutton ( "select graphic set", ua_selectgraphicset );
   currentMenu->addSeparator();
   addbutton ( "~G~oto coordinate\tctrl-g", ua_gotoPosition );

   addfield ( "~V~iew" );
   addbutton ( "toggle ~R~esourceview\t1", ua_changeresourceview );
   addbutton ( "toggle unit shading\t2", ua_toggleunitshading );
   addbutton ( "show ~P~ipeline net\t9", ua_viewPipeNet );
   addbutton ( "show ~V~isibility Range\t0", ua_visibilityInfo );
   currentMenu->addSeparator();
   addbutton ( "show reaction~f~ire", ua_viewReactionfireOverlay );	
   addbutton ( "show unit info\tctrl-1", ua_viewUnitinfoOverlay );
   addbutton ( "show unit experience\tctrl-2", ua_viewUnitexperienceOverlay );
   currentMenu->addSeparator();
   addbutton ( "Button Panel", ua_viewButtonPanel );
   addbutton ( "Wind Panel", ua_viewWindPanel );
   addbutton ( "Unit Info Panel", ua_viewUnitInfoPanel );
   addbutton ( "Overview Map Panel", ua_viewOverviewMapPanel );
   addbutton ( "Map Control Panel", ua_viewMapControlPanel );
//   addbutton ( "Action Panel",    ua_viewActionPanel );

   // addbutton("Weathercast", ua_weathercast);
   currentMenu->addSeparator();
   addbutton ( "Increase Map Zoom\tKP+", ua_increase_zoom );
   addbutton ( "Decrease Map Zoom\tKP-", ua_decrease_zoom );


   addfield ( "~D~ebug" );
   addbutton ( "Show ASC search ~P~ath", ua_showsearchdirs );
   addbutton ( "SDL settings", ua_SDLinfo );
   addbutton ( "Locate File", ua_locatefile );
   addbutton ( "test messages", ua_testMessages );
   addbutton ( "view font", ua_viewfont );
   addbutton ( "memory footprint", ua_getMemoryFootprint );
   addbutton ( "show event status", ua_eventInfo );
   currentMenu->addSeparator();
   addbutton ( "clear image cache", ua_clearImageCache );
   addbutton ( "reload dialog theme", ua_reloadDlgTheme );
   currentMenu->addSeparator();
   addbutton ( "resource analysis", ua_resourceAnalysis );
   addbutton ( "show techadapter", ua_showTechAdapter );
   addbutton ( "calc airplane endurance", ua_showUnitEndurance );
   addbutton ( "show game ~P~arameters", ua_GameParameterInfo );
   addbutton ( "Recompute View", ua_recompteview );
   addbutton ( "Create unit cost list ", ua_createUnitCostList );
   currentMenu->addSeparator();
   addbutton ( "benchmark without view calc", ua_benchgamewov );
   addbutton ( "benchmark with view calc", ua_benchgamewv);
   addbutton ( "compiler benchmark (AI)", ua_aibench );
   currentMenu->addSeparator();
   addbutton ( "unit AI properties", ua_unitAiOptions );
   currentMenu->addSeparator();
   addbutton ( "show packages used by map", ua_showUsedPackages );
   // addbutton ( "test memory integrity", ua_heapcheck );

   addfield ( "~H~elp" );
   // addbutton ( "HowTo ~S~tart email games", ua_howtostartpbem );
   // addbutton ( "HowTo ~C~ontinue email games", ua_howtocontinuepbem );
   // currentMenu->addSeparator();
   addbutton ( "~K~eys", ua_help );
   addbutton ( "~M~ap Layer", ua_viewlayerhelp );
   currentMenu->addSeparator();
	
   addbutton ( "~A~bout", ua_viewaboutmessage );
}


Menu::Menu ( PG_Widget *parent, const PG_Rect &rect)
    : PG_MenuBar( parent, rect, "MenuBar"),
      currentMenu(NULL)
{
   activateHotkey( KMOD_ALT );
   setup();
}






class UnitMovementRangeLayer : public MapLayer, public sigc::trackable {
   Surface& icon;
   GameMap* gamemap;

   map<MapCoordinate,int> fields;

   void markField( const MapCoordinate& pos )
   {
      fields[pos] |= 1;
   }

   bool addUnit( Vehicle* veh )
   {

      if ( fieldvisiblenow ( getfield ( veh->xpos, veh->ypos ))) {
         int counter = 0;
         
         AStar3D pathfinder( veh->getMap(), veh, false, veh->maxMovement() );
         pathfinder.findAllAccessibleFields( );
         
         for ( AStar3D::VisitedContainer::iterator i = pathfinder.visited.begin(); i != pathfinder.visited.end(); ++i )
            if ( fieldvisiblenow ( veh->getMap()->getField(i->h) )) {
               ++counter;
               markField( i->h );
            }
      
         if ( counter )
            fields[veh->getPosition()] |= 2;
            
         return counter;
      } else
         return false;
   };

   void reset()
   {
      fields.clear();
      if ( isActive() ) {
         setActive(false);
         statusMessage("Movement range layer disabled");
         repaintMap();
      }
   }
   public:

      void operateField( GameMap* actmap, const MapCoordinate& pos )
      {
         if ( !pos.valid() )
            return;
         
         if ( gamemap && gamemap != actmap )
            fields.clear();

         gamemap = actmap;
         
         if ( fields.find( pos ) != fields.end() ) {
            if ( fields[pos] & 2 ) {
               reset();
               return;
            }
         }
         
         if ( actmap->getField( pos )->vehicle ) {
            if ( addUnit( actmap->getField( pos )->vehicle ) ) {
               setActive(true);
               statusMessage("Movement range layer enabled");
               repaintMap();
            }
         }
      }
      
      UnitMovementRangeLayer() : icon ( IconRepository::getIcon( "markedfield-blue.png")), gamemap(NULL) {
         cursorMoved.connect( sigc::mem_fun( *this, &UnitMovementRangeLayer::reset ));
      }

      bool onLayer( int layer ) { return layer == 17; };
      
      void paintSingleField( const MapRenderer::FieldRenderInfo& fieldInfo,  int layer, const SPoint& pos )
      {
         if ( fieldInfo.gamemap != gamemap && gamemap) {
            reset();
            gamemap = NULL;
            return;
         }
         
         if ( fieldInfo.visibility >= visible_ago) {
            if ( fields.find( fieldInfo.pos ) != fields.end() ) {
               int p = fields[fieldInfo.pos];
               if ( p )
                  fieldInfo.surface.Blit( icon, pos );
            }
         }
      }
};


class VisibilityLayer : public MapLayer {
     void renderText( const MapRenderer::FieldRenderInfo& fieldInfo, SPoint pos, const ASCString& text );
    public: 
      bool onLayer( int layer ) { return layer == 17; };
      void paintSingleField( const MapRenderer::FieldRenderInfo& fieldInfo,  int layer, const SPoint& pos );
};

void VisibilityLayer::paintSingleField( const MapRenderer::FieldRenderInfo& fieldInfo, int layer, const SPoint& pos )
{
   if ( fieldVisibility( fieldInfo.fld, fieldInfo.gamemap->getPlayerView() ) >= visible_now ) {

      int view = 0;
      int jamming = 0;
      for ( int i = 0; i < fieldInfo.gamemap->getPlayerCount(); ++i ) 
         if ( fieldInfo.gamemap->getPlayer(i).diplomacy.sharesView( fieldInfo.gamemap->getPlayerView())) {
            view += fieldInfo.fld->view[i].view;
            jamming += fieldInfo.fld->view[i].jamming;
         }

#if 0
      ASCString s;
      s.format( "%d/%d", view, jamming);

      font->SetColor( 0xffffff );
      PG_FontEngine::RenderText( fieldInfo.surface.getBaseSurface() , PG_Rect( pos.x, pos.y, fieldsizex, fieldsizey), pos.x+6, pos.y + 21, s, font );

      font->SetColor( 0 );
      PG_FontEngine::RenderText( fieldInfo.surface.getBaseSurface() , PG_Rect( pos.x, pos.y, fieldsizex, fieldsizey), pos.x+5, pos.y + 20, s, font );
#else

      renderText( fieldInfo, SPoint(pos.x + 10, pos.y + 5), ASCString::toString( view ) + "V");
      renderText( fieldInfo, SPoint(pos.x + 10, pos.y + 18), ASCString::toString( jamming ) + "J");
      renderText( fieldInfo, SPoint(pos.x + 10, pos.y + 31), ASCString::toString( fieldInfo.fld->getjamming() ) + "T" );
#endif
   }
}

void VisibilityLayer::renderText( const MapRenderer::FieldRenderInfo& fieldInfo, SPoint pos, const ASCString& text )
{
   static PG_Font* font = NULL; 
   if ( !font ) {
      PG_Label l( NULL );
      font = new PG_Font( *(l.GetFont()));
      font->SetSize( 12 );
   }

   font->SetColor( 0xffffff );
   PG_FontEngine::RenderText( fieldInfo.surface.getBaseSurface() , PG_Rect( pos.x, pos.y, fieldsizex, fieldsizey), pos.x+1, pos.y + 13, text, font );

   font->SetColor( 0 );
   PG_FontEngine::RenderText( fieldInfo.surface.getBaseSurface() , PG_Rect( pos.x, pos.y, fieldsizex, fieldsizey), pos.x, pos.y+12, text, font );
}


ASC_MainScreenWidget::ASC_MainScreenWidget( PG_Application& application )
   : MainScreenWidget( application ), standardActionsLocked(0), guiHost(NULL), menu(NULL), unitInfoPanel(NULL), windInfoPanel(NULL), mapInfoPanel(NULL), actionInfoPanel(NULL)
{

   int mapWidth = Width() - 30;

   PG_Rect mapView(15,30,Width() - 200, Height() - 73);

   if ( CGameOptions::Instance()->panelColumns & 1 ) {
      mapView.x = 185;
      mapWidth -= 170;
   }
   if ( CGameOptions::Instance()->panelColumns & 2 ) {
      mapWidth -= 170;
   }
   mapView.my_width = mapWidth;


   setup( true, mapView );

   displayLogMessage ( 7, "done\n  Menu ");
   menu = new Menu(this, PG_Rect(15,0,Width()-200,20));

   dataLoaderTicker();
   SetID( ASC_PG_App::mainScreenID );

   
   weaponRangeLayer = new UnitWeaponRangeLayer();
   mapDisplay->addMapLayer( weaponRangeLayer, "weaprange" );

   movementRangeLayer = new UnitMovementRangeLayer();
   mapDisplay->addMapLayer( movementRangeLayer, "moverange" );

   mapDisplay->addMapLayer( new VisibilityLayer(), "visibilityvalue" );

   mapDisplay->layerChanged.connect( sigc::mem_fun( *this, &ASC_MainScreenWidget :: mapLayerChanged));

   // making a copy, because the activateMapLayer will modify the visibleMapLayer object
   vector<ASCString> layer = CGameOptions::Instance()->visibleMapLayer;
   for ( vector<ASCString>::iterator i = layer.begin(); i != layer.end(); ++i ) 
      mapDisplay->activateMapLayer(*i,true);
   


   int counter = 0;
   for ( CGameOptions::PanelDataContainer::iterator i = CGameOptions::Instance()->panelData.begin(); i != CGameOptions::Instance()->panelData.end(); ++i ) {
      if ( i->second.visible ) {
         if ( spawnPanel( i->first ))
            ++counter;
      }
   }

   if ( !counter ) {
      spawnPanel( UnitInfo );
      spawnPanel( ButtonPanel );
      spawnPanel( OverviewMap );
   }

   // to persuade the linker to really link that file
   uselessCallToTextRenderAddons();
}

void ASC_MainScreenWidget :: mapLayerChanged(bool b, const ASCString& name )
{
   mapDisplay->getActiveLayers( CGameOptions::Instance()->visibleMapLayer );
   CGameOptions::Instance()->setChanged();
}



void ASC_MainScreenWidget :: lockStandardActions( int dir, int options )
{
   standardActionsLocked += dir;
   
   if( dir > 0 ) { 
      lockOptionStack.push_back( lockOptions );
      lockOptions = options;
      lockOptionsChanged( options );
   }
   if ( dir < 0 ) {
      lockOptions = lockOptionStack.back();
      lockOptionStack.pop_back();
      lockOptionsChanged( lockOptions );
   }
   
   if ( menu ) {
      if ( standardActionsLocked <= 0 )
         menu->Show();
      else
         menu->Hide();
   }
}





void displaymessage2( const char* formatstring, ... )
{

   ASCString s;
   
    std::va_list arg_ptr;
    va_start ( arg_ptr, formatstring );

    s.vaformat( formatstring, arg_ptr );

    va_end ( arg_ptr );

    if ( mainScreenWidget )
       mainScreenWidget->displayMessage(s);
}



bool ASC_MainScreenWidget::spawnPanel ( const ASCString& panelName )
{
   if ( panelName == "WindInfo" ) {
      spawnPanel ( WindInfo );
      return true;
   }

   if ( panelName == "UnitInfo" ) {
      spawnPanel ( UnitInfo );
      return true;
   }
   
   if ( panelName == "GuiIcons" ) {
      spawnPanel ( ButtonPanel );
      return true;
   }

   if ( panelName == "OverviewMap" ) {
      spawnPanel ( OverviewMap );
      return true;
   }

   if ( panelName == "MapInfo" ) {
      spawnPanel ( MapControl );
      return true;
   }

   if ( panelName == "ActionInfo" ) {
      spawnPanel ( ActionInfo );
      return true;
   }
   
   return false;
}

void ASC_MainScreenWidget::spawnPanel ( Panels panel )
{
   if ( panel == WindInfo ) {
      if ( !windInfoPanel || !CGameOptions::Instance()->cacheASCGUI ) {
         delete windInfoPanel;
         windInfoPanel = new WindInfoPanel( this, PG_Rect(Width()-170, 480, 170, 114));
      }
      windInfoPanel->Show();
   }
   if ( panel == UnitInfo ) {
      if ( !unitInfoPanel || !CGameOptions::Instance()->cacheASCGUI ) {
         delete unitInfoPanel;
         unitInfoPanel = new UnitInfoPanel( this, PG_Rect(Width()-170, 160, 170, 320));
      }
      unitInfoPanel->Show();
   }
   if ( panel == ButtonPanel ) {
      if ( !guiHost || !CGameOptions::Instance()->cacheASCGUI ) {
         delete guiHost;
         guiHost = new NewGuiHost( this, mapDisplay, PG_Rect(Width()-170, Height()-200, 170, 200));
         guiHost->pushIconHandler( &GuiFunctions::primaryGuiIcons );
      }
      guiHost->Show();
   }
   
   if ( panel == OverviewMap ) 
      spawnOverviewMapPanel();
   
   if ( panel == MapControl ) {
      if ( !mapInfoPanel || !CGameOptions::Instance()->cacheASCGUI ) {
         delete mapInfoPanel;
         mapInfoPanel = new MapInfoPanel( this, PG_Rect(Width()-170, 0, 170, 135), mapDisplay );
      }
      mapInfoPanel->Show();
   }
   
   if ( panel == ActionInfo ) {
      if ( !actionInfoPanel || !CGameOptions::Instance()->cacheASCGUI ) {
         delete actionInfoPanel;
         actionInfoPanel = new ActionInfoPanel( this, PG_Rect(Width()-170, 0, 170, 160) );
      }
      actionInfoPanel->Show();
      actionInfoPanel->update( actmap );
   }
}


void ASC_MainScreenWidget::showMovementRange( GameMap* gamemap, const MapCoordinate& pos )
{
   movementRangeLayer->operateField( gamemap, pos );
}

void ASC_MainScreenWidget::showWeaponRange( GameMap* gamemap, const MapCoordinate& pos )
{
   weaponRangeLayer->operateField( gamemap, pos );
}



bool ASC_MainScreenWidget::eventKeyDown(const SDL_KeyboardEvent* key)
// bool Menu::eventKeyDown(const SDL_KeyboardEvent* key)
{
   if ( standardActionsLocked > 0  )
      return false;
   
   int mod = SDL_GetModState() & ~(KMOD_NUM | KMOD_CAPS | KMOD_MODE);

   if ( !mod  ) {
      switch ( key->keysym.sym ) {
            case SDLK_ESCAPE:
               executeUserAction( ua_mainmenu );
               return true;
               
            case SDLK_F1:
               executeUserAction ( ua_help );
               return true;

            case SDLK_F3:
               executeUserAction ( ua_continuenetworkgame );
               return true;

            case SDLK_F4:
               executeUserAction ( ua_networksupervisor );
               return true;

            case SDLK_F8:
               {
                  int color = actmap->actplayer;
                  for ( int p = 0; p < 8; p++ )
                     if ( actmap->player[p].stat == Player::computer && actmap->player[p].exist() )
                        color = p;

                  if ( actmap->player[color].ai ) {
                     AI* ai = (AI*) actmap->player[color].ai;
                     ai->showFieldInformation ( actmap->getCursor().x, actmap->getCursor().y );
                  }
               }
               return true;

            case SDLK_1:
               executeUserAction ( ua_changeresourceview );
               return true;

            case SDLK_2:
               executeUserAction ( ua_toggleunitshading );
               return true;

            case SDLK_3:
               executeUserAction( ua_viewunitweaponrange );
               return true;

            case SDLK_4:
               executeUserAction( ua_viewunitmovementrange );
               return true;

               /*
            case SDLK_5:
               execuseraction ( ua_GameStatus );
               return true;
               */

            case SDLK_5: executeUserAction ( ua_showCargoLayer );
               return true;

               /*
            case SDLK_6:
               execuseraction ( ua_UnitSetInfo );
               return true;
               */

            case SDLK_7:
               executeUserAction ( ua_viewterraininfo );
               return true;

            case SDLK_8:
               executeUserAction ( ua_unitweightinfo );
               return true;

            case SDLK_9:
               executeUserAction ( ua_viewPipeNet );
               return true;

            case SDLK_0: executeUserAction( ua_visibilityInfo );
               return true;

            case SDLK_BACKSPACE:               
               executeUserAction( ua_undo );
               return true;


            case SDLK_PLUS:   
            case SDLK_KP_PLUS: executeUserAction( ua_increase_zoom );
               return true;

            case SDLK_MINUS:   
            case SDLK_KP_MINUS: executeUserAction( ua_decrease_zoom );
               return true;
               
               case SDLK_F10: {
                  
                  PG_ScrollBar* sb = new PG_ScrollBar( NULL, PG_Rect( 20, 50, 15, 400), PG_ScrollBar::VERTICAL, -1, "DLGScrollbar" );
                  sb->SetPageSize(10);
                  sb->SetRange(0, 100);
                  sb->Show();
                  
               }
            // testText();
            break;
               
            default:;
      }
   }

   if ( (mod & KMOD_SHIFT) && (mod & KMOD_CTRL)) {
      switch ( key->keysym.sym ) {
            case SDLK_F11: {
               return true;
            }
            case SDLK_F10: {
               return true;
            }
         case SDLK_l:
               executeUserAction ( ua_loadrecentgame );
               return true;

            case SDLK_LEFT:
               executeUserAction( ua_turnUnitLeft );
               return true;

            case SDLK_RIGHT:
               executeUserAction( ua_turnUnitRight );
               return true;

            default:;
      }
      switch ( key->keysym.unicode ) {
            case 26:
               executeUserAction( ua_redo );
               return true;
               
            default:;
      }
   }

   if ( mod & KMOD_CTRL ) {
      switch ( key->keysym.sym ) {

            case SDLK_0: executeUserAction( ua_writescreentopcx );
               return true;

            case SDLK_1: executeUserAction( ua_viewUnitinfoOverlay );
              return true;

            case SDLK_2: executeUserAction( ua_viewUnitexperienceOverlay );
              return true;

            case SDLK_F10: {
               SDL_Surface* s = PG_Application::GetScreen();
               SDL_LockSurface( s );
               int* i = (int*) (s->pixels);
               for ( int x = 0; x < 100; ++x )
                  i[x * s->pitch/4 + x] = 0xffffff;
               SDL_UnlockSurface( s );
               SDL_UpdateRect(s,0,0,0,0);
               return true;
            }

            
            default:;
      }
      switch ( key->keysym.unicode ) {
            case 3: // C
               executeUserAction ( ua_cargosummary );
               return true;

            case 6: // F
               executeUserAction ( ua_unitsummary );
               return true;

            case 7: // G
               executeUserAction( ua_gotoPosition );
               return true;

            case 12: // L
               executeUserAction ( ua_loadgame );
               return true;

            case 14: // N
               executeUserAction ( ua_newGame );
               return true;

            case 17: // Q
               executeUserAction ( ua_exitgame );
               return true;

            case 18: // R
               executeUserAction( ua_createReminder );
               return true;
               
            case 19: // S
               executeUserAction ( ua_savegame );
               return true;

            case 20: // T
               executeUserAction ( ua_runOpenTasks );
               return true;
               
            case 26: // Z
               executeUserAction( ua_undo );
               return true;

      }
   }

   if ( mod & KMOD_ALT ) {
      switch ( key->keysym.sym ) {
            case SDLK_RETURN:
               getPGApplication().toggleFullscreen();
               return true;

            default:;
      }
   }

   if ( mod & KMOD_SHIFT ) {
      switch ( key->keysym.sym ) {
            case SDLK_F3:
               executeUserAction ( ua_continuerecentnetworkgame );
               return true;

            default:;
      }
   }
   return false;
}
