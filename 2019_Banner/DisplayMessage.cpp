//=======================================================================================
//
//	Display text message on matrix
//
//=======================================================================================

#include "DisplayMessage.h"

DisplayMessage::DisplayMessage(Adafruit_NeoMatrix* pMatrix)
{
	_pMatrix = pMatrix;
}

bool DisplayMessage::Display(char* msg)
{
	return true;
}
