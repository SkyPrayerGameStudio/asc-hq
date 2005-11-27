/*
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-2005  Martin Bickel  and  Marc Schellenberger

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

#include <stdio.h>
#include <cstring>
#include <cmath>

#include "global.h"
#include "ascstring.h"
#include "stringtokenizer.h"
#include "misc.h"
#include "typen.h"
#include "graphicset.h"
#include "basegfx.h" 

#include "vehicletype.h"
#include "buildingtype.h"
#include "textfileparser.h"
#include "textfile_evaluation.h"

//! The different levels of height
const char*  choehenstufen[choehenstufennum] = {"deep submerged", "submerged", "floating", "ground level", "low-level flight", "flight", "high-level flight", "orbit"};



const char*  cmovemalitypes[cmovemalitypenum] = { "default",
                                                 "light tracked vehicle", "medium tracked vehicle", "heavy tracked vehicle",
                                                 "light wheeled vehicle", "medium wheeled vehicle", "heavy wheeled vehicle",
                                                 "trooper",               "rail vehicle",           "medium aircraft",
                                                 "medium ship",           "building / turret / object", "light aircraft",
                                                 "heavy aircraft",        "light ship",             "heavy ship",  "helicopter",
                                                 "hoovercraft"  };

const char* cnetcontrol[cnetcontrolnum] = { "store energy",           "store material",           "store fuel",
                                            "move out all energy",           "move out all material",           "move out all fuel",
                                            "stop storing energy", "stop storing material", "stop storing fuel",
                                            "stop energy extraction", "stop material extraction", "stop fuelextraction" };
const char* cgeneralnetcontrol[4] = {       "store",  "move out", "stop storing", "stop using" };
                                          // Functionen in Gebuden ....

const char*  cwettertypen[cwettertypennum] = {"dry (standard)","light rain", "heavy rain", "few snow", "lot of snow", "lot of snow + ice"};
const char*  resourceNames[3]  = {"energy", "material", "fuel"};

//! when repairing a unit, the experience of the unit is decreased by one when passing each of these damage levels
const int experienceDecreaseDamageBoundaries[experienceDecreaseDamageBoundaryNum] = { 80, 60, 40, 20 };

const int directionangle [ sidenum ] = 
 { 0, -53, -127, -180, -180 -53 , -180 -127 };




const int csolarkraftwerkleistung[cwettertypennum] = { 1024, 512, 256, 756, 384, 384 }; // 1024 ist Maximum





ResourceMatrix :: ResourceMatrix ( const float* f )
{
   int num = 0;
   for ( int i = 0; i < resourceTypeNum; i++ )
      for ( int j = 0; j < resourceTypeNum; j++ )
         e[i][j] = f[num++];
}


Resources ResourceMatrix :: operator* ( const Resources& r ) const
{
   Resources res;
   for ( int i = 0; i < resourceTypeNum; i++ )
      for ( int j = 0; j < resourceTypeNum; j++ )
         res.resource(i) += int( e[i][j] * r.resource(j));

   return res;
}


const char* Resources::name( int r )
{
   return resourceNames[r];
}

void Resources :: read ( tnstream& stream )
{
   for ( int i = 0; i< resourceTypeNum; i++ )
      resource(i) = stream.readInt();
}

void Resources :: write ( tnstream& stream ) const
{
   for ( int i = 0; i< resourceTypeNum; i++ )
      stream.writeInt( resource(i) );
}


Resources operator- ( const Resources& res1, const Resources& res2 )
{
   Resources res = res1;
   res -= res2;
   return res;
}

Resources operator+ ( const Resources& res1, const Resources& res2 )
{
   Resources res = res1;
   res += res2;
   return res;
}

Resources operator* ( const Resources& res1, float a )
{
   Resources res = res1;
   for ( int r = 0; r < resourceTypeNum; r++ )
      res.resource(r) = int( res.resource(r) * a );
   return res;
}

Resources operator/ ( const Resources& res1, float a )
{
   Resources res = res1;
   for ( int r = 0; r < resourceTypeNum; r++ )
      res.resource(r) = int( res.resource(r) / a );
   return res;
}

/*
Resources Resources::operator* ( double d )
{
   Resources rs;
   for ( int i = 0; i < resourceTypeNum; i++ )
      rs.resource(i) = int( resource(i)*d );
   return rs;
}
*/

void Resources::runTextIO ( PropertyContainer& pc )
{
   pc.addInteger  ( "Energy", energy );
   pc.addInteger  ( "Material", material );
   pc.addInteger  ( "fuel", fuel );
}

void Resources::runTextIO ( PropertyContainer& pc, const Resources& defaultValue )
{
   pc.addInteger  ( "Energy", energy, defaultValue.energy );
   pc.addInteger  ( "Material", material, defaultValue.material );
   pc.addInteger  ( "fuel", fuel, defaultValue.fuel );
}

ASCString Resources::toString()
{
   ASCString s;
   int cnt = 0;
   for ( int r = 0; r < 3; r++ )
      if ( resource(r) )
         cnt++;

   int ps = 0;
   for ( int r = 0; r < 3; r++ )
      if ( resource(r) ) {
         ps++;
         ASCString txt3;
         txt3.format( "%d %s", resource(r), resourceNames[r] );
         if ( ps>1 && ps < cnt )
            s += ", ";
         if ( ps>1 && ps == cnt )
            s += " and ";
         s += txt3;
      }
   return s;
}



vector<IntRange> String2IntRangeVector( const ASCString& t )
{
   vector<IntRange> irv;

   StringSplit st ( t, ";, " );
   ASCString s = st.getNextToken();
   while ( !s.empty() ) {
      // we have to take care about negative numbers , which have the - at index 0
      if ( s.find ( "-",1 ) != ASCString::npos ) {
         ASCString from = s.substr ( 0, s.find ( "-", 1 ) );
         ASCString to = s.substr ( s.find ( "-",1 )+1 );
         irv.push_back ( IntRange ( atoi ( from.c_str() ), atoi ( to.c_str() )));
      } else {
         irv.push_back ( IntRange ( atoi ( s.c_str() ), atoi ( s.c_str() )));
      }
      s = st.getNextToken();
   }
   return irv;
}


void IntRange::read ( tnstream& stream )
{
   stream.readInt();
   from = stream.readInt();
   to = stream.readInt();
}

void IntRange::write ( tnstream& stream ) const
{
   stream.writeInt(1);
   stream.writeInt( from );
   stream.writeInt( to );
}

////////////////////////////////////////////////////////////////////


void MapCoordinate::move(const Vector2D& v){
  x +=v.getXComponent();
  y +=v.getYComponent();
}

void MapCoordinate::move(int width, int height) {
  x +=width;
  y +=height;
}

//***************************************************************************************************************************************
Vector2D::Vector2D():xComponent(0), yComponent(0) {}

Vector2D::Vector2D(int x, int y):xComponent(x), yComponent(y) {}

Vector2D::~Vector2D() {}

int Vector2D::getXComponent() const {
  return xComponent;
}

int Vector2D::getYComponent() const {
  return yComponent;
}
double Vector2D::getLength() const{
  return std::sqrt(std::pow(double(xComponent), 2) + std::pow(double(yComponent),2));
}

bool Vector2D::isZeroVector() const {
  return ((xComponent == 0) && (yComponent == 0));
}
