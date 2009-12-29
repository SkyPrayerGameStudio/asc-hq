%module asc
%include "std_vector.i"
%include "std_string.i"
%{
#include <string>
#include "../gamemap.h"
#include "../mapalgorithms.h"
#include "../util/messaginghub.h"
#include "../textfiletags.h"
#include "common.h"
%}

class Resources {
   public:
      %immutable;
      
      int energy;
      int material;
      int fuel;
      
      %mutable;

      Resources();
      Resources( int energy, int material, int fuel );
          
};

class MapCoordinate {
   protected:
      MapCoordinate();
   public:       
      MapCoordinate(int x, int y );
      bool valid();
};


class MapCoordinate3D : public MapCoordinate {
   protected:
      MapCoordinate3D();
   public:       
      MapCoordinate3D(int x, int y, int bitmappedHeight );
      bool valid();
};

GameMap* getActiveMap();

const ObjectType* getObjectType( int id );
const BuildingType* getBuildingType( int id );
const VehicleType* getUnitType( int id );
const TerrainType* getTerrainType( int id );

void errorMessage ( std::string s );
void warningMessage ( std::string s );
void infoMessage ( std::string s );


class ContainerBaseType {
   public: 
      int getID() const;
      int getHeight() const;
      
      %extend 
      {
         bool hasProperty ( std::string bitName ) // see textfiletags.cpp for value
         {
            for ( int i = 0; i < ContainerBaseType::functionNum; ++i )
               if ( containerFunctionTags[i] == bitName )
                  return $self->hasFunction( (ContainerBaseType::ContainerFunctions) i);
            return false;
         }
      }
      
   protected:
      ContainerBaseType();
};

class BuildingType : public ContainerBaseType {
};

class VehicleType : public ContainerBaseType {
};

class TerrainType {
   protected:
      TerrainType();
   public:
      int getID() const;
};

class ContainerBase {
   public:         
      void deleteProductionLine( const VehicleType* type );
      void deleteAllProductionLines();
      void addProductionLine( const VehicleType* type  );
      
      int getCargoCount();
      
      // warning: the cargo may have items which are NULL
      Vehicle* getCargo( int i );
      
      std::string getName();
      void setName( const std::string& name );
      
      int getOwner();
      int getHeight();
      
      Resources getStorageCapacity() const;
      
   protected:
      ContainerBase();
};

class Building : public ContainerBase {
   protected:
      Building();
   public:
      const BuildingType* getType();
};

class Vehicle : public ContainerBase {
   protected:       
      Vehicle();
   public:
      const VehicleType* getType();
      Resources getTank() const;
};

class Object {
   public:         
      ObjectType* getType();
      int getDir();
      void setDir( int dir );
};


class ObjectType {
   public:
      int getID();
      std::string getName();
};

class MapField {
   public:
      Building* getBuildingEntrance();
      Vehicle* getVehicle();
      int getWeather();
      
      int getMineralMaterial() const;
      int getMineralFuel() const;
    
      void setMineralMaterial( int material );
      void setMineralFuel( int material );
      
      %extend 
      {
         bool hasProperty ( std::string bitName ) // see textfiletags.cpp for value
         {
            for ( int i = 0; i < terrainPropertyNum; ++i )
               if ( terrainProperties[i] == bitName )
                  return $self->bdt.test(i);
            return false;
         }
      }
#ifdef mapeditor
      void setWeather( int weather );
      bool removeObject ( const ObjectType* obj, bool force = false );
#endif      
     Object* checkForObject ( const ObjectType*  o );
     
     %extend
     {   
        int getObjectCount()
        {
           return $self->objects.size();
        }
        
        Object* getObject( int num ) // zero based!
        {
           if ( num >= 0 && num < $self->objects.size() )
               return &($self->objects[num]);
            else
               return NULL;  
        }
     }
     
     
   protected:
      MapField();
};


class Properties {      
   public:
      std::string getValue( const std::string& key );
      void setValue( const std::string& key, const std::string& value );
};


class GameMap {
   public:
#ifdef mapeditor
      // in ASC, the scripts are not able to access all fields, as they are running in a user context
      MapField* getField( int x, int y );
      MapField* getField( const MapCoordinate& pos );
#endif      
      int width() const;
      int height() const;
      Player& getPlayer( int p );
      
      Properties& getProperties();
};

class Player {
   protected:
      Player();
   public:        
      int getPosition();
      bool exist() const;
      bool isHuman() const;
#ifdef mapeditor
      void setName( const char* name );
#endif      
      Research& getResearch();
};


class Research {
   protected:
      Research();
   public:
#ifdef mapeditor
      void addPredefinedTechAdapter( const std::string& techAdapter );
#endif
};


class StringArray {
   public:
      StringArray();
      void add( const std::string& s );
      std::string getItem( int n ); //!< 1 based, in best LUA tradition 
      int size(); //
};


class PropertyDialog  {
   public:
      PropertyDialog( const std::string& title );
      void addBool( const std::string& name, bool defaultValue );
      void addInteger( const std::string& name, int defaultValue );
      
      // returns and integer, so query result with getInteger
      void addIntDropdown ( const std::string& name, const StringArray& names, int defaultValue );
      
      void addString( const std::string& name, const std::string& defaultValue );
      
      std::string getString( const std::string& name );
      int getInteger( const std::string& name );
      bool getBool( const std::string& name );
      bool run();
};

int selectString ( const std::string& title, const StringArray& entries, int defaultEntry = -1 );

/* this is a very special function for usage in conjunction with setLocalizedEventMessage , to get the map that 
   is being loaded before it is set active. Only for translation scripts */
GameMap* getLoadingMap();

void setLocalizedEventMessage( GameMap* map, int eventID, const std::string& message );
void setLocalizedContainerName( GameMap* map, const MapCoordinate& pos, const std::string& name );

MapCoordinate getCursorPosition( const GameMap* gamemap );

