//Core Systems Definations.............................................................................................
#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include"Util.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<assert.h>
#include<atlstr.h>
#include<windows.h>
#include<windowsx.h>
#include<string>
#include<fstream>
#include<sstream>
#include<list>
#include <atlbase.h>
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
#include<DirectXMath.h>

using namespace std;

#include"Vector.h"
#include"Graphics.h"
#include "Vertex.h"

#include"Input.h"
#include"Timer.h"
#include"Camera.h"


//Extern functions connection outside application to the engine
extern int CoreMain(HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#ifndef _COLORS_
#define _COLORS_

namespace Colors
{
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Sandy = { 1.0f,0.96f,0.62f,1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Light_Yellow_Green = { 0.48f,0.77f,0.46f,1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Dark_Yellow_Green = { 0.1f,0.48f,0.19f,1.0f };
	extern CONST DECLSPEC_SELECTANY DirectX::XMFLOAT4 Brown = {0.45f,0.39f,0.34f,1.0f};
};
#endif


namespace CoreSystems
{
	class Graphics;
	class Model;
	class Core
	{
	public:
		Core(LPCWSTR title,bool fullscreen,HINSTANCE hInstance);
		~Core();
		int run();
		void initD3d();
		virtual void Start() {}
		void initWnd();
		HINSTANCE GetInstanceHandler() { return m_hInstance; }
		
		void virtual CoreUpdate() {}
		void Draw();
		void QuitEngine() { quit_engine = true; }
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
		void ShowMessageBox(LPCSTR message, LPCSTR title);
		//Get methods
		HWND GetHWND()  { return m_wnd; }
		bool GetFulscreen() { return fullscreen; }
		HINSTANCE GetHinstance() { return m_hInstance; }
		void Initialize();
		Graphics* GetGraphics() const { return p_graphics; }
		void AddInList(Model* model);

	private:
		int val;
		HINSTANCE m_hInstance;
		HWND m_wnd;
		LPCWSTR m_title;
		bool quit_engine;
		bool fullscreen;
		ID3D11Buffer vb;
		ID3D11Buffer ib;
		const ID3D11Device* const device;
		const ID3D11DeviceContext* const deviceContext;
		Graphics* p_graphics;
		list < Model*> m_modelList;
	};
};
//External Linkage to Core Class Object
extern CoreSystems::Core* gp_Core;

#endif