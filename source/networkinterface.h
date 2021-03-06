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



#ifndef networkinterfaceH
#define networkinterfaceH

#include "loki/Singleton.h"
#include "util/factory.h"

#include "basestrm.h"

class GameMap;

class GameTransferMechanism {
   protected:   
      virtual void readChildData ( tnstream& stream ) = 0;
      virtual void writeChildData ( tnstream& stream ) const = 0;
   public:
      virtual void setup() = 0;
      virtual void send( const GameMap* map, int lastPlayer, int lastturn ) = 0;
      virtual GameMap* receive() = 0;
      static GameTransferMechanism* read ( tnstream& stream );
      void write ( tnstream& stream ) const;
      virtual ASCString getMechanismID() const = 0;
      virtual ~GameTransferMechanism() {};
};

typedef Loki::SingletonHolder< Factory< GameTransferMechanism, ASCString > > networkTransferMechanismFactory;


#endif
