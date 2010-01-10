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

#pragma once

struct UPDATE;

namespace nit
{
	//class Chart;
	//class Fem1d;
	//class Fuelcell;
	class Graphics;
	class Gui;
	//class GuiMenu;
	class Timer;

	typedef FastDelegate2<void*, void*> Delegate;

	class EventQueue
	{
		typedef std::set<Delegate> Container;

		Container list;

	public:
		void operator +=(Delegate dg) { list.insert(dg); }
		void operator -=(Delegate dg) { list.erase(dg); }
		void operator ()(void* a, void* b)
		{
			for (Container::iterator it = list.begin(); it != list.end(); ++it)
			{
				(*it)(a, b);
			}
		}
	};

	class IApplication : public Application
	{
		public:
			IApplication(const unsigned int width, const unsigned int height, const bool windowed, const bool show_fps);
			~IApplication();

			void AddOnCreated(void* delegate);
			void AddRenderFunction(void* delegate, unsigned int priority);
			void CenterWindow(HWND hWnd, int _x = 0, int _y = 0);
			UPDATE* Create(wchar_t* title, int nShowCmd);
			bool CreateMainWindow(wchar_t* title, int cmdShow);			
			void DisplayError(wstring message);
			nitGraphics* GetGraphics() { return (nitGraphics*)gfx.get(); }
			Gui& GetGui();
			HWND GetWindow() { return hWnd; }
			void Initialize();
			void Input(WPARAM wParam, LPARAM lParam);
			void ProcessKeyboardChar(WPARAM wParam, LPARAM lParam);
			void ProcessKeyboardDown(WPARAM wParam, LPARAM lParam);
			void ProcessKeyboardUp(WPARAM wParam, LPARAM lParam);
			void ProcessMouseLButtonDown(WPARAM wParam, LPARAM lParam);
			void ProcessMouseLButtonUp(WPARAM wParam, LPARAM lParam);
			void ProcessMouseMButtonDown(WPARAM wParam, LPARAM lParam);
			void ProcessMouseMButtonUp(WPARAM wParam, LPARAM lParam);
			void ProcessMouseRButtonDown(WPARAM wParam, LPARAM lParam);
			void ProcessMouseRButtonUp(WPARAM wParam, LPARAM lParam);
			void ProcessMouseMove(WPARAM wParam, LPARAM lParam);
			void ProcessMouseWheel(WPARAM wParam, LPARAM lParam);
			void ProcessPaint(WPARAM wParam, LPARAM lParam);
			void ProcessResize(WPARAM wParam, LPARAM lParam);
			void ReleaseCapture();
			void Render(Graphics* gfx, float t);			
			void SetCapture();
			void Update();

			bool windowed, minimized;
			//unsigned short mx, my; // mouse cooridnates
			short dmx, dmy, wheel_delta;
			unsigned int mstate; // mouse button state
			//unsigned short width, height; // viewport size
			EventQueue created, /*frame,*/ input, keyChar, keyDown, keyUp, mouseLButton, mouseMove, mouseWheel;
			//shared_ptr<UPDATE> update;
			UPDATE* update;
									
			//shared_ptr<Fem1d> fem;
			//shared_ptr<FEM::Mesh> fem_mesh;
			//shared_ptr<Fuelcell> fuelcell;
			shared_ptr<Graphics> gfx;
			shared_ptr<Gui> gui;
			//shared_ptr<GuiMenu> menu;
			shared_ptr<Timer> timer;			

			//bool vKeys[256], prevKeys[256];

		protected:
			inline void CalculateFps(const float dt);
			void InitializeGraphics();
			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			HWND hWnd;			
			//shared_ptr<Chart> chart;
			float fps, num_frames, time_elapsed;
			bool show_fps;
	};
}
