/***************************************************************************
                          paradialog.h  -  description
                             -------------------
    begin                : Thu Feb 21 2002
    copyright            : (C) 2002 by Martin Bickel
    email                : bickel@asc-hq.org
 ***************************************************************************/

/*! \file paradialog.h
    \brief Dialog classes based on the Paragui library
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef paradialogH
 #define paradialogH

 #ifndef NO_PARAGUI
 
#include "global.h"

#include <paragui.h>
#include <pgapplication.h>
#include <pgmessagebox.h>
#include <pgdropdown.h>
#include "pgbutton.h"
#include "pgwidgetlist.h"
#include "pglabel.h"
#include "pgwindow.h"
#include "pgmaskedit.h"
#include "pgscrollbar.h"
#include "pgprogressbar.h"
#include "pgradiobutton.h"
#include "pgthemewidget.h"
#include "pgcheckbutton.h"
#include "pgslider.h"
#include "pglistbox.h"
#include "pgcolumnitem.h"
#include "pgeventobject.h"
#include "pgpopupmenu.h"
#include "pgspinnerbox.h"
#include "pglog.h"
#include "pgmenubar.h"

#include "sdl/graphics.h"
#include "ascstring.h"

 class ASC_PG_App : public PG_Application {
       ASCString themeName;
    public:
       ASC_PG_App ( const ASCString& themeName );
       bool InitScreen ( int w, int h, int depth = 0, Uint32 flags = SDL_SWSURFACE|SDL_HWPALETTE );
       void reloadTheme();
      // PG_Theme* LoadTheme(const char* xmltheme, bool asDefault = true, const char* searchpath = NULL );
 };

 
 //! Adapter class for using Paragui Dialogs in ASC. This class transfers the event control from ASC to Paragui and back. All new dialog classes should be derived from this class
class ASC_PG_Dialog : public PG_Window {
       SDL_Surface* background;
    private:
       int quitModalLoopValue;
    protected:
       void quitModalLoop(int i = 1) { quitModalLoopValue = i; };    
    public:
       ASC_PG_Dialog ( PG_Widget *parent, const PG_Rect &r, const char *windowtext, WindowFlags flags=DEFAULT, const char *style="Window", int heightTitlebar=25);
       int Run( );
       ~ASC_PG_Dialog();
};

 
#endif

 extern void soundSettings();

#endif
