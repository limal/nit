/*
Copyright (C) 2010 Lukasz Wolnik

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Lukasz Wolnik lukasz.wolnik@o2.pl
*/

#include "StdAfx.h"
#include "application.h"
#include "graphics.h"
#include "gui.h"
#include "timer.h"

using namespace nit;

IApplication::IApplication(const unsigned int width, const unsigned int height, const bool windowed, const bool show_fps)
	:	hWnd(0),
		num_frames(0),
		show_fps(show_fps),
		time_elapsed(0),
		windowed(windowed)
{
	hWnd = NULL;
	minimized = false;

	update = new nitUPDATE();
	memset(update, 0, sizeof(nitUPDATE));
	update->width = width;
	update->height = height;

	Initialize();
}

IApplication::~IApplication()
{
	delete update;
}

void IApplication::AddOnCreated(void* delegate)
{
	created += *((Delegate*)delegate);
}

void IApplication::AddRenderFunction(void* delegate, unsigned int priority)
{
	gfx->AddOnRender(delegate, priority);
}

bool IApplication::CreateMainWindow(wchar_t* title, int cmdShow)
{
	WNDCLASSEX wndClass;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	memset(&wndClass, 0, sizeof(wndClass));

	wndClass.lpszClassName = L"nit";
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = IApplication::WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100));
	wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);

	if (RegisterClassExW (&wndClass) == false)
		return false;

	hWnd = CreateWindowEx(NULL, wndClass.lpszClassName, title, WS_OVERLAPPEDWINDOW, 128, 138, update->width, update->height, NULL, NULL, GetModuleHandle (NULL), this);
	update->hWnd = hWnd;

	ShowCursor(windowed);
	SetWindowText(hWnd, title);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	InitializeGraphics();

	return true;
}

void IApplication::DisplayError(wstring message)
{
	MessageBox(hWnd, message.c_str(), L"DX9 Error", MB_ICONERROR);
}

inline void IApplication::CalculateFps(const float dt)
{
	time_elapsed += dt;
	num_frames += 1.0f;

	if (time_elapsed > 0.10f)
	{
		fps = num_frames / time_elapsed;

		num_frames = 0;
		time_elapsed = 0;
	}
}

void IApplication::CenterWindow(HWND hWnd, int _x, int _y)
{
	unsigned int width = update->width;
	unsigned int height = update->height;

	RECT rectDesktop, rectWindow, rectClient;
	POINT difference;

	GetClientRect(hWnd, &rectClient);
	GetWindowRect(hWnd, &rectWindow);
	GetWindowRect(GetDesktopWindow(), &rectDesktop);
	difference.x = rectWindow.right - rectWindow.left - rectClient.right;
	difference.y = rectWindow.bottom - rectWindow.top - rectClient.bottom;

	int x = (_x == 0) ? (rectDesktop.right / 2) - ((rectWindow.right - rectWindow.left) / 2) : _x;
	int y = (_y == 0) ? (rectDesktop.bottom / 2) - ((rectWindow.bottom - rectWindow.top) / 2) : _y;

	GetWindowRect(GetDesktopWindow(), &rectDesktop);

	MoveWindow(hWnd, x, y, width + difference.x, height + difference.y, true);
}

void IApplication::Update()
{
	if (!gfx->IsDeviceLost())
	{
		gfx->Render(timer->GetDelta());
	}

	//if (update->command == exit)
	//{
	//	PostQuitMessage(0);
	//}

	update->command = 0;
	update->wheel_x = update->wheel_y = 0;
	update->mouse_up = false;
	update->key_pressed = 0;
}

IGui& IApplication::GetGui()
{
	//return gfx->GetGui();
	return *gui;
}

void IApplication::Initialize()
{
	timer = shared_ptr<Timer>(new Timer());	
}

void IApplication::InitializeGraphics()
{
	gfx = shared_ptr<Graphics>(new Graphics(windowed));
	if (gfx->Initialize(update->width, update->height, hWnd) == E_FAIL)
	{
		MessageBox(NULL, L"Couldn't initalize DirectX interface!", L"Error", MB_ICONERROR);
	}	

	//gui = shared_ptr<Gui>(new Gui(*update, gfx.get()));
	//update->gui = (void*)gui.get();

	created((void*)update, (void*)gfx.get());

	gfx->AddOnRender(&MakeDelegate(this, &IApplication::Render), 200);
}

void IApplication::Input(WPARAM wParam, LPARAM lParam)
{
	//input(wParam, lParam);
}

void IApplication::ProcessKeyboardChar(WPARAM wParam, LPARAM lParam)
{
	update->key_pressed = wParam;
	update->key_params = lParam;
	//keyChar(wParam, lParam);
}

void IApplication::ProcessKeyboardDown(WPARAM wParam, LPARAM lParam)
{
	if (update->vKeys[wParam] == false)
	{
		update->prevKeys[wParam] = true;
	}
	else
	{
		update->prevKeys[wParam] = false;
	}

	update->vKeys[wParam] = true;

	//keyDown(wParam, lParam);
}

void IApplication::ProcessKeyboardUp(WPARAM wParam, LPARAM lParam)
{
	update->vKeys[wParam] = false;
	update->prevKeys[wParam] = false;

	//keyUp(wParam, lParam);
}

void IApplication::ProcessMouseLButtonDown(WPARAM wParam, LPARAM lParam)
{
	//mouseLButton(wParam, lParam);
	update->mouse_down = true;
	update->mouse_up = false;
	gui->mouse_down = true;
	gui->mouse_up = false;
}

void IApplication::ProcessMouseLButtonUp(WPARAM wParam, LPARAM lParam)
{
	update->mouse_down = false;
	update->mouse_up = true;
	gui->mouse_down = false;
	gui->mouse_up = true;
}

void IApplication::ProcessMouseMButtonDown(WPARAM wParam, LPARAM lParam)
{
	update->mouse_middle_down = true;
	update->mouse_middle_up = false;
}

void IApplication::ProcessMouseMButtonUp(WPARAM wParam, LPARAM lParam)
{
	update->mouse_middle_down = false;
	update->mouse_middle_up = true;
}

void IApplication::ProcessMouseRButtonDown(WPARAM wParam, LPARAM lParam)
{
	update->mouse_right_down = true;
	update->mouse_right_up = false;
}

void IApplication::ProcessMouseRButtonUp(WPARAM wParam, LPARAM lParam)
{
	update->mouse_right_down = false;
	update->mouse_right_up = true;
}

void IApplication::ProcessMouseMove(WPARAM wParam, LPARAM lParam)
{
	update->mx = GET_X_LPARAM(lParam);
	update->my = GET_Y_LPARAM(lParam);

	//mouseMove(wParam, lParam);
}

void IApplication::ProcessMouseWheel(WPARAM wParam, LPARAM lParam)
{
	update->wheel_y = GET_WHEEL_DELTA_WPARAM(wParam);
}

void IApplication::ProcessResize(WPARAM wParam, LPARAM lParam)
{
	minimized = (wParam == SIZE_MINIMIZED);

	if (gfx)
	{
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		if (clientRect.right > 0 && clientRect.bottom > 0)
		{
			update->width = (unsigned short)clientRect.right;
			update->height = (unsigned short)clientRect.bottom;

			gfx->Resize(clientRect.right, clientRect.bottom);
		}
	}
}

void IApplication::Render(Graphics* gfx, float t)
{
	CalculateFps(t);

	stringstream ss;
#ifndef _DEBUG
	ss << "~D" << ((1.0f / fps) * 1000.0f) << "~A ms (~E" << fps << "~A FPS)";
#else
	ss << "~D" << ((1.0f / fps) * 1000.0f) << "~A ms (~E" << fps << "~A FPS) DEBUG";
#endif
	string text = ss.str();
	if (show_fps)
	{
		gfx->Write(text.c_str(), 32, 32);
	}
}

nitUPDATE* IApplication::Create(wchar_t* title, int nShowCmd)
{
	CreateMainWindow(title, nShowCmd);

	return update;
}

LRESULT CALLBACK IApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	IApplication* app = 0;

	if (message == WM_NCCREATE)
	{
		app = (IApplication*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLong(hWnd, GWL_USERDATA, (long)app);

		return true;
	}
	else
	{
		app = (IApplication*)GetWindowLong(hWnd, GWL_USERDATA);
	}

	switch (message)
	{
		case WM_CHAR:
			app->ProcessKeyboardChar(wParam, lParam);
			return 0;

		case WM_CREATE:
			app->CenterWindow(hWnd);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_EXITSIZEMOVE:
			app->ProcessResize(wParam, lParam);
			return 0;

		case WM_KEYDOWN:
			app->ProcessKeyboardDown(wParam, lParam);
			return 0;

		case WM_KEYUP:
			app->ProcessKeyboardUp(wParam, lParam);
			return 0;

		case WM_LBUTTONDOWN:
			app->ProcessMouseLButtonDown(wParam, lParam);
			return 0;

		case WM_LBUTTONUP:
			app->ProcessMouseLButtonUp(wParam, lParam);
			return 0;

		case WM_MBUTTONDOWN:
			app->ProcessMouseMButtonDown(wParam, lParam);
			return 0;

		case WM_MBUTTONUP:
			app->ProcessMouseMButtonUp(wParam, lParam);
			return 0;

		case WM_RBUTTONDOWN:
			app->ProcessMouseRButtonDown(wParam, lParam);
			return 0;

		case WM_RBUTTONUP:
			app->ProcessMouseRButtonUp(wParam, lParam);
			return 0;

		case WM_MOUSEMOVE:
			app->ProcessMouseMove(wParam, lParam);
			return 0;

		case WM_MOUSEWHEEL:
			app->ProcessMouseWheel(wParam, lParam);
			return 0;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_SIZE:
			app->ProcessResize(wParam, lParam);
			return 0;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
