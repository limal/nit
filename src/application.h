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

struct nitUPDATE;

namespace nit
{
	class Graphics;
	class IGui;
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
			IApplication(const unsigned int width, const unsigned int height, const bool antialiasing, const bool windowed, const bool show_fps);
			~IApplication();

			void AddOnCreated(void* delegate);
			void AddRenderFunction(void* delegate, unsigned int priority);
			void AddUpdatingFunction(void* delegate);
			void CenterWindow(HWND hWnd, int _x = 0, int _y = 0);
			nitUPDATE* Create(wchar_t* title, int nShowCmd);
			bool CreateMainWindow(wchar_t* title, int cmdShow);			
			void DisplayError(wstring message);
			nitGraphics* GetGraphics() { return (nitGraphics*)gfx.get(); }
			IGui& GetGui();
			nitUPDATE* GetUPDATE() { return update; }
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

			bool antialiasing, windowed, minimized;
			short dmx, dmy, wheel_delta;
			unsigned int mstate; // mouse button state
			EventQueue created, input, keyChar, keyDown, keyUp, mouseLButton, mouseMove, mouseWheel, updating;
			nitUPDATE* update;
									
			shared_ptr<Graphics> gfx;
			shared_ptr<IGui> gui;
			shared_ptr<Timer> timer;			

		protected:
			inline void CalculateFps(const float dt);
			void InitializeGraphics();
			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			HWND hWnd;			
			float fps, num_frames, time_elapsed;
			bool show_fps;
	};
}
