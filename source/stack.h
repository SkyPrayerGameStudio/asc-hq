/*! \file stack.h
    \brief Interface for a small general-purpose stack (not type safe)
*/

//     $Id: stack.h,v 1.6 2009-04-18 13:48:38 mbickel Exp $
//
//     $Log: not supported by cvs2svn $
//     Revision 1.5  2001/01/28 14:04:20  mbickel
//      Some restructuring, documentation and cleanup
//      The resource network functions are now it their own files, the dashboard
//       as well
//      Updated the TODO list
//
//     Revision 1.4  2001/01/21 12:48:36  mbickel
//      Some cleanup and documentation
//
//     Revision 1.3  2000/01/04 19:43:54  mbickel
//      Continued Linux port
//
//     Revision 1.2  1999/11/16 03:42:38  tmwilson
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

#ifndef stack_h_included
 #define stack_h_included
 #include <SDL_stdinc.h>

 #define npush(a) pushdata( (Uint8*)&a, sizeof ( a ) )
 #define npop(a) popdata ( (Uint8*)&a, sizeof ( a ) )

 extern void  pushdata(Uint8 *       daten, int          size);

 extern void  popdata(Uint8 *       daten, int          size);

 extern int   stackfree(void);

 class fatalstackerror {};
#endif
