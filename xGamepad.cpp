#include "xGamepad.h"

xGamepad::xGamepad(int pNum)
{
	padNum = pNum - 1;
}

XINPUT_STATE xGamepad::GetState()
{
	ZeroMemory(&padState, sizeof(XINPUT_STATE));
	XInputGetState(padNum, &padState);
	return padState;
}

bool xGamepad::isConnected()
{
	ZeroMemory(&padState, sizeof(XINPUT_STATE));
	DWORD result = XInputGetState(padNum, &padState);
	if(result == ERROR_SUCCESS) return true;
	else return false;
}

void xGamepad::vibrate(int left, int right)
{
	XINPUT_VIBRATION Vibration;
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
	Vibration.wLeftMotorSpeed = left;
	Vibration.wRightMotorSpeed = right;
	XInputSetState(padNum, &Vibration);
}


