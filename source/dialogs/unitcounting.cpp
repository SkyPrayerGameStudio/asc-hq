/***************************************************************************
                          cargodialog.cpp  -  description
                             -------------------
    begin                : Tue Oct 24 2000
    copyright            : (C) 2000 by Martin Bickel
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

#include <pgimage.h>
#include <pgtooltiphelp.h>
#include "unitcounting.h"
#include "vehicletypeselector.h"
#include "../vehicle.h"
#include "../containerbase.h"
#include "../gamemap.h"
#include "../unitset.h"
#include "fileselector.h"



class VehicleCounterFactory: public SelectionItemFactory, public sigc::trackable  {
public:
    typedef vector<const VehicleType*> Container;
protected:
    Container::iterator it;
    Container items;

    typedef map<const VehicleType*,int> Counter;
    Counter counter;
    GameMap* gamemap;

    void calcCargoSummary( const ContainerBase* cb, Counter& summary )
    {
        for ( ContainerBase::Cargo::const_iterator i = cb->getCargo().begin(); i != cb->getCargo().end(); ++i )
            if ( *i ) {
                calcCargoSummary( *i, summary );
                if ( (*i)->getOwner() == cb->getMap()->actplayer )
                    summary[ (*i)->typ] += 1;
            }
    }


public:
    VehicleCounterFactory( GameMap* actmap );
    VehicleCounterFactory( const ContainerBase* container );

    ASCString toString();

    void restart();
    SelectionWidget* spawnNextItem( PG_Widget* parent, const PG_Point& pos );
    void itemSelected( const SelectionWidget* widget, bool mouse ) {}
};

VehicleCounterFactory :: VehicleCounterFactory( GameMap* actmap ) : gamemap ( actmap )
{
    for ( int y = 0; y < actmap->ysize; ++y )
        for ( int x = 0; x < actmap->xsize; ++x ) {
            MapField* fld = actmap->getField(x,y);
            if ( fld ) {
                if ( fld->vehicle ) {
                    calcCargoSummary( fld->vehicle, counter );
                    if ( fld->vehicle->getOwner() == actmap->actplayer )
                        counter[ fld->vehicle->typ] += 1;
                }
                if ( fld->building && (fld->bdt & getTerrainBitType(cbbuildingentry)).any() )
                    calcCargoSummary( fld->getContainer(), counter );
            }
        }

    for ( Counter::iterator i = counter.begin(); i != counter.end(); ++i )
        items.push_back( i->first );

    sort( items.begin(), items.end(), vehicleComp );
    restart();
};


VehicleCounterFactory::VehicleCounterFactory( const ContainerBase* container ) : gamemap( container->getMap() )
{
    calcCargoSummary( container, counter );
    for ( Counter::iterator i = counter.begin(); i != counter.end(); ++i )
        items.push_back( i->first );

    sort( items.begin(), items.end(), vehicleComp );
    restart();
}



void VehicleCounterFactory::restart()
{
    it = items.begin();
};


SelectionWidget* VehicleCounterFactory::spawnNextItem( PG_Widget* parent, const PG_Point& pos )
{
    if ( it != items.end() ) {
        const VehicleType* v = *(it++);
        return new VehicleTypeCountWidget( parent, pos, parent->Width() - 15, v, gamemap->getCurrentPlayer(), counter[v] );
    } else
        return NULL;
};

ASCString VehicleCounterFactory::toString()
{
    ASCString t;
    for ( Counter::const_iterator i = counter.begin(); i != counter.end(); ++i ) {
        const VehicleType* v = i->first;
        t += v->getName() + "\t" + ASCString::toString(i->second) + "\n";
    }
    return t;
}


/*
void VehicleCounterFactory::itemSelected( const SelectionWidget* widget, bool mouse )
{
   if ( !widget )
      return;

   const VehicleTypeBaseWidget* fw = dynamic_cast<const VehicleTypeBaseWidget*>(widget);
   assert( fw );
}
*/

#include "fieldmarker.h"

struct CursorDistSorter : public binary_function<const MapCoordinate&, const MapCoordinate&, bool> {
    CursorDistSorter( const MapCoordinate& cursorPos) : cursor( cursorPos ) {};
    bool operator()(const MapCoordinate& x, const MapCoordinate& y) {
        return beeline(x,cursor) < beeline(y,cursor);
    }
private:
    MapCoordinate cursor;
};


void showAllUnitPositions( const VehicleType* vt, GameMap* gamemap )
{
    Player& player = gamemap->getPlayer( gamemap->actplayer );

    SelectFromMap::CoordinateList coordinates;

    for ( Player::VehicleList::iterator i = player.vehicleList.begin(); i != player.vehicleList.end(); ++i ) {
        if ( (*i)->typ == vt )
            if ( find ( coordinates.begin(), coordinates.end(),(*i)->getPosition() ) == coordinates.end() )
                coordinates.push_back ( (*i)->getPosition() );
    }

    sort( coordinates.begin(), coordinates.end(), CursorDistSorter( gamemap->getCursor()) );

    SelectFromMap sfm( coordinates, gamemap, false, true );
    sfm.Show();
    sfm.RunModal();
}

class UnitSummaryWindow : public ItemSelectorWindow {
private:
    GameMap* gamemap;
    VehicleCounterFactory* factory;

    virtual void itemSelected( const SelectionWidget* sw) {
        const VehicleTypeCountWidget* vtcw = dynamic_cast<const VehicleTypeCountWidget*>(sw);
        assert( vtcw );

        if ( gamemap ) {
            Hide();
            showAllUnitPositions( vtcw->getVehicletype(), gamemap );
            Show();
        }
    };


    void saveText()
    {
        ASCString name = selectFile( "*.txt", false );
        if ( !name.empty() ) {
            tn_file_buf_stream s( name, tnstream::writing );
            s.writeString( factory->toString(), true );
        }
    }

    bool eventKeyDown(const SDL_KeyboardEvent* key) {
        int mod = SDL_GetModState() & ~(KMOD_NUM | KMOD_CAPS | KMOD_MODE);
        if ( mod & KMOD_CTRL )
            if ( key->keysym.sym == SDLK_s )
                saveText();

        return ItemSelectorWindow::eventKeyDown( key );
    }

public:
    UnitSummaryWindow ( PG_Widget *parent, const PG_Rect &r , const ASCString& title, VehicleCounterFactory* itemFactory, GameMap* actmap ) : ItemSelectorWindow( parent, r, title, itemFactory ), gamemap( actmap ), factory( itemFactory ) {};
};


void showUnitCargoSummary( ContainerBase* cb )
{
    UnitSummaryWindow isw( NULL, PG_Rect( -1, -1, 500, 700 ),  "cargo summary", new VehicleCounterFactory( cb ), NULL );
    isw.Show();
    isw.RunModal();
}


void showUnitSummary( GameMap* actmap )
{
    UnitSummaryWindow isw( NULL, PG_Rect( -1, -1, 500, 700 ),  "unit summary", new VehicleCounterFactory( actmap ), actmap );
    isw.Show();
    isw.RunModal();
}
