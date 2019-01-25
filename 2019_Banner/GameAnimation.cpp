//=======================================================================================
//
//	Game Animation object
//
//=======================================================================================

#include "GameAnimation.h"

GameAnimation::GameAnimation(Adafruit_NeoMatrix* pMatrix)
{
  _pMatrix = pMatrix;
}


void GameAnimation::GetAnimationInfo()
{
  if (frame >= frameDone)
  {
    animationSubFrame = frame - frameDone;
    animationStep = Done;
  }
  else if (frame >= frameClimbRope)
  {
    animationSubFrame = frame - frameClimbRope;
    animationStep = ClimbRope;
  }
  else if (frame >= frameMoveToRope)
  {
    animationSubFrame = frame - frameMoveToRope;
    animationStep = MoveToRope;
  }
  else if (frame >= framePlaceGear)
  {
    animationSubFrame = frame - framePlaceGear;
    animationStep = PlaceGear;
  }
  else if (frame >= frameMoveToPeg)
  {
    animationSubFrame = frame - frameMoveToPeg;
    animationStep = MoveToPeg;
  }
  else if (frame >= framePickUpGear)
  {
    animationSubFrame = frame - framePickUpGear;
    animationStep = PickUpGear;
  }
  else if (frame >= frameMoveToGear)
  {
    animationSubFrame = frame -  frameMoveToGear;
    animationStep = MoveToGear;
  }
  else
  {
    animationSubFrame = 9999;
    animationStep = Idle;
  }
}


void GameAnimation::RobotMoveToGear() 
{
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(10, false);
  DisplayRobot(24 - animationSubFrame, false, BLUE);
  if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(10, false);
      DisplayRobot(24 - animationSubFrame, false, RED);
    }
    
}



void GameAnimation::RobotPickupGear() 
{
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(10, true);
  DisplayRobot(12, true, BLUE); 
    if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(10, false);
      DisplayRobot(24 - animationSubFrame, false, RED);
    }
}
  
void GameAnimation::RobotMoveToPeg()  
{
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(10-animationSubFrame, true);
  DisplayRobot(12-animationSubFrame, true, BLUE);
    if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(10-animationSubFrame, false);
      DisplayRobot(12 - animationSubFrame, false, RED);
    }
}

void GameAnimation::RobotPlaceGear()  
{
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(-1, true);
  DisplayRobot(1, true, BLUE);
    if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(-1, false);
      DisplayRobot(1, false, RED);
    }
}
void GameAnimation::RobotMoveToRope() 
{
    Serial.print("robot move to rope");
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(-1, true);
  DisplayRobot(1+animationSubFrame, true, BLUE);
    if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(-1, false);
      DisplayRobot(1+animationSubFrame, false, RED);
    }
}
void GameAnimation::RobotClimbRope()  
{  
  DisplayRope(WHITE);
  DisplayPeg();
  DisplayGear(-1, true);
  DisplayRobotClimbing(12, 5-animationSubFrame, true, BLUE);
    if (alliance == RED)
    {
      DisplayRope(WHITE);
      DisplayPeg();
      DisplayGear(10, false);
      DisplayRobotClimbing(12, 5-animationSubFrame, true, RED);
    }
  int forwbackw = 0;
}

void GameAnimation::DropRope()        {}
void GameAnimation::RaiseGear()       {}



//-------------------------------------------------------------------------------
//display robot body arm up
//offset is x offset
void GameAnimation::DisplayRobot(int offset, bool armUp, int robotColor)
{
//  Serial.print("DisplayRobot offset: ");
//  Serial.println(offset);
  //parameters go (x, y, w, h, color)
  _pMatrix->fillRect(offset+1, 7, 2, 1,  robotColor); //left wheel
  _pMatrix->fillRect(offset+5, 7, 2, 1,  robotColor); //right wheel
  _pMatrix->fillRect(offset+0, 5, 8, 2,  robotColor); //chassis
  _pMatrix->fillRect(offset+6, 4, 2, 1,  robotColor); //winch
  
  if (armUp)
    _pMatrix->fillRect(offset+1, 2, 1, 3,  robotColor); //arm up
  else 
    _pMatrix->fillRect(offset-2, 6, 3, 1,  robotColor); //arm down
  /*_pMatrix->fillCircle(7, 4, 2, YELLOW); //big gear
  _pMatrix->fillRect(5, 4, 1, 1, BLACK); // gear gap 1
  _pMatrix->fillRect(7, 4, 1, 1, BLACK); //gear hole
  _pMatrix->fillRect(9, 4, 1, 1, BLACK); // gg2
  _pMatrix->fillRect(7, 2, 1, 1, BLACK); // gg3
  _pMatrix->fillRect(7, 6, 1, 1, BLACK); // gg4*/
}

void GameAnimation::DisplayGear(int offset, bool vert)
{
  //parameters go (x, y, w, h, color)
  if (vert)
    _pMatrix->fillRect(offset+2, 2, 1, 3, YELLOW);
  else
    _pMatrix->fillRect(offset, 7, 3, 1, YELLOW);
}

void GameAnimation::DisplayPeg()
{
  _pMatrix->drawLine(0, 2, 0, 7, RED); //side peg
  _pMatrix->drawLine(0, 7, 3, 7, RED); //bottom peg
  _pMatrix->drawLine(1, 3, 3, 3, WHITE); //spring
}

void GameAnimation::DisplayRope(uint16_t touchpadColor)
{
  if (frame >= 56)
    touchpadColor = BLUE;
  _pMatrix->drawLine(19, 0, 19, 6, CYAN); 
  _pMatrix->drawRect(18, 0, 3, 1, touchpadColor); 
}

void GameAnimation::DisplayRobotClimbing(int offset, int offset_vert, bool armUp, int robotColor)
{
/*
  //parameters go (x, y, w, h, color)
  _pMatrix->fillRect(offset+1, 7, 2, 1,  robotColor); //left wheel
  _pMatrix->fillRect(offset+5, 7, 2, 1,  robotColor); //right wheel
  //_pMatrix->fillRect(offset+0, 5, 8, 2,  robotColor); //chassis
  _pMatrix->drawLine(offset, 2, 19, 3, robotColor); //chassis top  offset should be 12
  _pMatrix->drawLine(offset, 2, 19, 3, robotColor); //chassis bottom offset sould be 12
  
  _pMatrix->fillRect(offset+6, 4, 2, 1,  robotColor); //winch
  
  if (armUp)
    _pMatrix->fillRect(offset+1, 2, 1, 3,  robotColor); //arm up
  else 
    _pMatrix->fillRect(offset-2, 6, 3, 1,  robotColor); //arm down
*/
  if (frame > 54)
    offset_vert = 2;
  _pMatrix->fillRect(offset+1, offset_vert+2, 2, 1,  robotColor); //left wheel
  _pMatrix->fillRect(offset+5, offset_vert+2, 2, 1,  robotColor); //right wheel
  _pMatrix->fillRect(offset+0, offset_vert, 8, 2,  robotColor); //chassis
  _pMatrix->fillRect(offset+6, offset_vert-1, 2, 1,  robotColor); //winch
  
  if (armUp)
    _pMatrix->fillRect(offset+1, offset_vert-3, 1, 3,  robotColor); //arm up
  else 
    _pMatrix->fillRect(offset-2, offset_vert+1, 3, 1,  robotColor); //arm down
}


