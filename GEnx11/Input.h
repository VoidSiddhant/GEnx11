
#ifndef INPUT_H
#define INPUT_H
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#include<string>

#include"Core.h"

#define HorizontalAxisN DIK_A
#define HorizontalAxisP DIK_D
#define VerticalAxisP DIK_W
#define VerticalAxisN DIK_S
#define GetKDown(x) (keyState[static_cast<char>(x)] & 0x80)!=0
namespace CoreSystems
{
	static char keyState[256];            /// CHANGES HEREEEEEE...................

	class Input
	{
	public:
		Input();
		~Input();
		void Poll();
		bool GetKeyDown(char keyname);
		bool GetKeyUp(char key);

		void SetMouseButtonDown(WPARAM button);
		void SetMouseButtonUp(int button);
		void SetMouseAxis(LPARAM);
		void SetPrevMouseAxis();
		//Returns True when the button is pressed for the firs time
		bool GetMouseButton(int button);
		//Returns True till the button is pressed down
		bool GetMouseBtnDown(int button);
		//Returns True when the button is lifted
		bool GetMouseBtnUp(int button);
		int GetHorizontalAxis(void);
		int GetVerticalAxis(void);
		bool MouseMoved(void);
		float GetMouseAxisX(void) const { return mouseAxis.x; }
		float GetMouseAxisY(void) const { return mouseAxis.y; }
		long GetMouseWheelAxis(void) const { return mouseState.lZ; }

	private:
		IDirectInput8* di;
		IDirectInputDevice8* dev_keyboard;
		IDirectInputDevice8* dev_mouse;
		char prevKeyState[256];
		bool mouseBDown[3];
		bool mouseBUp[3];
		DIMOUSESTATE2 mouseState;
		std::string currentKey;
		Vector3 mouseAxis;
		Vector3 prevMouseAxis;
		int registeredButton;
	};
	extern Input* input;
};
#endif