#include"Input.h"

namespace CoreSystems
{
	extern Input* input = 0;
	Input::Input()
	{
		registeredButton = 999;
		ZeroMemory(&keyState, sizeof(keyState));
		ZeroMemory(&mouseState, sizeof(mouseState));
		ZeroMemory(&prevKeyState, sizeof(prevKeyState));
		ZeroMemory(&mouseBDown, sizeof(mouseBDown));
		ZeroMemory(&mouseBUp, sizeof(mouseBUp));

		DirectInput8Create(gp_Core->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL);

		//Create Keyboard Device
		di->CreateDevice(GUID_SysKeyboard, &dev_keyboard, NULL);
		dev_keyboard->SetDataFormat(&c_dfDIKeyboard);
		dev_keyboard->SetCooperativeLevel(gp_Core->GetHWND(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		dev_keyboard->Acquire();

		//Create Mouse Device
		di->CreateDevice(GUID_SysMouse, &dev_mouse, NULL);
		dev_mouse->SetDataFormat(&c_dfDIMouse2);
		dev_mouse->SetCooperativeLevel(gp_Core->GetHWND(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		dev_mouse->Acquire();
	}

	Input::~Input()
	{
		//fopen("Exit.txt", "a");

		//fclose(gp_Engine->GetDestFile());

		dev_keyboard->Unacquire();
		dev_mouse->Unacquire();
		dev_keyboard->Release();
		dev_mouse->Release();
		di->Release();

	}

	void Input::Poll()
	{
		//Poll Keyboard
		HRESULT hr = dev_keyboard->GetDeviceState(sizeof(keyState), (void**)&keyState);
		if (FAILED(hr))       // WHILE FAILED NOT IF FAILED FOR PAUSING GAME ON INPUT LOSS
		{
			ZeroMemory(keyState, sizeof(keyState));
			hr = dev_keyboard->Acquire();
		}

		//Poll Mouse
		hr = dev_mouse->GetDeviceState(sizeof(mouseState), (void**)&mouseState);
		if (FAILED(hr))
		{
			ZeroMemory(&mouseState, sizeof(mouseState));
			hr = dev_mouse->Acquire();
		}
	}

	bool Input::GetKeyDown(char key)
	{
		if ((keyState[key] & 0x80) != 0)
		{
			dev_keyboard->GetDeviceState(sizeof(prevKeyState), (void**)prevKeyState);
			return true;
		}
		else
			return false;
		//return (keyState[key] & 0x80) !=0;
	}

	void Input::SetMouseButtonDown(WPARAM button)
	{
		switch (button)
		{
		case MK_LBUTTON:
			mouseBDown[0] = true;
			mouseBUp[0] = false;
			break;
		case MK_RBUTTON:
			mouseBDown[1] = true;
			mouseBUp[1] = false;
			break;
		case MK_MBUTTON:
			mouseBDown[2] = true;
			mouseBUp[2] = false;
			break;
		}
	}


	void Input::SetMouseButtonUp(int button)
	{
		switch (button)
		{
		case 0:
			mouseBUp[0] = true;
			mouseBDown[0] = false;
			break;
		case 1:
			mouseBUp[1] = true;
			mouseBDown[1] = false;
			break;
		case 2:
			mouseBUp[2] = true;
			mouseBDown[2] = false;
			break;
		}
	}

	void Input::SetMouseAxis(LPARAM lparam)
	{
		mouseAxis.x = (float)GET_X_LPARAM(lparam);
		mouseAxis.y = (float)GET_Y_LPARAM(lparam);
		mouseAxis.z = 0.0f;
	}

	void Input::SetPrevMouseAxis()
	{
		prevMouseAxis = mouseAxis;
	}

	bool Input::GetKeyUp(char key)
	{
		this->GetKeyDown(key);
		if ((keyState[key] & 0x80) == 0 && (prevKeyState[key] & 0x80) != 0)
		{
			dev_keyboard->GetDeviceState(sizeof(prevKeyState), (void**)prevKeyState);
			return true;
		}
		else
			return false;
	}

	bool Input::GetMouseBtnUp(int button)
	{
		if (mouseBUp[button] == true)
		{
			mouseBUp[button] = false;
			return true;
		}
		else
			return false;
	}

	bool Input::GetMouseButton(int button)
	{
		return mouseBDown[button];
	}

	bool Input::GetMouseBtnDown(int button)
	{
		return mouseBDown[button];
	}

	bool Input::MouseMoved()
	{
		if (prevMouseAxis.x != mouseAxis.x || prevMouseAxis.y != mouseAxis.y )
		{
			return true;
		}

		else
		{
			return false;
		}
		
	}

	int Input::GetHorizontalAxis()
	{

		if (this->GetKeyDown(HorizontalAxisP))
			return 1;
		else if (this->GetKeyDown(HorizontalAxisN))
			return -1;
		else
			return 0;
	}

	int Input::GetVerticalAxis()
	{
		if (this->GetKeyDown(VerticalAxisP))
			return 1;
		else if (this->GetKeyDown(VerticalAxisN))
			return -1;
		else
			return 0;
	}

};