#if !defined(GAME_ANIMATION_H)
#define GAME_ANIMATION_H

#include <Adafruit_GFX.h>
#include <glcdfont.c>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define frameMoveToGear 0
#define framePickUpGear 13
#define frameMoveToPeg  16
#define framePlaceGear  28
#define frameMoveToRope 39
#define frameClimbRope  51  
#define frameDone       67  

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

class GameAnimation
{

 private:
 
  Adafruit_NeoMatrix* _pMatrix;
  
  enum Step
  {
    Idle,
    MoveToGear,
    PickUpGear,
    MoveToPeg,
    PlaceGear,
    MoveToRope,
    ClimbRope,
    Done
  };
	
	//variables for animation
  int x;
	int frame = 0;
	int animationSubFrame = 0;
	Step animationStep = Idle;
	Step anaimationStep;
	int animationSubStep;

	int alliance = BLUE;

private:
  GameAnimation(Adafruit_NeoMatrix* pMatrix);
  
	void GetAnimationInfo();
	void RobotMoveToGear();
	void RobotPickupGear();
	void RobotMoveToPeg();
	void RobotPlaceGear();
	void RobotMoveToRope();
	void RobotClimbRope();
  void DropRope();
  void RaiseGear();
	
	void DisplayRobot(int offset, bool armUp, int robotColor);
	void DisplayGear(int offset, bool vert);
	void DisplayPeg();
  void DisplayRope(uint16_t touchpadColor);
	void DisplayRobotClimbing(int offset, int offset_vert, bool armUp, int robotColor);

};

#endif 

