/***************************************************************************
                          mapdisplay.h  -  description
                             -------------------
    begin                : Wed Jan 24 2001
    copyright            : (C) 2001 by Martin Bickel
    email                : bickel@asc-hq.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef mapdisplayH
 #define mapdisplayH


#include <sigc++/sigc++.h>
#include "libs/loki/Functor.h"

 
#include "typen.h"
#include "vehicle.h"
#include "basegfx.h"
#include "events.h"


#include "paradialog.h"
#include "mapdisplayinterface.h"


class MapDisplay : public MapDisplayInterface {
           dynamic_array<int> cursorstat;
           int cursorstatnum;
         public:
           int displayMovingUnit ( const MapCoordinate3D& start, const MapCoordinate3D& dest, Vehicle* vehicle, int fieldnum, int totalmove, SoundStartCallback soundStart );
           void displayMap ( void );
           void displayMap ( Vehicle* additionalVehicle ) {};
           void displayPosition ( int x, int y );
           void resetMovement ( void );
           void startAction ( void );
           void stopAction ( void );
           void displayActionCursor ( int x1, int y1, int x2, int y2 ) {};
           void removeActionCursor ( void ) {};
           void updateDashboard ();
           void repaintDisplay ();
    };

class tlockdispspfld {
      public:
        tlockdispspfld ( void );
        ~tlockdispspfld ();
      };


class MapLayer;

class MapRenderer {
    
      struct Icons {
         Surface mapBackground;
         Surface notVisible;
         Surface markField;
         Surface markFieldDark;
      };
      static Icons icons;

      typedef deallocating_vector<MapLayer*> LayerRenderer;
      LayerRenderer layerRenderer;
     
      void readData();

    public:  
      class FieldRenderInfo {
            public:
               FieldRenderInfo( Surface& s, tmap* actmap) : surface(s), gamemap(actmap ) {};
               Surface& surface;
               VisibilityStates visibility;
               int playerView;
               pfield fld;
               MapCoordinate pos;
               tmap* gamemap;
      };

    protected:
      MapRenderer() { readData(); };

      static const int surfaceBorder = 60;
            
      struct ViewPort {
         ViewPort( int x1, int y1, int x2, int y2 );
         ViewPort();
         int x1,y1,x2,y2;
      };

      // return the position of a field on the internal surface      
      int getFieldPosX( int x, int y ) { 
          if (y & 1 )   // even lines are shifted to the right
             return surfaceBorder + fielddisthalfx + x * fielddistx;
          else
             return surfaceBorder + x * fielddistx;
      };             
      
      // return the position of a field on the internal surface      
      int getFieldPosY( int x, int y ) {
          return surfaceBorder + y * fielddisty;
      };    
      
      SPoint getFieldPos( int x, int y ) {
          return SPoint( getFieldPosX(x,y), getFieldPosY(x,y));
      };
      SPoint getFieldPos2( const MapCoordinate& pos ) {
          return SPoint( getFieldPosX(pos.x,pos.y), getFieldPosY(pos.x,pos.y));
      };
      
      void paintSingleField( const MapRenderer::FieldRenderInfo& fieldInfo,  int layer, const SPoint& pos );
      void paintTerrain( Surface& surf, tmap* actmap, int playerView, const ViewPort& viewPort, const MapCoordinate& offset );
      
      int bitmappedHeight2pass( int height );
      
      SigC::Signal2<void,Surface&,int> additionalItemDisplayHook;
      
      void addMapLayer( MapLayer* mapLayer ) { layerRenderer.push_back( mapLayer ); };
      
};


class MapLayer {
      bool active;
   protected:
      MapLayer() : active(true) {};
   public:
      virtual bool onLayer( int layer ) = 0;
      bool isActive() { return active; };
      virtual void setActive( bool active ) { this->active = active; };
      virtual void paintSingleField( const MapRenderer::FieldRenderInfo& fieldInfo,  int layer, const SPoint& pos ) = 0;
      virtual ~MapLayer() {};
};








class MapDisplayPG: public PG_Widget, protected MapRenderer {

      typedef map<ASCString,MapLayer*> LayerMap;
      LayerMap layerMap;
      
      struct Icons {
         Surface cursor;
         Surface fieldShape;
      };
      static Icons icons;

      void readData();

      static MapDisplayPG* theMapDisplay;
      
            
      int zoom;
      Surface* surface;
      
      MapCoordinate offset;
     
      void setupFastBlitterCommands();
      struct Field {
         ViewPort viewPort;
         int numx, numy;
      } field;   
      

      void displayCursor( const PG_Rect& dst );
      void displayCursor();
      
      enum Dirty { Nothing, Curs, Map } dirty;
      
      Vehicle* additionalUnit; 
      
   protected:
   
      // Paragui stuff
      void eventBlit(SDL_Surface* srf, const PG_Rect& src, const PG_Rect& dst);
      // void eventDraw(SDL_Surface* surface, const PG_Rect& rect);


      bool eventMouseButtonDown (const SDL_MouseButtonEvent *button);
      bool eventMouseMotion (const SDL_MouseMotionEvent *button);
      bool eventMouseButtonUp (const SDL_MouseButtonEvent *button);
      
      //
   
      void checkViewPosition();
      
      void fillSurface( int playerView );


   public:
      void setNewZoom( int zoom );
      int getZoom() const { return zoom; };
      SigC::Signal1<void, int> newZoom;
      
      MapCoordinate screenPos2mapPos( const SPoint& pos );
      MapCoordinate widgetPos2mapPos( const SPoint& pos );

      SPoint mapGlobalPos2internalPos ( const MapCoordinate& pos );
      SPoint mapViewPos2internalPos ( const MapCoordinate& pos );
      SPoint internal2widget( const SPoint& pos );
      SPoint widget2screen( const SPoint& pos );
      
      MapCoordinate upperLeftCorner();
      MapCoordinate lowerRightCorner();
      
   private:
      SPoint upperLeftSourceBlitCorner;
   protected:

      
      void blitInternalSurface( SDL_Surface* dest, const SPoint& pnt, const PG_Rect& dstClip );

      
      static const int effectiveMovementSurfaceWidth = 4 * fielddisthalfx + fieldsizex;
      static const int effectiveMovementSurfaceHeight = 4*fieldsizey;
      
      struct MovementMask {
         Surface mask;
         SPoint  startFieldPos;
      } movementMask[sidenum];

      void displayAddons( Surface& surf, int pass);
      
      struct Movement {
         Vehicle* veh;
         SPoint from;
         SPoint to;
         int fromShadow;
         int toShadow;
         pmap actmap;
         Surface* mask;
         int playerView;

         PG_Rect blitViewPortInternal;
         PG_Rect blitViewPortScreen;
         SPoint maskPosition;
         SPoint targetBlitPos;

         static const int touchedFieldNum = 10;
         int actualFieldNum;
         struct {
            MapCoordinate mapPos;
            SPoint surfPos;
         } touchedFields[touchedFieldNum];

      };

      void initMovementStructure();
      Surface createMovementBufferSurface();
      void displayMovementStep( Movement& movement, int percentage );

      bool eventKeyDown(const SDL_KeyboardEvent* key)
      {
         return keyboardHandler( key );
      };

      bool keyboardHandler( const SDL_KeyboardEvent* keyEvent);
      
      bool disableKeyboardCursorMovement;
     
   public:   
      void keyboadCursorMovement( bool enable ) { disableKeyboardCursorMovement = !enable; }; 
      
      void scrollMap( int dir );
      
   private:
      void UpdateRect( const PG_Rect& rect );
      void moveCursor( int dir, int step );
      void redrawMapAtCursor ( const MapCoordinate& oldpos );

   public:

   

      MapDisplayPG ( PG_Widget *parent, const PG_Rect r );

      void displayUnitMovement( pmap actmap, Vehicle* veh, const MapCoordinate3D& from, const MapCoordinate3D& to );

      bool fieldInView(const MapCoordinate& mc );
      bool centerOnField( const MapCoordinate& mc );

      void registerAdditionalUnit ( Vehicle* veh );

      //! repaints to the internal surface, but does not blit this surface the screen
      void updateMap( bool force = false );

      //! update the internal surface and blits it to the screen
      void updateWidget();

      void addMapLayer( MapLayer* layer, const ASCString& name );
      void activateMapLayer( const ASCString& name, bool active );
      void toggleMapLayer( const ASCString& name );
      SigC::Signal2<void, bool, const ASCString&> layerChanged;

      /** Signal that is fired when the mouse is pressed on a valid field, after the cursor evaluation has been run.
           \param MapCoordinate the Coordinate of the field that was clicked
           \param SPoint the mouse position
           \param bool true if the cursor had been repositioned
      */
      SigC::Signal3<bool,const MapCoordinate&, const SPoint&, bool> mouseButtonOnField;

      /** Signal that is fired when the mouse is dragged onto a new field with mouse buttons pressed.
           \param MapCoordinate the Coordinate of the field that was clicked
           \param SPoint the mouse position
           \param bool true if the cursor had been repositioned
      */
      SigC::Signal3<bool,const MapCoordinate&, const SPoint&, bool> mouseDraggedToField;
      
      
      struct Cursor {
            int invisible;
            MapDisplayPG* mapDisplay; 
            Cursor( MapDisplayPG* md ) : invisible(0), mapDisplay(md) {};
            friend class MapDisplayPG;
            MapCoordinate& pos();
          public:
            void goTo( const MapCoordinate& position );
            void intoView();
      } cursor; 
            
      class CursorHiding {
         public:
            CursorHiding();
            ~CursorHiding();
      };
            
      friend class CursorHiding;

      ~MapDisplayPG ();
           
};

extern void benchMapDisplay();


#endif
