//
// C++ Interface: weatherdialog
//
// Description:
//
//
// Author: Martin Bickel <bickel@asc-hq.org>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WEATHERDIALOG_H
#define WEATHERDIALOG_H

#include <sstream>
#include "global.h"
#include "paradialog.h"
#include "weatherdialog.h"
#include "events.h"
#include "gameoptions.h"
#include "sg.h"
#include "sdl/sound.h"

/**
@author Kevin Hirschmann
*/
class ChanceSettingsDialog :  public ASC_PG_Dialog {
public:  
  ChanceSettingsDialog(std::string title);
  
  virtual ~ChanceSettingsDialog();
  
  virtual bool closeWindow() {
    quitModalLoop(2);
    return true;
  };
  
protected:
  virtual void buildUpForm(const vector<string>& labelVec);  
private:  
  virtual bool buttonEvent( PG_Button* button ) = 0;
  virtual int getNthChanceValue(int n) = 0;
  virtual void setChanceValues(const vector<PG_LineEdit*>& p)= 0;

  protected:  
  vector<PG_LineEdit*> chances;
  vector<PG_Label*> labels;
  PG_Label* note;
  static const int xsize;
  static const int ysize;
};

class FallOutSettingsDialog: public ChanceSettingsDialog  {
virtual int getNthChanceValue(int n);
virtual void setChanceValues(const vector<PG_LineEdit*>& p);
virtual bool buttonEvent( PG_Button* button );


public:
  FallOutSettingsDialog();
  ~FallOutSettingsDialog();
};

class WindSpeedSettingsDialog: public ChanceSettingsDialog  {
virtual int getNthChanceValue(int n);
virtual void setChanceValues(const vector<PG_LineEdit*>& p);
virtual bool buttonEvent( PG_Button* button );


public:
  WindSpeedSettingsDialog();
  ~WindSpeedSettingsDialog();
};

class WindDirectionSettingsDialog: public ChanceSettingsDialog  {
virtual int getNthChanceValue(int n);
virtual void setChanceValues(const vector<PG_LineEdit*>& p);
virtual bool buttonEvent( PG_Button* button );


public:
  WindDirectionSettingsDialog();
  ~WindDirectionSettingsDialog();
};

class WeatherAreaInformation{
private:
  WeatherArea* weatherArea;
  int turn;
  int duration;
  FalloutType fallOut;
public:
  WeatherAreaInformation(WeatherArea* wa, int turn, int duration, FalloutType fallOut);
  WeatherAreaInformation(WeatherArea* wa, int turn);
  ~WeatherAreaInformation();
  
  int getTurn() const;
  int getDuration() const;
  FalloutType getFalloutType() const;
  WeatherArea* getWeatherArea(){
    return weatherArea;
  }
  std::string getInformation() const;

};
class EventAreasDialog: public ASC_PG_Dialog{

public:
EventAreasDialog();
~EventAreasDialog();
void addNewWeatherAreaInformation(WeatherAreaInformation* wai);

bool closeWindow();

private:
  static const int xSize;
  static const int ySize;
  static const string SEPERATOR;
  list<WeatherAreaInformation*> currentList;
  list<WeatherAreaInformation*> removeList;
  list<WeatherAreaInformation*> addList;
  
  PG_ListBox* eventList;
  PG_Button* addButton;
  PG_Button* removeButton;
  bool buttonEvent( PG_Button* button );
  
  bool buttonAdd( PG_Button* button );
  bool buttonRemove( PG_Button* button );
  
  void updateAreaList();
};

class AddWeatherAreaDialog: public ASC_PG_Dialog{
public:
  AddWeatherAreaDialog(EventAreasDialog* ead);
  ~AddWeatherAreaDialog();
  
  
private:
  static const int xSize;
  static const int ySize;

  PG_Label* turnLabel;
  PG_LineEdit* turnValue;
  
  PG_Label* durationLabel;
  PG_LineEdit* durationValue;
  
  PG_Label* xCoordLabel;
  PG_LineEdit* xCoordValue;
  
  PG_Label* yCoordLabel;
  PG_LineEdit* yCoordValue;
  
  PG_Label* widthLabel;
  PG_LineEdit* widthValue;
  
  PG_Label* heightLabel;
  PG_LineEdit* heightValue;
  
  PG_Label* wTypesLabel;
  PG_DropDown* weatherTypes;
  
  bool closeWindow();  
  bool buttonEvent( PG_Button* button );
};




/**
@author Kevin Hirschmann
*/
class WeatherDialog :  public ASC_PG_Dialog {
public:
  WeatherDialog();
  

  virtual ~WeatherDialog();

  
  
   static const char* int2String(int i) {
    stringstream s;
    s << i;
    return s.str().c_str();
  }
  
   static const char* float2String(float f) {
    stringstream s;
    s << f;
    return s.str().c_str();
  }
  
private:
  static const int xsize;
  static const int ysize;
  
  PG_CheckButton* randomMode;
  
  PG_Label* areaSpawnsLabel;
  PG_LineEdit* areaSpawnsValue;
  
  PG_Label* nthTurnLabel;
  PG_LineEdit* nthTurnValue;
  
  PG_Label* windSpeedFieldRatioLabel;
  PG_LineEdit* windSpeedFieldRatioValue;
  
  PG_Label* upperSizeLimitsLabel;
  PG_LineEdit* upperSizeLimitsValue;
  
  PG_Label* lowerSizeLimitsLabel;
  PG_LineEdit* lowerSizeLimitsValue;
  
  PG_Label* fallOutLabel;
  PG_Button* fallOutButton;
  
  PG_Label* windSpeedLabel;
  PG_Button* windSpeedButton;
  
  PG_Label* windDirectionLabel;
  PG_Button* windDirectionButton;
  
  PG_Label* eventAreasLabel;
  PG_Button* eventAreasButton;
  
  bool editEventAreas(PG_Button* button );  
  bool editFallOut( PG_Button* button );  
  bool editWindSpeed( PG_Button* button );  
  bool editWindDirection( PG_Button* button );
  
  bool buttonEvent( PG_Button* button );
  
  bool closeWindow() {
    quitModalLoop(2);
    return true;
  };
};

extern void weatherConfigurationDialog();

#endif
