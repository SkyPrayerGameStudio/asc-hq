//     $Id: edevents.h,v 1.2 1999-11-16 03:41:29 tmwilson Exp $
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

class   tstringselect : public tdialogbox {
                public :
                     char txt[200];
                     boolean  ok;
                     int sy,ey,sx,ex,action,dx;
                     byte dk;
                     int msel,mouseselect,redline,lnshown,numberoflines,firstvisibleline,startpos;
                     boolean scrollbarvisible;
                     void init(void);
                     virtual void setup(void);
                     virtual void run(void);
                     virtual void buttonpressed(byte id);
                     void scrollbar_on(void);
                     void viewtext(void);
                     virtual void gettext(word nr);
                     virtual void resettextfield(void);
                     void done(void);
                     };

#define clear_and_or 124

extern void         event(void);
extern void setmapparameters( void );

