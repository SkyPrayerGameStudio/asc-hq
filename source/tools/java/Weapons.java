//     $Id: Weapons.java,v 1.4 2009-04-18 13:48:40 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.3  2000/11/07 16:19:41  schelli
//     Minor Memory-Functions & Problems changed
//     Picture Support partly enabled
//     New Funktion partly implemented
//     Buildings partly implemented
//
//     Revision 1.2  2000/10/13 13:15:48  schelli
//     *** empty log message ***
//

/*
 * tWeapons.java
 *
 * Created on 22. November 1999, 14:34
  
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-2000  Martin Bickel  and  Marc Schellenberger

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

public class Weapons extends Object {
  int          weaponcount; //c
  Weapon      weapon[]; 

  /** Creates new tWeapons */
  public Weapons() {
    weaponcount = 0;
    weapon = new Weapon[8];  
  }
  
}