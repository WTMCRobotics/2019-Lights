#if !defined(DISPLAY_MESSAGE_H)
#define DISPLAY_MESSAGE_H

#include <Adafruit_GFX.h>
#include <glcdfont.c>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

class DisplayMessage
{

private:
 
  Adafruit_NeoMatrix* _pMatrix;
  

private:
	DisplayMessage(Adafruit_NeoMatrix* pMatrix);
	bool Display(char* msg);
};

#endif 

