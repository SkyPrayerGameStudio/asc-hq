//     $Id: global_os.h,v 1.1 2004-07-12 18:15:10 mbickel Exp $
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

#ifndef global_os_h_included
 #define global_os_h_included

  #ifndef minimalIO
   // #define sdlheader "SDL.h"
   // #define sdlmixerheader "SDL_mixer.h"
   // #include sdlheader
  #endif
//   #include <string>  // some Standard library include file to define the namespace std
  using namespace std;
  #define HAVE_STRICMP
  #define HAVE_ITOA
  #define HAVE_DIRENT_H
  #define HAVE_STDIO_H
  #define StaticClassVariable
  #define NoStdio

 #define CASE_SENSITIVE_FILE_NAMES 0
 #define USE_HOME_DIRECTORY 0


#endif