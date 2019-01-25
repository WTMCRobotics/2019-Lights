// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       2019_Banner.ino
    Created:	1/24/2019 9:01:18 PM
    Author:     tls-mobile7\tomh
*/

/*
  Strange Quarks Banner Display

  This program displays test messages and graphic images on a 32x8 NeoPixle
  matrix mounted on top of the Strange Quarks banner. In can be expanded to
  support a second display on the reverse side of the banner.

  A mode switch is supported to cycle through differen display messages, images,
  or special effects.

*/


#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GFX.h>
#include <glcdfont.c>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define FIRST_MODE 0


// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

#define frameMoveToGear 0
#define framePickUpGear 13
#define frameMoveToPeg  16
#define framePlaceGear  28
#define frameMoveToRope 39
#define frameClimbRope  51  
#define frameDone       67  

#define MODE_PIN    5
#define MATRIX_PIN  6

#define MATRIX_ROWS 8
#define MATRIX_COLS 32
#define NUM_MODES   3

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoMatrix matrix =	Adafruit_NeoMatrix(
								MATRIX_COLS,
								MATRIX_ROWS,
								MATRIX_PIN,
								NEO_MATRIX_TOP
								+ NEO_MATRIX_LEFT
								+ NEO_MATRIX_COLUMNS
								+ NEO_MATRIX_ZIGZAG,
								NEO_GRB + NEO_KHZ800
							);


const uint16_t gfx_colors[] =
{
	BLACK,
	BLUE,
	RED,
	GREEN,
	CYAN,
	MAGENTA,
	YELLOW,
	WHITE
};

const uint16_t matrix_colors[] =
{
	matrix.Color(0xFF, 0x00, 0x00),   // red
	matrix.Color(0x00, 0xFF, 0x00),   // green
	matrix.Color(0xFF, 0xFF, 0x00),   // yellow   
	matrix.Color(0x00, 0x00, 0xFF),   // blue
	matrix.Color(0xFF, 0x00, 0xFF),   // purple
	matrix.Color(0x00, 0xFF, 0xFF),   // teal
	matrix.Color(0xFF, 0xFF, 0xFF5)   // white
};

int x = matrix.width();
int msgLen = 0;
int pass = 0;
int curMode = FIRST_MODE;
int nextMode = FIRST_MODE;
int lastModePin = 1;
int scrollDelay = 30;

void setup()
{
	pinMode(MODE_PIN, INPUT_PULLUP);
	lastModePin = digitalRead(MODE_PIN);
	Serial.begin(115200);
	matrix.begin();
	matrix.setTextWrap(false);
	matrix.setBrightness(7);
	matrix.setTextColor(matrix_colors[0]);
}

void loop()
{
	// check for speed change
	//
	if (Serial.available() > 0)
	{
		int delayBump = scrollDelay < 50 ? 5 : 10;
		bool printDelay = false;
		switch (Serial.read())
		{
			case '+':
				// speed up scroll (reduce delay)
				if (scrollDelay > 5)
					scrollDelay -= delayBump;
				printDelay = true;
				break;

			case '-':
				// slow down scroll (increase delay)
				scrollDelay += delayBump;
				printDelay = true;
				break;

			default:
				break;
		}

		if (printDelay)
		{
			Serial.print("scroll delay: ");
			Serial.println(scrollDelay);
		}
	}

	// check for mode change
	//
	int modePin = !digitalRead(MODE_PIN);
	if (modePin && !lastModePin)
	{
		nextMode++;
		if (nextMode >= NUM_MODES)
			nextMode = 0;
		Serial.println(nextMode);
	}
	lastModePin = modePin;

	switch (curMode)
	{
		case 0:
			if (DisplayMode0())
				curMode = nextMode;
			break;

		case 1:
			if (DisplayMode1())
				curMode = nextMode;
			break;

		//experimental
		case 2:
			if (DisplayMode2())
				curMode = nextMode;
			break;
	}
	int forwbackw = 0; //depending on if robot is going forwards or backwards, changes shadow location
}

char str0[] = "Strange Quarks";
char str1[] = "Team 6101";
char str2[] = "Washtenaw Technical Middle College";
char str3[] = "Ann Arbor, MI";

int dispMode0Len = 84;

bool DisplayMode0()
{
	return DisplayMessage(str0);
}

bool DisplayMode1()
{
	return DisplayGame(BLUE);
}

bool DisplayMode2()
{
	return DisplayGame(RED);
}


//=============================================================================================================
//
// Display Text Message
//


bool msgInProcess = false;
int  msgLength;
int  msgIndex;
int  loopCount = 0;

bool DisplayMessage(const char* msg)
{
	if (!msgInProcess)
	{
		msgInProcess = true;
		msgLength = -strlen(msg) * 6;
		msgIndex = matrix.width();
	}

	matrix.fillScreen(0);
	matrix.setCursor(msgIndex, 0);

	if (--msgIndex < msgLength)
	{
		msgInProcess = false;
		return true;
	}

	matrix.print(msg);
	matrix.show();
	delay(scrollDelay);
	return false;
}



//=============================================================================================================
//
// Display Game Animation
//

//variables for DisplayRobot
uint16_t robotColor = BLUE;
uint16_t robotColorB = YELLOW;


//variables for drawing
int robotOffset = 24;
int gearOffset = 10;
bool gearVert = false;
int alliance = BLUE;

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
Step anaimationStep;
int animationSubStep;

//variables for animation
int frame = 0;
int animationSubFrame = 0;
Step animationStep = Idle;

bool DisplayGame(uint32_t alianceColor)
{
	alliance = alianceColor;
	GetAnimationInfo();
	switch (animationStep)
	{
		case Idle:
			break;
		case MoveToGear:
			RobotMoveToGear();
			break;
		case PickUpGear:
			RobotPickupGear();
			break;
		case MoveToPeg:
			RobotMoveToPeg();
			break;
		case PlaceGear:
			RobotPlaceGear();
			//DropRope();
			break;
		case MoveToRope:
			RobotMoveToRope();
			RaiseGear();
			break;
		case ClimbRope:
			RobotClimbRope();
			break;
		case Done:
			return true;
	}

	matrix.show();
	delay(100);

	frame++;
	if (frame >= frameDone)
	{
		delay(500);
		frame = 0;
		return true;
	}

	return false;
}

void GetAnimationInfo()
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
		animationSubFrame = frame - frameMoveToGear;
		animationStep = MoveToGear;
	}
	else
	{
		animationSubFrame = 9999;
		animationStep = Idle;
	}
}

void RobotMoveToGear()
{
	DisplayRope(WHITE, 0);
	DisplayPeg();
	DisplayBox(10, false);
	DisplayRobot(24 - animationSubFrame, BLUE, CYAN, 0);
	if (alliance == RED)
	{
		DisplayRope(WHITE, 0);
		DisplayPeg();
		DisplayBox(0, false);
		DisplayRobot(24 - animationSubFrame, RED, YELLOW, 0);
	}

}

void RobotPickupGear()
{
	DisplayRope(WHITE, 0);
	DisplayPeg();
	DisplayBox(10, false);
	DisplayRobot(12, BLUE, CYAN, 0);
	if (alliance == RED)
	{
		DisplayRope(WHITE, 0);
		DisplayPeg();
		DisplayBox(10, false);
		DisplayRobot(12, RED, YELLOW, 0);
	}
}

void RobotMoveToPeg()
{
	DisplayRope(WHITE, 0);
	DisplayPeg();
	DisplayBox(10 - animationSubFrame, false);
	DisplayRobot(12 - animationSubFrame, BLUE, CYAN, 0);
	if (alliance == RED)
	{
		DisplayRope(WHITE, 0);
		DisplayPeg();
		DisplayBox(10 - animationSubFrame, false);
		DisplayRobot(12 - animationSubFrame, RED, YELLOW, 0);
	}
}

void RobotPlaceGear()
{
	DisplayRope(WHITE, 0);
	DisplayPeg();
	DisplayBox(10, false);
	DisplayRobot(1, BLUE, CYAN, 1);
	if (alliance == RED)
	{
		DisplayRope(WHITE, 0);
		DisplayPeg();
		DisplayBox(10, false);
		DisplayRobot(1, RED, YELLOW, 1);
	}
}
void RobotMoveToRope()
{
	// Serial.print("robot move to rope");
	if (animationSubFrame <= 6)
		DisplayRope(WHITE, 0 + animationSubFrame);
	else
		DisplayRope(WHITE, 6);
	DisplayPeg();
	DisplayBox(10, false);
	DisplayRobot(1 + animationSubFrame, RED, YELLOW, 1);
	if (alliance == RED)
	{
		if (animationSubFrame <= 6)
			DisplayRope(WHITE, 0 + animationSubFrame);
		else
			DisplayRope(WHITE, 6);
		DisplayPeg();
		DisplayBox(10, false);
		DisplayRobot(1 + animationSubFrame, RED, YELLOW, 1);
	}
}
void RobotClimbRope()
{
	DisplayRope(WHITE, 6 - animationSubFrame);
	DisplayPeg();
	DisplayBox(10, false);
	DisplayRobotClimbing(12, 5 - animationSubFrame, true, RED);
	if (alliance == RED)
	{
		DisplayRope(WHITE, 6 - animationSubFrame);
		DisplayPeg();
		DisplayBox(10, false);
		DisplayRobotClimbing(12, 5 - animationSubFrame, true, RED);
	}
}
void DropRope() {}
void RaiseGear() {}



//-------------------------------------------------------------------------------
//display robot body arm up
//offset is x offset
void DisplayRobot(int offset, int robotColor, int robotColorB, int forwbackw)
{
	//  Serial.print("DisplayRobot offset: ");
	//  Serial.println(offset);
	  //parameters go (x, y, w, h, color)
	matrix.fillRect(offset + 1, 7, 2, 1, BLACK); //left wheel SHADOW
	matrix.fillRect(offset + 3, 7, 2, 1, BLACK); //mid wheel SHADOW
	matrix.fillRect(offset + 6, 7, 2, 1, BLACK); //right wheel SHADOW
	matrix.fillRect(offset + 1, 5, 8, 2, BLACK); //chassis SHADOW
	matrix.fillRect(offset + 7, 4, 2, 1, BLACK); //winch SHADOW

	matrix.fillRect(offset + 1, 7, 2, 1, robotColor); //left wheel 
	matrix.fillRect(offset + 3, 7, 2, 1, robotColorB); //mid wheel
	matrix.fillRect(offset + 5, 7, 2, 1, robotColor); //right wheel 
	matrix.fillRect(offset + 0, 5, 8, 2, robotColor); //chassis  
	//matrix.fillRect(offset+6, 4, 2, 1,  robotColor); //winch 

	if (forwbackw = 1) 
	{
		matrix.fillRect(offset + 0, 7, 2, 1, BLACK); //left wheel SHADOW
		matrix.fillRect(offset + 3, 7, 2, 1, BLACK); //mid wheel SHADOW
		matrix.fillRect(offset + 5, 7, 2, 1, BLACK); //right wheel SHADOW
		matrix.fillRect(offset - 1, 5, 8, 2, BLACK); //chassis SHADOW
		matrix.fillRect(offset + 6, 4, 2, 1, BLACK); //winch SHADOW

		matrix.fillRect(offset + 1, 7, 2, 1, robotColor); //left wheel 
		matrix.fillRect(offset + 3, 7, 2, 1, robotColorB); //mid wheel
		matrix.fillRect(offset + 5, 7, 2, 1, robotColor); //right wheel 
		matrix.fillRect(offset + 0, 5, 8, 2, robotColor); //chassis  
		matrix.fillRect(offset + 6, 4, 2, 1, robotColor); //winch
		forwbackw = 0; //depending on if robot is going forwards or backwards, changes shadow location 
	}

	//arm   
	/*  matrix.fillRect(offset+1, 4, 3, 1,  robotColorB); //arm triangle 1
	  matrix.fillRect(offset+2, 3, 3, 2,  robotColorB); // arm triangle 2
	  matrix.fillRect(offset+3, 2, 3, 3,  robotColorB); //arm triangle 3
	//  matrix.fillRect(offset+3, 3, 3, 2,  BLACK); //arm up shadow 2
	//  matrix.fillRect(offset+3, 2, 3, 3,  BLACK); //arm up shadow 3
	  matrix.fillRect(offset+9, 1, 1, 6,  BLACK); //main shadow
	  matrix.fillRect(offset+4, 2, 3, 3,  BLACK);
	  if (forwbackw = 1)
		matrix.fillRect(offset-1, 2, 2, 3, BLACK);
		matrix.fillRect(offset+1, 2, 3, 3,  BLACK); //arm up shadow
		matrix.fillRect(offset+1, 4, 3, 1,  robotColorB); //arm triangle 1
		matrix.fillRect(offset+2, 3, 3, 2,  robotColorB); // arm triangle 2
		matrix.fillRect(offset+3, 2, 3, 3,  robotColorB); //arm triangle 3
		matrix.fillRect(offset+4, 2, 3, 3,  BLACK);

	}

	else {*/
	//parameters ((offset in x referenced from start of chassis, y, w, h, color)
	matrix.fillRect(offset + 3, 4, 1, 1, BLACK); //arm shadow 4
	matrix.fillRect(offset + 4, 3, 1, 2, BLACK); //arm shadow 3
	matrix.fillRect(offset + 2, 2, 1, 3, BLACK); //arm shadow 2
	matrix.fillRect(offset + 1, 1, 1, 4, BLACK); //arm shadow 1
	matrix.fillRect(offset + 1, 1, 1, 4, robotColorB); //arm triangle 1
	matrix.fillRect(offset + 2, 2, 1, 3, robotColorB); // arm triangle 2
	matrix.fillRect(offset + 3, 3, 1, 2, robotColorB); //arm triangle 3
	matrix.fillRect(offset + 3, 4, 1, 1, robotColorB); //arm triangle 4
	matrix.fillRect(offset + 8, 1, 1, 6, BLACK);
	if (forwbackw = 1)
		matrix.fillRect(offset - 1, 2, 2, 3, BLACK);
	matrix.fillRect(offset + 1, 2, 1, 3, BLACK); //arm up shadow
	matrix.fillRect(offset + 1, 1, 1, 4, robotColorB); //arm triangle 1
	matrix.fillRect(offset + 2, 3, 1, 2, robotColorB); // arm triangle 2
	matrix.fillRect(offset + 3, 2, 1, 3, robotColorB); //arm triangle 3
	matrix.fillRect(offset + 4, 2, 1, 3, BLACK);

	//  matrix.fillCircle(7, 4, 2, YELLOW); //big gear
	//  matrix.fillRect(5, 4, 1, 1, BLACK); // gear gap 1
	//  matrix.fillRect(7, 4, 1, 1, BLACK); //gear hole
	//  matrix.fillRect(9, 4, 1, 1, BLACK); // gg2
	//  matrix.fillRect(7, 2, 1, 1, BLACK); // gg3
	//  matrix.fillRect(7, 6, 1, 1, BLACK); // gg4
}

void DisplayBox(int offset, bool vert)
{
	//parameters go (x, y, w, h, color)
	matrix.fillRect(offset, 6, 2, 2, YELLOW);
}

//matrix.drawLine(x1, y1, x2, y2, color);

void DisplayPeg()
{
	matrix.drawLine(0, 2, 0, 7, GREEN); //side peg
	matrix.drawLine(0, 7, 3, 7, GREEN); //bottom peg
	matrix.drawLine(1, 3, 3, 3, WHITE); //spring
}

void DisplayRope(uint16_t touchpadColor, int rope_len)
{
	//rope len should end at 6
	if (frame >= 56)
	{
		if (alliance == BLUE)
			touchpadColor = BLUE;
		else
			touchpadColor = RED;
	}
	matrix.drawLine(19, 0, 19, rope_len, CYAN);
	matrix.drawRect(18, 0, 3, 1, touchpadColor);

}

void DisplayRobotClimbing(int offset, int offset_vert, bool armUp, int robotColor)
{
/*
	//  //parameters go (x, y, w, h, color)
	//  matrix.fillRect(offset+1, 7, 2, 1,  robotColor); //left wheel
	//  matrix.fillRect(offset+5, 7, 2, 1,  robotColor); //right wheel
	//  //matrix.fillRect(offset+0, 5, 8, 2,  robotColor); //chassis
	//  matrix.drawLine(offset, 2, 19, 3, robotColor); //chassis top  offset should be 12
	//  matrix.drawLine(offset, 2, 19, 3, robotColor); //chassis bottom offset sould be 12
	//
	//  matrix.fillRect(offset+6, 4, 2, 1,  robotColor); //winch
	//
	//  if (armUp)
	//    matrix.fillRect(offset+1, 2, 1, 3,  robotColor); //arm up
	//  else
	//    matrix.fillRect(offset-2, 6, 3, 1,  robotColor); //arm down

	  if (frame > 54)
		offset_vert = 2;
	  matrix.fillRect(offset+1, offset_vert+2, 2, 1,  robotColor); //left wheel
	  matrix.fillRect(offset+5, offset_vert+2, 2, 1,  robotColor); //right wheel
	  matrix.fillRect(offset+0, offset_vert, 8, 2,  robotColor); //chassis
	  matrix.fillRect(offset+6, offset_vert-1, 2, 1,  robotColor); //winch

	  if (armUp)
		matrix.fillRect(offset+1, offset_vert-3, 1, 3,  robotColor); //arm up
	  else
		matrix.fillRect(offset-2, offset_vert+1, 3, 1,  robotColor); //arm down
	}
*/
}

