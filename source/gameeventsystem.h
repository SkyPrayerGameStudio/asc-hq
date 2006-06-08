/*! \file gameeventsystem.h
    \brief Interface to the event handling of ASC
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


#ifndef gameeventsystemH
#define gameeventsystemH

#include <sigc++/sigc++.h>

#include "typen.h"
#include "libs/loki/Singleton.h"
#include "factory.h"

#include "mapdisplayinterface.h"

class MapDisplayInterface;

enum  EventConnections { cconnection_destroy = 1,
                         cconnection_conquer = 2,
                         cconnection_lose  = 4,
                         cconnection_seen = 8,
                         cconnection_areaentered_anyunit = 16,
                         cconnection_areaentered_specificunit = 32 };


extern bool  checkevents( MapDisplayInterface* md );

extern void  checktimedevents( MapDisplayInterface* md );

extern void eventReady();

extern const int EventActionNum;
extern const int EventTriggerNum;
extern const char* EventTriggerName[];
extern const char* EventActionName[];

typedef int EventTriggerID;
typedef int EventActionID;

class Event;

class EventTrigger {
      EventTriggerID triggerID;
   public:
      enum State { unfulfilled, fulfilled, finally_fulfilled, finally_failed };
   protected:
      EventTrigger ( EventTriggerID id ) : triggerID ( id ), gamemap(NULL), event(NULL), stateCache(unfulfilled), triggerFinal( false ), invert(false) {};
      virtual State getState( int player ) = 0;
      GameMap* gamemap;
      Event* event;

      bool isFulfilled();
   private:
      State stateCache;
      bool triggerFinal;
   public:
      bool invert;
      //! takes the inversion into account, which getState(int) does not
      State state( int player );

      virtual void readData ( tnstream& stream ) = 0;
      virtual void writeData ( tnstream& stream ) = 0;
      
      virtual ASCString getName() const = 0;
      virtual void setup() = 0;
      virtual void arm() {};
      void setMap( GameMap* gamemap_ ) { gamemap = gamemap_; };
      void setEvent( Event* ev ) { event = ev; };
      EventTriggerID getTriggerID() { return triggerID; };

      virtual ~EventTrigger(){};

      // friend Event* readOldEvent( pnstream stream );
};

class EventAction {
      EventActionID actionID;
   protected:
      GameMap* gamemap;
      EventAction( EventActionID id ) : actionID ( id ), gamemap(NULL) {};
   public:

      virtual void readData ( tnstream& stream ) = 0;
      virtual void writeData ( tnstream& stream ) = 0;
      virtual ASCString getName();

      virtual void execute( MapDisplayInterface* md ) = 0;
      virtual void setup() = 0;
      void setMap( GameMap* gamemap_ ) { gamemap = gamemap_; };
      EventActionID getActionID() { return actionID; };
      virtual ~EventAction() {};
};

class Event {
      GameMap& gamemap;

      void clear();
   public:
      Event( GameMap& map_ );
      const GameMap* getMap() const { return &gamemap; };

      enum Status { Untriggered, Triggered, Timed, Executed } status;

      typedef vector<EventTrigger*> Trigger;
      Trigger trigger;

      int id;
      int playerBitmap; 
      ASCString  description;
      GameTime   triggerTime;
      struct Delayedexecution {
         Delayedexecution():turn(0),move(0){};
         int turn;
         int move;   // negative values allowed !!
      } delayedexecution;

      //! the number of times this event can be executed; makes only sense in cunjunction with delayedexecution
      int reArmNum;

      EventAction* action;
      enum TriggerConnection { AND, OR } triggerConnection;

      void check( MapDisplayInterface* md );
      void execute( MapDisplayInterface* md );
      void spawnAction( EventActionID eai );
      EventTrigger* spawnTrigger( EventTriggerID eti );

      Event& operator= ( const Event& ev  );

      void read ( tnstream& stream );
      void write ( tnstream& stream );

      void arm();

      SigC::Signal0<void> executed;

      virtual ~Event();
};

class VariableLocker {
   bool& var;
   public:
      VariableLocker( bool& variable ) : var ( variable ) { var = true; };
      ~VariableLocker() { var = false; };
};



typedef Loki::SingletonHolder< Factory< EventTrigger, EventTriggerID > > triggerFactory;
typedef Loki::SingletonHolder< Factory< EventAction , EventActionID  > > actionFactory;



// }; // namespace GameEvents

#endif
