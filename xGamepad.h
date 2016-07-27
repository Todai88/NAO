
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#include <iostream>

//Link XInput
#pragma comment(lib, "XInput.lib")



// xGamepad class def
class xGamepad
{
private:
	XINPUT_STATE padState;
	int padNum;
public:
	xGamepad(int pNum);
	XINPUT_STATE GetState();
	bool isConnected();
	void vibrate(int left = 0, int right = 0);
};
