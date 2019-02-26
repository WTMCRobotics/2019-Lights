// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       2019_Robot.ino
    Created:	1/24/2019 9:21:22 PM
    Author:     tls-mobile7\tomh
*/

#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define stripPin  6
#define i2cSlaveAddress 8
#define NUM_LEDS 300
#define BRIGHTNESS 127

// color definitions
#define clrBLACK	0x000000
#define clrBLUE		0x0000FF
#define clrGREEN	0x00FF00
#define clrRED		0xFF0000
#define clrCYAN		0x00FFFF
#define clrPURPLE	0xFFFF00
#define clrYELLOW	0xFF00FF 
#define clrWHITE	0xFFFFFF
#define clrChase0	0xA00080
#define clrChase1	0xA08000
#define clrChase2	0xFFFF00
// display modes
enum eMode
{
	modeAllWhite	= 'w',	// All Blue
	modeAllBlue		= 'b',	// All Blue
	modeAllGreen	= 'g',	// All Green
	modeAllRed		= 'r',	// All Red
	modeAllOff		= 'o', // All Off  

	 modeChase 		= 'c', // Chase
	 modeBreath 	= 'p', // Breathe
	// modexxxx 	 's', // ??????
	// modeCheckerboard 	= 't', // Checkerboard
};

// Start position assume that the strip physically starts on the left side
// travels across to the right side, then goes across to the 
//  
#define HEIGHT    10
#define WIDTH      5

#define leftStart1        0
#define leftStart2        (leftStart1 + (2*HEIGHT)-1)

#define horzStart         (2*HEIGHT)

#define rightStart2        (horzStart * WIDTH)
#define rightStart1        (rightStart2 + (2*HEIGHT-1))


//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, stripPin, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, stripPin, NEO_GRB + NEO_KHZ800);

eMode mode = modeAllOff;
eMode lastMode = modeAllOff;
int step = 0;
bool serialModeActive = false;


byte neopix_gamma[] = 
{
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
 };


void setup()
{
	strip.setBrightness(BRIGHTNESS);
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'

	Wire.begin(i2cSlaveAddress);
	Wire.onReceive(receiveEvent);

	Serial.begin(115200);

}

void loop()
{
	getMode();
	//  Serial.println(mode);

	if (mode != lastMode)
	{
		step = 0;
		lastMode = mode;
	}

	switch (mode)
	{
	case modeAllBlue:
		colorWipe(strip.Color(0, 0, 255), 0);
		break;

	case modeAllGreen:
		colorWipe(strip.Color(0, 255, 0), 0);
		break;

	case modeAllRed:
		colorWipe(strip.Color(255, 0, 0), 0);
		break;

	case modeAllWhite:
		colorWipe(strip.Color(255, 255, 255), 0);
		break;

	case modeAllOff:
		clearAll();
		break;

	case modeChase:
		chase();
		break;

	//case modeRainbow:
	//	rainbowFade2White(3, 3, 1);
	//	break;

	//case modeRainbowCycle:
	//	break;

	default:
		clearAll();
		break;
	}

	/*
	  // Some example procedures showing how to display to the pixels:
	  colorWipe(strip.Color(255, 0, 0), 50); // Red
	  colorWipe(strip.Color(0, 255, 0), 50); // Green
	  colorWipe(strip.Color(0, 0, 255), 50); // Blue
	  colorWipe(strip.Color(0, 0, 0, 255), 50); // White
	  whiteOverRainbow(20,75,5);
	  pulseWhite(5);
	  // fullWhite();
	  // delay(2000);
	  rainbowFade2White(3,3,1);
	*/
}

void getMode(void)
{
	if (Serial.peek() != -1)
	{
		int ch = Serial.read();
		Serial.println(ch, HEX);

		if (ch == 'Q' || ch == 'q')
		{
			serialModeActive = false;
			return;
		}

		mode = ch;
		serialModeActive = true;
	}
}

void receiveEvent(int howMany)
{
	if (Wire.available() == 0)
		return;

	if (!serialModeActive)
	{
		mode = Wire.read();
	}
}

void clearAll(void)
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		strip.setPixelColor(i, clrBLACK);
	}
	strip.show();
}

void colorWipe(uint32_t c, uint8_t wait)
{
	for (uint16_t i = 0; i < 300 /*strip.numPixels()*/; i++)
	{
		strip.setPixelColor(i, c);
	}
	strip.show();
}

void pulseWhite(uint8_t wait) 
{
	for (int j = 0; j < 256; j++) 
	{
		for (uint16_t i = 0; i < strip.numPixels(); i++) 
		{
			strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j]));
		}
		delay(wait);
		strip.show();
	}

	for (int j = 255; j >= 0; j--) 
	{
		for (uint16_t i = 0; i < strip.numPixels(); i++) 
		{
			strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j]));
		}
		delay(wait);
		strip.show();
	}
}

void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) 
{
	float fadeMax = 100.0;
	int fadeVal = 0;
	uint32_t wheelVal;
	int redVal, greenVal, blueVal;

	for (int k = 0; k < rainbowLoops; k++) 
	{

		for (int j = 0; j < 256; j++) 
		{
			// 5 cycles of all colors on wheel
			for (int i = 0; i < strip.numPixels(); i++) 
			{
				wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

				redVal = red(wheelVal) * float(fadeVal / fadeMax);
				greenVal = green(wheelVal) * float(fadeVal / fadeMax);
				blueVal = blue(wheelVal) * float(fadeVal / fadeMax);

				strip.setPixelColor(i, strip.Color(redVal, greenVal, blueVal));
			}

			//First loop, fade in!
			if (k == 0 && fadeVal < fadeMax - 1) 
			{
				fadeVal++;
			}
			else if (k == rainbowLoops - 1 && j > 255 - fadeMax) 
			{
				//Last loop, fade out!
				fadeVal--;
			}

			strip.show();
			delay(wait);
		}

	}

	delay(500);

	for (int k = 0; k < whiteLoops; k++) 
	{
		for (int j = 0; j < 256; j++) 
		{

			for (uint16_t i = 0; i < strip.numPixels(); i++) {
				strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j]));
			}
			strip.show();
		}

		delay(2000);
		for (int j = 255; j >= 0; j--) 
		{
			for (uint16_t i = 0; i < strip.numPixels(); i++) 
			{
				strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j]));
			}
			strip.show();
		}
	}

	delay(500);
}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength) 
{
	if (whiteLength >= strip.numPixels()) 
		whiteLength = strip.numPixels() - 1;

	int head = whiteLength - 1;
	int tail = 0;

	int loops = 3;
	int loopNum = 0;

	static unsigned long lastTime = 0;


	while (true) 
	{
		for (int j = 0; j < 256; j++) 
		{
			for (uint16_t i = 0; i < strip.numPixels(); i++) 
			{
				if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head)) {
					strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
				}
				else {
					strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
				}

			}

			if (millis() - lastTime > whiteSpeed) {
				head++;
				tail++;
				if (head == strip.numPixels()) {
					loopNum++;
				}
				lastTime = millis();
			}

			if (loopNum == loops) return;

			head %= strip.numPixels();
			tail %= strip.numPixels();
			strip.show();
			delay(wait);
		}
	}
}

void chase()
{	
	for (int i = 0; i < NUM_LEDS; i++)
	{
		if (Serial.peek() != -1)
		{
			char ch = Serial.read();
			if (ch == 'q') return;
			while (true) 
			{
				while (Serial.peek() == -1) {}
				ch = Serial.read();
				if (ch == 'q') return;
				if (ch == 's') break;
			}
		}

		switch (i)
		{
		case 0:
			strip.setPixelColor(i, clrChase2);
			break;
		case 1:
			strip.setPixelColor(i - 1, clrChase2);
			strip.setPixelColor(i, clrChase1);
			break;
		case 2:
			strip.setPixelColor(i - 2, clrChase2);
			strip.setPixelColor(i - 1, clrChase1);
			strip.setPixelColor(i, clrChase0);
			break;
		case NUM_LEDS - 3:
			strip.setPixelColor(i - 1, clrBLACK);
			strip.setPixelColor(i - 1, clrChase2);
			strip.setPixelColor(i, clrChase1);
			break;
		case NUM_LEDS - 2:
			strip.setPixelColor(i - 1, clrBLACK);
			strip.setPixelColor(i, clrChase2);
			break;
		case NUM_LEDS - 1:
			strip.setPixelColor(i, clrBLACK);
			break;
		default:
			strip.setPixelColor(i-3, clrBLACK);
			strip.setPixelColor(i-2, clrChase2);
			strip.setPixelColor(i-1, clrChase1);
			strip.setPixelColor(i, clrChase0);
			break;
		}
		strip.show();
		delay(10);
	}
	clearAll();
	delay(100);
}

void fullWhite() 
{
	for (uint16_t i = 0; i < strip.numPixels(); i++) 
	{
		strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
	}
	strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
	uint16_t i, j;

	for (j = 0; j < 256 * 5; j++) // 5 cycles of all colors on wheel
	{
		for (i = 0; i < strip.numPixels(); i++) 
		{
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

void rainbow(uint8_t wait) 
{
	uint16_t i, j;

	for (j = 0; j < 256; j++) 
	{
		for (i = 0; i < strip.numPixels(); i++) 
		{
			strip.setPixelColor(i, Wheel((i + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) 
	{
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
	}
	if (WheelPos < 170) 
	{
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
	}
	WheelPos -= 170;
	return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

uint8_t red(uint32_t c) 
{
	return (c >> 16);
}

uint8_t green(uint32_t c) 
{
	return (c >> 8);
}

uint8_t blue(uint32_t c) 
{
	return (c);
}

