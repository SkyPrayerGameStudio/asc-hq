/***************************************************************************
                          containerbasetype.h  -  description
                             -------------------
    begin                : Sun Feb 18 2001
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

#include "containerbasetype.h"
#include "textfiletags.h"
#include "textfile_evaluation.h"
#include "vehicletype.h"
#include "graphics/blitter.h"


const char*  ccontainerfunctions[ContainerBaseType::functionNum+1]  =
              { "training",
                "internal vehicle production",
                "ammunition production",
                "internal unit repair",
                "recycling",
                "research",
                "sonar",
                "view satellites",
                "view mines",
                "wind power plant",
                "solar power plant",
                "matter converter",
                "mining station",
                "construct units that cannot move out",
                "resource sink",
                "external energy transfer",
                "external material transfer",
                "external fuel transfer",
                "external ammo transfer",
                "external repair",
                "no object chaining",
                "self destruct on conquer",
               "paratrooper",
               "mine-layer",
               "cruiser landing",
               "conquer buildings",
               "move after attack",
               "external vehicle production",
               "construct specific buildings",
               "icebreaker",
               "cannot be refuelled in air",
               "makes tracks",
               "search for mineral resources automatically",
               "no reactionfire",
               "auto repair",
               "Kamikaze only",
               "immune to mines",
               "jams only own field",
               "move with reaction fire on",
               "only move to and from transports",
               "AutoHarvestObjects",
               "No customization of production",
               "Manual self-destruct",
               "No removal of mines",
               "No rotation of image",
               NULL };


ContainerBaseType :: ContainerBaseType ()
{
   maxLoadableUnits = 0;
   maxLoadableUnitSize = 0;
   maxLoadableWeight = maxint;
   vehicleCategoriesStorable = -1;
   id = 0;
   jamming = 0;
   view = 0;
   efficiencyfuel = 1024;
   efficiencymaterial = 1024;

   maxresearchpoints = 0;
   defaultMaxResearchpoints = 0;
   nominalresearchpoints = 0;
   vehicleCategoriesProduceable = 0xfffffff;
   autoHarvest.range = 0;
   autoHarvest.maxFieldsPerTurn = maxint;
   minFieldRepairDamage = 0;
}

bool ContainerBaseType::hasFunction( ContainerFunctions function ) const
{
   int f = int(function);
   return features.test( f );
}

bool ContainerBaseType::hasAnyFunction( BitSet functions ) const
{
   bool result = (features & functions).any(); 
   return result;
}

void ContainerBaseType::setFunction( ContainerFunctions function )
{
   features.set( int(function) );
}

           


const char* ContainerBaseType::getFunctionName( ContainerFunctions function )
{
   if ( function < functionNum )
      return ccontainerfunctions[function];
   else
      return NULL;
}


ContainerBaseType::TransportationIO::TransportationIO()
{
  mode = 0;
  height_abs = 0;
  height_rel = 0;
  container_height = 0;
  vehicleCategoriesLoadable = -1;
  dockingHeight_abs = 0;
  dockingHeight_rel = 0;
  disableAttack = false;
  movecost = -1;
}


void ContainerBaseType :: TransportationIO :: runTextIO ( PropertyContainer& pc )
{
   pc.addTagInteger( "Mode", mode, entranceModeNum, entranceModes );
   pc.addTagInteger( "UnitHeightAbs", height_abs, choehenstufennum, heightTags );
   pc.addInteger( "UnitHeightRel", height_rel, -100 );
   pc.addTagInteger( "ContainerHeight", container_height, choehenstufennum, heightTags );
   pc.addTagInteger( "CategoriesNOT", vehicleCategoriesLoadable, cmovemalitypenum, unitCategoryTags, true );
   pc.addTagInteger( "DockingHeightAbs", dockingHeight_abs, choehenstufennum, heightTags, 0 );
   pc.addInteger( "DockingHeightRel", dockingHeight_rel, -100 );
   if ( pc.find( "RequireUnitFunction" )) {
      int r = 0;
      pc.addTagInteger( "RequireUnitFunction", r, VehicleType::legacyVehicleFunctionNum, vehicleAbilities, 0 );
      requiresUnitFeature = VehicleType::convertOldFunctions(r, pc.getFileName() );
   } else
      if ( pc.find( "RequiresUnitFeature" ) || !pc.isReading() )
         pc.addTagArray( "RequiresUnitFeature", requiresUnitFeature, ContainerBaseType::functionNum, containerFunctionTags );
      else
         requiresUnitFeature.reset();
      
   pc.addBool( "DisableAttack", disableAttack, false );
   pc.addInteger( "MoveCost", movecost, -1 );
   if ( movecost < 10 && movecost >= 0 )
      fatalError ( "MoveCost for TransportationIO is lower than 10! file: " + pc.getFileName() );
}


void ContainerBaseType :: runTextIO ( PropertyContainer& pc )
{
   pc.addBreakpoint();
   pc.openBracket ( "Transportation" );
    int num = entranceSystems.size();
    pc.addInteger ( "EntranceSystemNum", num, 0 );
    entranceSystems.resize(num);
    for ( int i = 0; i < num; i++ ) {
       pc.openBracket ( ASCString("EntranceSystem") + strrr(i) );
       entranceSystems[i].runTextIO( pc );
       pc.closeBracket();
    }
    pc.addInteger ( "MaxLoadableUnits", maxLoadableUnits, 0 );

    pc.addInteger ( "MaxLoadableUnitSize", maxLoadableUnitSize, maxint );
    pc.addInteger ( "MaxLoadableMass", maxLoadableWeight, maxint );
    pc.addTagInteger( "CategoriesNOT", vehicleCategoriesStorable, cmovemalitypenum, unitCategoryTags, -1, true );
   pc.closeBracket();

   pc.addString( "Name", name );
   pc.addString( "Description", description, "" );
   pc.addString( "Infotext", infotext, "" );

   while ( infotext.find ( "#CRT#" ) != ASCString::npos )
      infotext.replace ( infotext.find ( "#CRT#" ), 5, "\n" );
   while ( infotext.find ( "#crt#" ) != ASCString::npos )
      infotext.replace ( infotext.find ( "#crt#" ), 5, "\n" );
   while ( infotext.find ( "\r" ) != ASCString::npos )
      infotext.replace ( infotext.find ( "\r" ), 1, "" );

   pc.addInteger( "ID", id );

   if ( pc.find( "SecondaryIDs") || !pc.isReading())
      pc.addIntegerArray("SecondaryIDs", secondaryIDs );

   pc.addInteger( "View", view );
   if ( view > maxViewRange )
      view = maxViewRange;

   pc.addInteger( "Jamming", jamming, 0 );
   pc.addString( "InfoImage", infoImageFilename, "" );
   pc.addString( "InfoImageSmall", infoImageSmallFilename, "" );

   if ( infoImageFilename.find_first_of("\n\r") != ASCString::npos )
      fatalError ( "Invalid InfoImage filaname in file: " + pc.getFileName() );

   if ( infoImageSmallFilename.find_first_of("\n\r") != ASCString::npos )
      fatalError ( "Invalid InfoImageSmall filaname in file: " + pc.getFileName() );


   pc.openBracket ( "MaxResourceProduction" );
   maxplus.runTextIO ( pc, Resources(0,0,0) );
   pc.closeBracket ();

   pc.openBracket ( "ResourceExtractionEfficiency");
    pc.addInteger( "Material", efficiencymaterial, 1024 );
    pc.addInteger( "Fuel", efficiencyfuel, 1024 );
   pc.closeBracket ();

   pc.openBracket ( "StorageCapacity" );
    pc.openBracket( "BImode" );
     bi_mode_tank.runTextIO ( pc, Resources(0,0,0) );
    pc.closeBracket();
    pc.openBracket ( "ASCmode" );
     asc_mode_tank.runTextIO ( pc, Resources(0,0,0) );
    pc.closeBracket();
   pc.closeBracket ();
   
   pc.addInteger ( "MaxResearch", maxresearchpoints, 0 );
   pc.addInteger ( "NominalResearch", nominalresearchpoints, maxresearchpoints/2 );
   pc.addInteger ( "MaxResearchpointsDefault", defaultMaxResearchpoints, maxresearchpoints );
   
   pc.openBracket( "DefaultProduction" );
    defaultProduction.runTextIO ( pc, Resources(0,0,0) );
   pc.closeBracket();

   pc.addTagInteger( "CategoriesProduceable", vehicleCategoriesProduceable, cmovemalitypenum, unitCategoryTags, -1 );
	 
   pc.openBracket( "AutoHarvestObjects" );
   pc.addIntRangeArray( "objects", autoHarvest.objectsHarvestable, false );
   pc.addIntRangeArray( "objectGroups", autoHarvest.objectGroupsHarvestable, false );
   pc.addInteger( "Range", autoHarvest.range, 0 );
   pc.addInteger( "MaxFieldsPerTurn", autoHarvest.maxFieldsPerTurn, maxint);
   pc.closeBracket();
   
   
   pc.openBracket ( "Construction" );
   pc.addIntRangeArray ( "VehiclesInternally", vehiclesInternallyProduceable, false );
   productionEfficiency.runTextIO("ProductionEfficiency", pc, productionEfficiency );
   pc.closeBracket();
   
   pc.addInteger("minFieldRepairDamage", minFieldRepairDamage , 0 );
}


bool ContainerBaseType :: vehicleFit ( const VehicleType* fzt ) const
{
   if ( maxLoadableUnits > 0 )
      if ( maxLoadableWeight > 0 )
         if ( vehicleCategoriesStorable & (1<<fzt->movemalustyp) )
            if ( maxLoadableUnitSize >= fzt->maxsize() )
               return true;

   return false;
}

int  ContainerBaseType :: vehicleUnloadable ( const VehicleType* vehicleType, int carrierHeight ) const
{
   int height = 0;

   int carrierBinHeight = 1 << carrierHeight;

   if ( vehicleFit ( vehicleType ))
      for ( ContainerBaseType::EntranceSystems::const_iterator i = entranceSystems.begin(); i != entranceSystems.end(); i++ )
         if ( i->mode & ContainerBaseType::TransportationIO::Out )
            if ( (i->container_height & carrierBinHeight) || (i->container_height == 0))
               if ( i->vehicleCategoriesLoadable & (1<<vehicleType->movemalustyp))
                  if ( vehicleType->hasAnyFunction(i->requiresUnitFeature) || i->requiresUnitFeature.none() ) {
                     if ( i->height_abs != 0 && i->height_rel != -100 ) {
                        int h = 0;
                        for ( int hh = 0; hh < 8; ++hh)
                           if ( getheightdelta(carrierHeight, hh) == i->height_rel )
                              h += 1 << hh;
                        height |= i->height_abs & h;
                     } else
                        if ( i->height_rel != -100 )
                           height |= 1 << (carrierHeight + i->height_rel) ;
                        else
                           height |= i->height_abs ;
                  }
   return height & vehicleType->height;
}


const int containerBaseTypeVersion = 8;


void ContainerBaseType :: read ( tnstream& stream )
{
   int version = stream.readInt();
   if ( version > containerBaseTypeVersion || version < 1 ) {
      ASCString s = "invalid version for reading ContainerBaseType: ";
      s += strrr ( version );
      throw ASCmsgException ( s );
   }
   maxLoadableUnits = stream.readInt();
   maxLoadableUnitSize = stream.readInt();
   maxLoadableWeight = stream.readInt();
   vehicleCategoriesStorable = stream.readInt();
   int num = stream.readInt();
   entranceSystems.resize(num);
   for ( int i = 0; i < num; i++ )
      entranceSystems[i].read( stream );

   if ( version >= 2 )
      infoImageFilename = stream.readString(true);

   if ( version >= 3 )
      stream.readBitset( features );

   if ( version >= 4 )
      vehicleCategoriesProduceable = stream.readInt();

   if ( version >= 5 )
      readClassContainer( secondaryIDs, stream );

   if ( version >= 6 ) {
		 autoHarvest.range = stream.readInt();
       readClassContainer( autoHarvest.objectsHarvestable, stream );
       readClassContainer( autoHarvest.objectGroupsHarvestable, stream );
       autoHarvest.maxFieldsPerTurn = stream.readInt();
       
       readClassContainer( vehiclesInternallyProduceable, stream );
       productionEfficiency.read( stream );
	 }
   if ( version >= 7 ) 
      infoImageSmallFilename = stream.readString();
   
   if ( version >= 8 )
      minFieldRepairDamage = stream.readInt();
   else
      minFieldRepairDamage  = 0;
}

void ContainerBaseType :: write ( tnstream& stream ) const
{
   stream.writeInt( containerBaseTypeVersion );
   stream.writeInt( maxLoadableUnits );
   stream.writeInt( maxLoadableUnitSize );
   stream.writeInt( maxLoadableWeight );
   stream.writeInt( vehicleCategoriesStorable );
   stream.writeInt( entranceSystems.size() );
   for ( int i = 0; i < entranceSystems.size(); i++ )
      entranceSystems[i].write( stream );
   stream.writeString( infoImageFilename );
   stream.writeBitset( features );
   stream.writeInt( vehicleCategoriesProduceable );
   writeClassContainer( secondaryIDs, stream );

   stream.writeInt( autoHarvest.range );
   writeClassContainer( autoHarvest.objectsHarvestable, stream );
   writeClassContainer( autoHarvest.objectGroupsHarvestable, stream );
   stream.writeInt( autoHarvest.maxFieldsPerTurn );
   
   writeClassContainer( vehiclesInternallyProduceable, stream );
   productionEfficiency.write( stream );
   stream.writeString( infoImageSmallFilename );
   stream.writeInt( minFieldRepairDamage  );
}

const int containerBaseTypeTransportVersion = 3;


void ContainerBaseType :: TransportationIO :: read ( tnstream& stream )
{
   int version = stream.readInt();
   if ( version > containerBaseTypeTransportVersion || version < 1 ) {
      ASCString s = "invalid version for reading ContainerBaseTypeTransportation: ";
      s += strrr ( version );
      throw ASCmsgException ( s );
   }
   mode = stream.readInt();
   height_abs = stream.readInt();
   height_rel = stream.readInt();
   container_height = stream.readInt();
   vehicleCategoriesLoadable = stream.readInt();
   dockingHeight_abs = stream.readInt();
   dockingHeight_rel = stream.readInt();
   if ( version <= 2 ) {
      int r = stream.readInt();
      requiresUnitFeature = VehicleType::convertOldFunctions(r, stream.getLocation());
   } else
      stream.readBitset( requiresUnitFeature );
      
   disableAttack = stream.readInt();
   if ( version >= 2 )
      movecost = stream.readInt();
   else
      movecost = -1;
   
}

void ContainerBaseType :: TransportationIO :: write ( tnstream& stream ) const
{
   stream.writeInt ( containerBaseTypeTransportVersion );
   stream.writeInt ( mode );
   stream.writeInt ( height_abs );
   stream.writeInt ( height_rel );
   stream.writeInt ( container_height );
   stream.writeInt ( vehicleCategoriesLoadable );
   stream.writeInt ( dockingHeight_abs );
   stream.writeInt ( dockingHeight_rel );
   stream.writeBitset ( requiresUnitFeature );
   stream.writeInt ( disableAttack );
   stream.writeInt ( movecost );
}


Resources ContainerBaseType::getStorageCapacity( int mode ) const
{
   if ( mode == 1 )
      return bi_mode_tank;
   else
      return asc_mode_tank;
}


