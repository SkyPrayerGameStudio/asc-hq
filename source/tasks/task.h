/*
     This file is part of Advanced Strategic Command; http://www.asc-hq.de
     Copyright (C) 1994-2008  Martin Bickel  and  Marc Schellenberger
 
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


#ifndef taskH
#define taskH

#include "../actions/context.h"
#include "../actions/actionresult.h"

#include "../basestreaminterface.h"

typedef int TaskID;


/** T task is a operation that the user (or the AI) initiates.
    Tasks may take several turns to complete.
    During Execution they will generate one or more GameAction objects,
    which encapsulate the modifications that are done to the map and which 
    serve as the foundation for doing undo / redo 
*/
class Task {
   private:
      GameMap* gamemap;
   public:
      enum State { Planned, Evaluated, SetUp, Completed, Failed };
   private:
      State state;
   public:
      State getState() const { return state; };
      
      virtual TaskID getID() const = 0;
      
      virtual ActionResult go ( Context& context ) = 0; 
      
      void read ( tnstream& stream );
      void write ( tnstream& stream );
      
      virtual ~Task(){};
      
      ASCString getCommandString();
      
   protected:
      Task( GameMap* gamemap );
      
      virtual void readData ( tnstream& stream ) = 0;
      virtual void writeData ( tnstream& stream ) = 0;
      
      GameMap* getMap();
};

#endif

