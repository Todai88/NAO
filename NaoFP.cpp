/*
 * Nao First Person main class
 * Robert Petre, Joakim Bajoul Kakaei
 * VIP Project Group
 */



//Includes
#include <windows.h>
#include <windowsx.h>
#include "build-tc1\resource.h"
#include <iostream>
#include <alcommon\albroker.h>
#include <boost\shared_ptr.hpp>
#include <alerror\alerror.h>
#include <alcommon\albrokermanager.h>
#include <alcommon\alproxy.h>
#include "Grabber.h"
#include "Motionizr.h"
#include "xGamepad.h"
#include "xGUI.h"
#include <boost\thread.hpp>



//Define
#define WIN32_LEAN_AND_MEAN
#define IDC_EDIT_BOX 101
#define IDC_BUTTON_CON 102

// Globals
boost::shared_ptr<AL::ALBroker> xBroker;
HBITMAP NaoView = NULL;
AL::ALProxy* padProxy;
xGamepad* pad;
HWND hEdit;

 LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);
 void padLoop()
{
	pad = new xGamepad(1);
	padProxy = new AL::ALProxy(xBroker, "Motionizr");
	bool hand;
	while(true)
	{
		
		if(padProxy->call<bool>("isVibrating"))
		{
			pad->vibrate(10000,10000);
			padProxy->callVoid("setVibration",false);
		}
		else 
		{
			pad->vibrate(0,0);
		}
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			int a = padProxy->call<int>("switchMode");
		}
		hand = padProxy->call<bool>("isHandMode");
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) break;
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			padProxy->callVoid("moveHead",0.5f,0.5f);
		}
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			padProxy->callVoid("moveHead",-0.5f,-0.5f);
		}
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			if (padProxy->call<bool>("isHandMode")) padProxy->callVoid("closeHand",(std::string)"RHand");
		}
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			if (padProxy->call<bool>("isHandMode")) padProxy->callVoid("closeHand",(std::string)"LHand");
		}
		//Left stick
		float left_stick_X = 0;
		float left_stick_Y = 0;
		if( (pad->GetState().Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			pad->GetState().Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad->GetState().Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			pad->GetState().Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
		{    
			left_stick_X = 0;
			left_stick_Y = 0;
		 }
		else 
		{
			if(!hand) 
			{
				left_stick_X = pad->GetState().Gamepad.sThumbLX / 32767.0f;
				left_stick_Y = pad->GetState().Gamepad.sThumbLY / 32767.0f;
				if(left_stick_Y > 0.8f) left_stick_Y = 0.8f;
				if(left_stick_Y < -0.5f) left_stick_Y = -0.5f;
				if(left_stick_X < 0.3f && left_stick_X > -0.3f) left_stick_X = 0;
				if(left_stick_X > 1) left_stick_X = 1.0f;
				if(left_stick_X < -1) left_stick_X = -1.0f;
			}
			else 
			{
				left_stick_X = pad->GetState().Gamepad.sThumbLX / 32767.0f;
				left_stick_Y = pad->GetState().Gamepad.sThumbLY / 32767.0f;
				if (left_stick_X > -0.4 && left_stick_X <0.4) left_stick_X = 0;
				if (left_stick_Y > -0.4 && left_stick_Y <0.4) left_stick_Y = 0;
				left_stick_X/=-3;
				left_stick_Y/=-5;
			}
		}
		if(left_stick_X || left_stick_Y)
		{
			if (!hand) 
			{
				if (left_stick_X != 0) left_stick_X*=-1.0f;				
				padProxy->callVoid("startMoving",left_stick_Y,left_stick_X);
				std::cout<<"speed: "<<left_stick_Y<<" t: "<<left_stick_X<<std::endl;
			}
			else
			{
				padProxy->callVoid("moveLeftHand", left_stick_Y, left_stick_X);
			}
			
		}
		else 
		{
			padProxy->callVoid("stopMoving");
		}
		//Right stick
		float right_stick_X = 0;
		float right_stick_Y = 0;
		if( (pad->GetState().Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			pad->GetState().Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad->GetState().Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			pad->GetState().Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
		{    
			right_stick_X = 0;
			right_stick_Y = 0;
		 }
		else 
		{
			right_stick_X = pad->GetState().Gamepad.sThumbRX;
			right_stick_Y = pad->GetState().Gamepad.sThumbRY;
			if (hand) 
			{
				right_stick_X /= 32767.0f;
				right_stick_Y /= 32767.0f;
				if (right_stick_X > -0.4 && right_stick_X <0.4) right_stick_X = 0;
				if (right_stick_Y > -0.4 && right_stick_Y <0.4) right_stick_Y = 0;
				right_stick_X/=-5;
				right_stick_Y/=-5;
			}
			
		}
		if(right_stick_X || right_stick_Y)
		{
			if (hand)
			{
				padProxy->callVoid("moveRightHand", right_stick_Y, right_stick_X);
			}
			else 
			{
				if(right_stick_X>10000) right_stick_X=-0.2f;
				else if(right_stick_X<-10000) right_stick_X=0.2f;
				else right_stick_X=0.0f;
				if(right_stick_Y>10000) right_stick_Y=-0.2f;
				else if(right_stick_Y<-10000) right_stick_Y=0.2f;
				else right_stick_Y=0.0f;
				padProxy->callVoid("moveHead",right_stick_X,right_stick_Y);
			}
		}
		else 
		{
			padProxy->callVoid("resetHead");
		}
		// Arm extensions
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) 
		{
			std::cout<<"shoulder";
			padProxy->callVoid("contractLeft");
		}
		if (pad->GetState().Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) 
		{
			std::cout<<"trigger";
			padProxy->callVoid("extendLeft");
		}
		if (pad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
		{
			std::cout<<"shoulder";
			padProxy->callVoid("contractRight");
		}
		if (pad->GetState().Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) 
		{
			std::cout<<"trigger";
			padProxy->callVoid("extendRight");
		}
	}

}
 bool loadModules(const std::string pbrokerIP)
{
	setlocale(LC_NUMERIC, "C");
	// Set broker params
	//const std::string pbrokerIP = "127.0.0.1";
	int pbrokerPort = 9559;	
	const std::string brokerIP = "0.0.0.0";
	int brokerPort = 54000;	
	const std::string brokerName = "xploreBroker";

	
	// Init broker
	try 
	{
		xBroker = AL::ALBroker::createBroker(brokerName, brokerIP, brokerPort, pbrokerIP, pbrokerPort, 0);
	}
	catch (AL::ALError e)
	{
		AL::ALBrokerManager::getInstance()->killAllBroker();
		AL::ALBrokerManager::kill();
		return false;
	}
	AL::ALBrokerManager::setInstance(xBroker->fBrokerManager.lock());
	AL::ALBrokerManager::getInstance()->addBroker(xBroker);
	
	// Connect modules
	AL::ALModule::createModule<Motionizr>(xBroker, "Motionizr");
	AL::ALModule::createModule<VidModule>(xBroker, "VidModule");
	padProxy = new AL::ALProxy(xBroker, "Motionizr");
	return true;
}
int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{// the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;
    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL),
	MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
    // register the window class
    RegisterClassEx(&wc);

   
    hWnd = CreateWindowEx(NULL,
                          "WindowClass1",    
                          "NAO First Person",   
                          WS_OVERLAPPEDWINDOW,  
                          300,    
                          300,   
                          516,    
                          229, 
                          NULL,   
                          NULL,    
                          hInstance,   
                          NULL);   

    
    ShowWindow(hWnd, nCmdShow);

   

   
    MSG msg;
   
    while(GetMessage(&msg, NULL, 0, 0))
    {
        
        TranslateMessage(&msg);

        
        DispatchMessage(&msg);
    }

   
    return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   
    switch(message)
    {
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			
			HDC hMemDC=CreateCompatibleDC(hdc); 
			::SelectObject(hMemDC,NaoView);           
			BitBlt(hdc,0,0,516,229,hMemDC,0,0,SRCCOPY); 
			::DeleteDC(hMemDC) ;    
			EndPaint(hWnd, &ps);
		}
			break;
		case WM_CREATE:
			{
				
				hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
					"EDIT",
					"",
					WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
					200,
					120,
					100,
					25,
					hWnd,
					(HMENU)IDC_EDIT_BOX,
					GetModuleHandle(NULL),
					NULL);
					HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
				SendMessage(hEdit,
					WM_SETFONT,
					(WPARAM)hfDefault,
					MAKELPARAM(FALSE,0));
				SendMessage(hEdit,
					WM_SETTEXT,
					NULL,
					(LPARAM)"127.0.0.1");
				
				HWND hWndButton=CreateWindowEx(NULL,
				"BUTTON",
				"Connect",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				200,
				155,
				100,
				24,
				hWnd,
				(HMENU)IDC_BUTTON_CON,
				GetModuleHandle(NULL),
				NULL);
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));




				NaoView = (HBITMAP)LoadImage(NULL, (LPCSTR)"img.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
				
				if(NaoView == NULL) MessageBox(hWnd, "Could not load background image!", "Error", MB_OK | MB_ICONEXCLAMATION);

			} break;
			
		case WM_DESTROY:
            {
                
                PostQuitMessage(0);
                return 0;
            } break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_BUTTON_CON:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					if (!loadModules(buffer)) MessageBox(hWnd, "Could not connect to Nao!", "Error", MB_OK | MB_ICONEXCLAMATION);
					else 
					{
						boost::thread padThread(padLoop);
						
					}
				}
				break;
			}
			break;
    }

    
    return DefWindowProc (hWnd, message, wParam, lParam);
}


