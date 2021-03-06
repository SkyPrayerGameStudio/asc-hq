//
// C++ Implementation: guidegenerator
//
// Description:
//
//
// Author: Kevin Hirschmann <hirsch@dhcppc0>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "../itemrepository.h"
#include "../buildingtype.h"
#include "../vehicletype.h"
#include "../ascstring.h"
#include "../typen.h"
#include "../basegfx.h"
#include "../loadpcx.h"
#include "../terraintype.h"
#include "../sgstream.h"
#include "../unitset.h"
#include "../graphics/surface2png.h"

#include "guidegenerator.h"
#include "infopage.h"

typedef vector<InfoPage*> InfoPageVector;

const ASCString BuildingGuideGen::APPENDIX ="B";
const ASCString UnitGuideGen::APPENDIX ="U";

const ASCString UnitGuideGen::TROOPER = "Trooper";
const ASCString UnitGuideGen::GROUNDUNIT = "Ground unit";
const ASCString UnitGuideGen::AIRUNIT = "Aircraft";
const ASCString UnitGuideGen::SEAUNIT = "Marine";
const ASCString UnitGuideGen::TURRETUNIT = "Turret";
const ASCString UnitGuideGen::ORBITUNIT = "Orbital";

bool InfoPageUtil::equalFiles(const ASCString src, const ASCString dst) {
  const int maxFileSize = 10000000;
  auto_ptr<char> s ( new char[maxFileSize]);
  auto_ptr<char> d ( new char[maxFileSize]);
  FILE* i = fopen ( src.c_str(), filereadmode );
  FILE* o = fopen ( dst.c_str(), filereadmode );
  bool result = true;
  if ( !i )
    fatalError ("equalFiles :: Could not open input file " + src);
  if ( !o ) {
    fclose ( i );
    result = false;
    return result;
  } else {
    int s1 = fread ( s.get(), 1, maxFileSize, i );
    int s2 = fread ( d.get(), 1, maxFileSize, o );
    if ( s1 != s2 )
      result = false;
    else {
      for ( int i = 0; i < s1; i++ )
        if ( s.get()[i] != d.get()[i] ) {
          result = false;
          break;
        }
    }
  }
  fclose( o );
  fclose( i );
  return result;
}

void InfoPageUtil::copyFile(const ASCString src, const ASCString dst) {
  const int maxFileSize = 10000000;

  auto_ptr<char> s ( new char[maxFileSize]);
  FILE* i = fopen ( src.c_str(), filereadmode );
  FILE* o = fopen ( dst.c_str(), filewritemode);
  if(!i) {
    fclose( o );
    return;
  }
  if ( !o ) {
    fclose( i );
    return;
  }
  int s1 = fread ( s.get(), 1, maxFileSize, i );
  fwrite ( s.get(), 1, s1, o );
  fclose( o );
  fclose( i );
  printf("*");
}

bool InfoPageUtil::diffMove(const ASCString src, const ASCString dst) {
  if(equalFiles(src, dst)) {
    remove(src.c_str());
    return false;
  } else {
    InfoPageUtil::copyFile(src, dst);
    remove(src.c_str());
    return true;
  }
}

ASCString InfoPageUtil::getTmpPath() {
#ifdef _WIN32_
  ASCString tempPath = getenv("temp");
  appendbackslash(tempPath);
  return tempPath;
#else
  return ("/tmp/");
#endif

}

void InfoPageUtil::updateFile(ASCString fileName, ASCString exportPath) {
  ASCString destilledFileName = fileName;
#ifdef _WIN32_
  destilledFileName.erase(0, fileName.find_last_of("\\") + 1);
#else
  destilledFileName.erase(0, fileName.find_last_of("/") + 1);
#endif
  ASCString exportTarget = exportPath + destilledFileName ;
  ASCString tmpTarget = InfoPageUtil::getTmpPath() + destilledFileName;
  InfoPageUtil::diffMove(tmpTarget, exportTarget);
}
//*************************************************************************************************************
ImageConverter::ImageConverter() {}


ASCString ImageConverter::createPic(const BuildingType&  bt, ASCString filePath) {
  Surface s = Surface::createSurface( 600, 600, 32, 0xf8f4f0 );
  int x=0; int y=0;
  bt.paint( s, SPoint(x,y));
  
  int xsize = 300;
  int ysize = 200;
  convert(constructImgFileName(bt), s, filePath, xsize, ysize);
  return  (constructImgPath(bt, RELATIVEIMGPATH)) ;
}

ASCString ImageConverter::createPic(const VehicleType&  vt, ASCString filePath) {
   Surface s = Surface::createSurface(100, 100, 32, 0xf8f4f0);
  vt.paint( s, SPoint(0,0));
   
  convert(constructImgFileName(vt), s, filePath );
  return  (constructImgPath(vt, RELATIVEIMGPATH)) ;
}


void ImageConverter::convert(const ASCString&  fileName, Surface& s, ASCString filePath, int xsize, int ysize) {
  cout << "creating image..." << fileName << endl;
  writePNGtrim ( filePath + fileName, s );
}




ASCString ImageConverter::constructImgPath(const BuildingType&  bt, const ASCString filePath) {
  return (filePath + constructImgFileName(bt));
}

ASCString ImageConverter::constructImgPath(const VehicleType&  vt, const ASCString filePath) {
  return (filePath + constructImgFileName(vt));
}

ASCString ImageConverter::constructImgFileName(const BuildingType&  bt) {
  return (strrr(bt.id) + ASCString("B.png"));
}

ASCString ImageConverter::constructImgFileName(const VehicleType&  vt) {
  return (strrr(vt.id) + ASCString("U.png"));
}


//**************************************************************************************************************


GuideGenerator::GuideGenerator(ASCString fp, ASCString menuCss, int id, ASCString maCSSFile, ASCString techIDs, bool imgCreate, ASCString relativeMenuPath, bool upload, int imageSize):filePath(fp), menuCSSFile(menuCss), mainCSSFile(maCSSFile), createImg(imgCreate), setID(id), imageWidth(imageSize), createUpload(upload), techTreeIDs(String2IntRangeVector(techIDs)), relMenuPath(relativeMenuPath)
{
   if ( relMenuPath.length() > 0  ){
     if(relMenuPath.find_last_of("/")!=relMenuPath.size()-1){
        relMenuPath += "/";
	//appendbackslash( relMenuPath );
     }
   }
}

const ASCString& GuideGenerator::getImagePath(int id) {
  return graphicRefs[id];
}

const ASCString& GuideGenerator::getMainCSSPath() const {
  return mainCSSFile;
}
//******************************************************************************************************

BuildingGuideGen::BuildingGuideGen(ASCString fp, ASCString css, int id, ASCString maCSSFile, ASCString techIDs, bool imgCreate,  ASCString relativeMenuPath, bool bUnique, bool upload, int imageSize): GuideGenerator(fp, css, id, maCSSFile, techIDs,
    imgCreate, relativeMenuPath, upload), buildingsUnique(bUnique) {}



ASCString BuildingGuideGen::constructFileName(const BuildingType& bType) {
  return ASCString(strrr(bType.id)) + APPENDIX;

}

void BuildingGuideGen::processBuilding(const BuildingType&  bt) {
  cout << "Processing building:"  << bt.id <<", " << bt.name << endl;
  int buildingID = bt.id;
  ImageConverter ic;
  ASCString imgPath;
  ASCString target;
  if(createUpload) {
    target = InfoPageUtil::getTmpPath();
  } else {
    target = filePath;
  }
  if(createImg) {
    imgPath = ic.createPic(bt, target);
    if(createUpload) {
      InfoPageUtil::updateFile(ic.constructImgPath(bt, filePath), filePath);
    }
  } else {
    imgPath = ic.constructImgPath(bt, filePath);
  }
  graphicRefs.insert(Int2String::value_type(buildingID, imgPath));
  InfoPageVector ipv;
  BuildingMainPage mp(bt, target, this);
  ipv.push_back(&mp);
  BuildingTerrainPage tp(bt, target, this);
  ipv.push_back(&tp);
  BuildingCargoPage ap(bt, target,  this);
  ipv.push_back(&ap);
  BuildingResourcePage rp(bt, target, this);
  ipv.push_back(&rp);
  BuildingResearchPage rea(bt, target, this);
  ipv.push_back(&rea);
  for(int i = 0; i < ipv.size(); i++) {
    ipv[i]->buildPage();
    if(createUpload) {      
      InfoPageUtil::updateFile(ipv[i]->getPageFileName(), filePath);
    }
  }
}



void BuildingGuideGen::generateCategories() const {
  try {
    for ( std::vector<SingleUnitSet*>::iterator i = unitSets.begin(); i != unitSets.end(); i++  ) {
      SingleUnitSet* s = (*i);
      if((s->ID == setID)||(setID<=0)) {        
        ASCString fileName =  InfoPageUtil::getTmpPath() + "buildingset_id(" + strrr(s->ID) +")" + GROUPS;
        Category set(s->name, menuCSSFile);
	
        Category* hqCat = new Category(HQ, menuCSSFile);
        Category* facCat = new Category(FACTORY, menuCSSFile);
        Category* researchCat = new Category(RESEARCHFAC, menuCSSFile);
        Category* windpowCat = new Category(WINDPOWERPLANT, menuCSSFile);
        Category* solarpowCat = new Category(SOLARPOWERPLANT, menuCSSFile);
        Category* mattconCat = new Category(MATTERCONVERTER, menuCSSFile);
        Category* trainCat = new Category(TRAININGCENTER, menuCSSFile);
        Category* noCat = new Category(NOCATEGORY, menuCSSFile);
        Category* mineCat = new Category(MININGSTATION, menuCSSFile);
        Category* idCat = new Category(SORTID, menuCSSFile);

        set.addEntry(hqCat);
        set.addEntry(facCat);
        set.addEntry(researchCat);
        set.addEntry(windpowCat);
        set.addEntry(solarpowCat);
        set.addEntry(mattconCat);
        set.addEntry(trainCat);
        set.addEntry(noCat);
        set.addEntry(mineCat);

        for ( int building = 0; building < buildingTypeRepository.getNum(); building++ ) {
          BuildingType*  bt = buildingTypeRepository.getObject_byPos ( building );
          if(s->isMember(bt->id, SingleUnitSet::building)&&(processedBuildingIds.find(bt->id)!=processedBuildingIds.end())) {
            ASCString fileLink = relMenuPath + strrr(bt->id) + APPENDIX + ASCString(MAINLINKSUFFIX) + HTML;
            CategoryMember* dataEntry = new CategoryMember(bt->name.toUpper(), menuCSSFile, fileLink, TARGET);
            if ( bt->hasFunction( ContainerBaseType::InternalVehicleProduction  )) {
              facCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::Research ) ) {
              researchCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::WindPowerPlant  )) {
              windpowCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::SolarPowerPlant  )) {
               solarpowCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::MatterConverter  )) {
               mattconCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::MiningStation  )) {
               mineCat->addEntry(dataEntry);
            } else if ( bt->hasFunction( ContainerBaseType::TrainingCenter  )) {
               trainCat->addEntry(dataEntry);
            } else {
              noCat->addEntry(dataEntry);
            }
            CategoryMember* idEntry = new CategoryMember(ASCString(strrr(bt->id)) + "(" +  bt->name +")", menuCSSFile, fileLink, TARGET);
            idCat->addEntry(idEntry);
          }
        }
        set.sort();
        idCat->sort();
        set.addEntry(idCat);
        GroupFile gf (fileName.c_str(), set);
        gf.write();
        InfoPageUtil::updateFile(fileName, filePath);
      }
    }
  } catch(ASCmsgException& e) {
    cout << e.getMessage() << endl;
  }
}

BuildingGuideGen::~BuildingGuideGen() {}


void BuildingGuideGen::processSubjects() {
  cout << "Selected set-ID is " << setID << endl;
  for ( int building = 0; building < buildingTypeRepository.getNum(); building++ ) {
    BuildingType*  bt = buildingTypeRepository.getObject_byPos ( building );
    if(setID > 0) {
      for ( std::vector<SingleUnitSet*>::iterator i = unitSets.begin(); i != unitSets.end(); i++  ) {
        SingleUnitSet* s = (*i);
        if((s->ID == setID) && (s->isMember(bt->id,SingleUnitSet::building))) {	  
          if(buildingNames.end() == buildingNames.find(strrr(s->ID) +bt->name)||(!buildingsUnique)) {
            processBuilding(*bt);
            buildingNames.insert(strrr(s->ID) +bt->name);	    
	    processedBuildingIds.insert(bt->id);
          }
        }
      }
    } else {
      processBuilding(*bt);
      processedBuildingIds.insert(bt->id);
    }
    
  }
  generateCategories();
}
//UnitGuideGen****************************************************************************************************
UnitGuideGen::UnitGuideGen(ASCString fp, ASCString meCSSFile, int id, ASCString maCSSFile, ASCString techIDs, bool imgCreate,  ASCString relativeMenuPath, bool upload, int imageSize): GuideGenerator(fp, meCSSFile, id, maCSSFile, techIDs, imgCreate, relativeMenuPath, upload, imageSize) {}




UnitGuideGen::~UnitGuideGen() {}

ASCString UnitGuideGen::constructFileName(const VehicleType& vt) {
  return ASCString(strrr(vt.id)) + APPENDIX;

}

void UnitGuideGen::processSubjects() {
  cout << "Selected set-ID is " << setID << endl;
  for(int unit = 0; unit < vehicleTypeRepository.getNum(); unit++) {
    VehicleType* vt = vehicleTypeRepository.getObject_byPos(unit);
    if(setID > 0) {
      for ( std::vector<SingleUnitSet*>::iterator i = unitSets.begin(); i != unitSets.end(); i++  ) {
        SingleUnitSet* s = (*i);
        if((s->ID == setID) && (s->isMember(vt->id,SingleUnitSet::unit))) {
          processUnit(*vt);
        }
      }
    } else {
      processUnit(*vt);
    }
  }
  generateCategories();
}

void UnitGuideGen::processUnit(const VehicleType& vt) {
  cout << "Processing unit:"  << vt.id <<", " << vt.name << endl;
  int unitID = vt.id;
  ImageConverter ic;
  ASCString imgPath;
  ASCString target;
  if(createUpload) {
    target = InfoPageUtil::getTmpPath();
  } else {
    target = filePath;
  }
  if(createImg) {
    imgPath = ic.createPic(vt, target);
    if(createUpload) {
      InfoPageUtil::updateFile(ic.constructImgPath(vt, filePath), filePath);
    }
  } else {
    imgPath = ic.constructImgPath(vt, filePath);
  }
  graphicRefs.insert(Int2String::value_type(unitID, imgPath));
  InfoPageVector ipv;
  UnitMainPage mp(vt, target, this);
  ipv.push_back(&mp);
  UnitTerrainPage tp(vt, target, this);
  ipv.push_back(&tp);
  UnitCargoPage ap(vt, target,  this);
  ipv.push_back(&ap);
  UnitWeaponPage wp(vt, target, this);
  ipv.push_back(&wp);
  UnitConstructionPage cp(vt, target, this);
  ipv.push_back(&cp);
  UnitResearchPage rp(vt, target, this);
  ipv.push_back(&rp);
  for(int i = 0; i < ipv.size(); i++) {
    ipv[i]->buildPage();
    if(createUpload) {      
    InfoPageUtil::updateFile(ipv[i]->getPageFileName(), filePath);
    }
  }
}

void UnitGuideGen::generateCategories() const {
  try {
    for ( std::vector<SingleUnitSet*>::iterator i = unitSets.begin(); i != unitSets.end(); i++  ) {
      SingleUnitSet* s = (*i);      
        if((s->ID == setID)||(setID<=0)) {
          ASCString fileName =  InfoPageUtil::getTmpPath() + "unitset_id(" + strrr(s->ID) +")" + GROUPS;
          Category set(s->name, menuCSSFile);

          Category* trooperCat = new Category(TROOPER, menuCSSFile);
          Category* groundCat = new Category(GROUNDUNIT, menuCSSFile);
          Category* airCat = new Category(AIRUNIT, menuCSSFile);
          Category* orbitCat = new Category(ORBITUNIT, menuCSSFile);
          Category* seaCat = new Category(SEAUNIT, menuCSSFile);
          Category* turretCat = new Category(TURRETUNIT, menuCSSFile);
          Category* miscCat = new Category(NOCATEGORY, menuCSSFile);
          Category* idCat = new Category(SORTID, menuCSSFile);



          set.addEntry(trooperCat);
          set.addEntry(groundCat);
          set.addEntry(airCat);
          set.addEntry(orbitCat);
          set.addEntry(seaCat);
          set.addEntry(turretCat);
          set.addEntry(miscCat);

          for ( int unit = 0; unit < vehicleTypeRepository.getNum(); unit++ ) {
            VehicleType*  vt = vehicleTypeRepository.getObject_byPos ( unit );
            if(s->isMember(vt->id,SingleUnitSet::unit)) {
              ASCString fileLink = relMenuPath + strrr(vt->id) + APPENDIX + ASCString(MAINLINKSUFFIX) + HTML;

              CategoryMember* dataEntry = new CategoryMember(vt->name.toUpper(), menuCSSFile, fileLink, TARGET);
              switch ( vt->movemalustyp ) {
              case 7:  //   "trooper"
                trooperCat->addEntry(dataEntry);
                break;
              case 1:  // "light tracked vehicle"
              case 2:  // "medium tracked vehicle"
              case 3:  // "heavy tracked vehicle",
              case 4:  // "light wheeled vehicle",
              case 5:  //  "medium wheeled vehicle",
              case 6:  //  "heavy wheeled vehicle",
              case 8:   // "rail vehicle",
                groundCat->addEntry(dataEntry);
                break;
              case 9:   // "medium aircraft",
              case 12:  // "light aircraft",
              case 13:  // "heavy aircraft",
              case 16:  //  "helicopter",
                if ( vt->height & chsatellit )
                    orbitCat->addEntry(dataEntry);
                   else
                    airCat->addEntry(dataEntry);
                break;
              case 10:  // "medium ship",
              case 14:  // "light ship",
              case 15:  // "heavy ship",
                seaCat->addEntry(dataEntry);
                break;

              case 11:  // "building / turret / object",
                turretCat->addEntry(dataEntry);
                break;

              case 0:  // default",
              case 17: // "hoovercraft"
              default:
                miscCat->addEntry(dataEntry);
                break;
              };
              CategoryMember* idEntry = new CategoryMember(ASCString(strrr(vt->id)) + "(" + vt->name + ")", menuCSSFile, fileLink, TARGET);
              idCat->addEntry(idEntry);
            }
          }
          set.sort();
          idCat->sort();
          set.addEntry(idCat);
          GroupFile gf (fileName.c_str(), set);
          gf.write();
          InfoPageUtil::updateFile(fileName, filePath);
        }
      }    
  } catch(ASCmsgException& e) {
    cout << e.getMessage() << endl;
  }
}





