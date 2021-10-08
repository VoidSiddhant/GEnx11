#include"Core.h"
extern CoreSystems::Core* gp_Core = nullptr; 

namespace CoreSystems
{
	Core::Core(LPCWSTR title, bool fullscreen,HINSTANCE hInstance)
	{
		p_graphics = 0;
		//m_modelList.clear();
		m_hInstance = hInstance;
		m_title = title;
		this->fullscreen = fullscreen;
		quit_engine = false;
	}

	void Core::Initialize()
	{
		this->initWnd();
		this->initD3d();
	}


	void Core::initD3d()
	{
		//Call Graphics Class here
		p_graphics = new Graphics(fullscreen);
		p_graphics->Initialize();
		*device = p_graphics->GetDevice();
		*deviceContext = p_graphics->GetContext();
	}

	void Core::initWnd()
	{
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(WNDCLASS));
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = m_hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpfnWndProc = WndProc;
		wc.hbrBackground = HBRUSH(COLOR_WINDOW);
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.lpszMenuName = 0;
		wc.lpszClassName = L"GEnx11";


		RegisterClass(&wc);

		m_wnd = CreateWindow(L"GEnx11", m_title, WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, m_hInstance, NULL);
		if (!m_wnd)
		{
			MessageBox(m_wnd, L"Window Could Not Be Created", L"Error!!!", MB_OK);
		}
		ShowWindow(m_wnd, SW_SHOW);
		UpdateWindow(m_wnd);
	}

	int Core::run()
	{
		MSG msg;
		msg.message = NULL;
		timer = new Timer();
		input = new Input();
		camera = new Camera(Vector3(0.0f,0.0f,0.0f),Vector3(0.0f,0.0f,1.0f)
			                ,Vector3(0.0f,1.0f,0.0f));
		//camera = new Camera();
		this->Start();
		while (!QUIT_ENGINE)
		{
			if (PeekMessage(&msg, m_wnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//Check for Escape Key
			else if (msg.message == WM_QUIT)
			{
				QUIT_ENGINE = true;
			}
			else
			{
				//Other Frame Related Systems Run
				//Run Timer Update ticks..........
				input->Poll();
				timer->Update();
				timer->CalculateFPS();
				//System Update Call
				CoreUpdate();
				//Core Draw Method to draw out all the list contents
				this->Draw();
				input->SetPrevMouseAxis();
			}
		}
		return (int)msg.wParam;
	}

	void Core::Draw()
	{
//Begin Scene
		p_graphics->BeginDraw();
//Drawing Calls Here......
		//Iterate Through Model Class Drawing Each Model		
		list<Model*>::iterator i = m_modelList.begin();
		while (i != m_modelList.end())
		{
			(*i)->Draw();
			i++;
    	}

//End Scene
		p_graphics->EndDraw();
	}

	LRESULT CALLBACK Core::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			quit_engine = true;
			PostQuitMessage(0);
			return 0;
			break;
		case WM_SIZE:
			if (camera == nullptr)
				break;
			RECT r;
			GetClientRect(m_wnd, &r);
			camera->UpdateProjectionMatrix((int)r.right, (int)r.bottom);
			p_graphics->OnResize(wParam);
			break;
		case WM_MOUSEMOVE:
			input->SetMouseAxis(lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			input->SetMouseButtonDown(wParam);
			break;
		case WM_LBUTTONUP:
			input->SetMouseButtonUp(0);
			break;
		case WM_MBUTTONUP:
			input->SetMouseButtonUp(1);
			break;
		case WM_RBUTTONUP:
			input->SetMouseButtonUp(2);
			break;

		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Core::AddInList(Model* model)
	{
		m_modelList.push_back(model);
	}


	void Core::ShowMessageBox(LPCSTR message, LPCSTR title)
	{
		//LPCWSTR wstr = reinterpret_cast<LPCWSTR>(message);
		USES_CONVERSION;
		MessageBox(m_wnd,A2W(message), A2W(title), MB_OK);
	}

	Core::~Core()
	{
//		MessageBox(m_wnd, L"DEstruct", L"oty", MB_OK);
		delete p_graphics;
		delete input;
		delete timer;
	}

	
};//End of Namespace
//................CLASS DEFINATIONS ENDED.....................................................

 //Message Handling Functions
LRESULT CALLBACK WndProc(HWND hwnd, UINT uInt, WPARAM wParam, LPARAM lParam)
{
	return gp_Core->MessageHandler(hwnd, uInt, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//Return CoreMain To Win Main Exit Path
	return CoreMain(hInstance);
}