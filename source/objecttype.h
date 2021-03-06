/***************************************************************************
                          objecttype.h  -  description
                             -------------------
    begin                : Fri Jul 27 2001
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

#ifndef objecttypeH
 #define objecttypeH

 #include "typen.h"
 #include "terraintype.h"
 #include "research.h"
 #include "overviewmapimage.h"
 #include "mapitemtype.h"
 #include "mapfield.h"


 //! An object that can be placed on fields. Roads, pipelines and ditches are examples of objects. \sa Object
 class ObjectType : public MapItemType, public LoadableItemType {
     bool rotateImage;
     void realDisplay ( Surface& surface, const SPoint& pos, int dir, int weather ) const;
   public:
     //! the id of the object, used when referencing objects in files
     int id;

     //! when loading a file and these IDs are encountered, this object will be used.
     vector<int> secondaryIDs;

     //! the group ID allows units to specify whole groups of objects to be buildable / removable without specifying each individual object.
     int groupID;

     //! bitmapped variable containing the different weather types the objects exist for
     BitSet weather;

     //! is the object displayed under fog of war
     bool visibleago;

     //! some objects are graphically linked with others on neighbouring fields. \sa no_autonet linkableTerrain
     vector<IntRange> linkableObjects;

     //! objects can be graphically linked to terrain, as if the terrain was an object of the same type. \sa no_autonet linkableObjects
     vector<IntRange> linkableTerrain;

     //! if an object should not be attackable, set armor to 0
     int armor;

     struct FieldModification {
        FieldModification();

        //! the movemalus_plus is added to the current movemalus of the field to form the new movemalus. Negative values are ok.
        TerrainType::MoveMalus movemalus_plus;

        //! The movemalus_abs replaces the current movemalus of the field by a new one. Values of 0 and -1 won't affect the movemalus of the field, and values ranging from 1 to 9 must not be used.
        TerrainType::MoveMalus movemalus_abs;

        //! The terrain on which this object can be placed
        TerrainAccess terrainaccess;

        //! the terrain properties of the field will be AND-masked with this field and then OR-masked with terrain_or to form the new terrain properties
        TerrainBits terrain_and;
        TerrainBits terrain_or;
        void runTextIO ( PropertyContainer& pc );
     } fieldModification[cwettertypennum];

     const FieldModification& getFieldModification( int weather ) const;

     //! this is added to the current attackbonus of the field to form the new attackbonus.
     int attackbonus_plus;
     //! The attackbonus_abs replaces the current attackbonus of the field by a new one. A value of -1 won't affect the attackbonus of the field
     int attackbonus_abs;

     //! this is added to the current defensebonus of the field to form the new defensebonus.
     int defensebonus_plus;
     //! The defensebonus_abs replaces the current defensebonus of the field by a new one. A value of -1 won't affect the attackbonus of the field
     int defensebonus_abs;

     //! this is added to the current basicjamming of the field to form the new jamming.
     int basicjamming_plus;
     //! basicjamming_abs replaces the current basicjamming of the field by a new one. A value < 0 won't affect the jamming of the field
     int basicjamming_abs;

     //! units standing on this object will get a bonus to their view
     int viewbonus_plus;
     //! units standing on this object will get a bonus to their view
     int viewbonus_abs;



     //! the level of height the object is on. This is not the simple system of 8 levels used for units and building, but one with 255 levels to allow a fine grained specification of the order in which different objects are painted
     int imageHeight;

     //! the 'real' height of the object, which evaluated for construction and attack. The destinction between imageHeight and physicalHeight was made because of compatibility reasons
     int physicalHeight;

     //! The resources required to construct the object with a unit; Note that units usually don't have any energy available
     Resources buildcost;

     //! The resources required to remove the object with a unit; Note that units usually don't have any energy available
     Resources removecost;

     //! The movement points that are needed to build this object
     int build_movecost;

     //! The movement points that are needed to remove this object
     int remove_movecost;

     //! if a building is constructed on the field containing the object, will the object be removed or will it stay beneath the building.
     bool canExistBeneathBuildings;

     //! The name of the object
     ASCString name;
     
     ASCString getName() const { return name; };

     enum NamingMethod { ReplaceTerrain, AddToTerrain, UnNamed };
     int  namingMethod;
     static const char* namingMethodNames[];
     static const int namingMethodNum;
     
     int getID() const { return id; };    
     

     static const int netBehaviourNum = 7;
     enum NetBehaviour { NetToBuildings = 1, NetToBuildingEntry = 2, NetToSelf = 4, NetToBorder = 8, SpecialForest = 0x10, AutoBorder = 0x20, KeepOrientation = 0x40 };

     //! specifies how the object is going to connect to other things
     int netBehaviour;

     ObjectType ( void );

     TechAdapterDependency techDependency;

     const OverviewMapImage* getOverviewMapImage( int picnum, int weather ) const ; 

     //! the images of the objects
     struct WeatherPicture {
        mutable vector<OverviewMapImage> overviewMapImage;
        vector<Surface> images;
        vector<int>   bi3pic;
        vector<int>   flip;
        void resize(int i) { flip.resize(i); bi3pic.resize(i); images.resize(i); };
        ASCString originalFilename;
     } weatherPicture [cwettertypennum];

     //! displays the objecttype at x/y on the screen
     void display ( Surface& surface, const SPoint& pos ) const ;
     void display ( Surface& surface, const SPoint& pos, int dir, int weather = 0 ) const;
     
     //! returns the pointer to the image i
     const Surface& getPicture ( int i = 0, int weather = 0 ) const;

     //! can the object be build on the field fld
     bool buildable ( MapField* fld ) const;

     //! reads the objecttype from a stream
     void read ( tnstream& stream );
     //! write the objecttype from a stream
     void write ( tnstream& stream ) const;

     //! reads or writes the objecttype to the text stream pc
     void runTextIO ( PropertyContainer& pc );

     //! some objects require special displaying methods, for example shading the terrain they are build on
     int displayMethod;

     //! returns the level of height of this object in the normal 8 level scheme of asc (deep submerged, submerged, ... )
     int getEffectiveHeight() const;

     //! the probability that an object of this type spawns another object on a neighbouring field
     double growthRate;

     //! is the object growth stopped by units and buildings
     bool growOnUnits;

     //! the object is removed when it is lifetime turns old. -1 disables removal
     int lifetime;

     /** the time after which any growth stops. This parameter is passed to all child-objects spawned through growth and should 
         therefor be bigger than lifetime. Use -1 for unlimited */
     int growthDuration;

     
     int getMemoryFootprint() const;
   protected:
     int getWeather( int weather ) const;    
     
   private:
     //! the loading functions call this method to setup the objects images
     void setupImages();
 };

 
 
const int objectDisplayingMethodNum = 5;

namespace ForestCalculation {
  //! automatically adjusting the pictures of woods and coasts to form coherent structures
  extern void smooth ( int what, GameMap* gamemap, ObjectType* woodObj );
};


#endif
