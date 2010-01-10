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

namespace nit
{
	class Graphics;
	class Font;
	//class RenderTexture;
	class Textures;

	typedef FastDelegate2<Graphics*, float> RenderQueueDelegate;

	class RenderQueue
	{		
		struct RenderFunc
		{
			RenderFunc(RenderQueueDelegate delegate_, unsigned int priority_) : delegate(delegate_), priority(priority_) { }

			RenderQueueDelegate delegate;
			unsigned int priority;
		};

		struct ltrender
		{
			bool operator()(const RenderFunc r1, const RenderFunc r2) const
			{
				return r1.priority > r2.priority;
			}
		};
		
		typedef std::set<RenderFunc, ltrender> Container;
		Container list;

	public:
		void Add(RenderQueueDelegate dg, unsigned int priority) { list.insert(RenderFunc(dg, priority)); }

		void operator +=(RenderQueueDelegate dg) { list.insert(RenderFunc(dg, 100)); }
		void operator -=(RenderQueueDelegate dg) { list.erase(RenderFunc(dg, 100)); }
		void operator ()(Graphics* gfx, float t)
		{
			for (Container::iterator it = list.begin(); it != list.end(); ++it)
			{
				(*it).delegate(gfx, t);
			}
		}
	};

	typedef FastDelegate0<> LostQueueDelegate;

	class LostQueue
	{
		typedef std::set<LostQueueDelegate> Container;

		Container list;

	public:
		void operator +=(LostQueueDelegate dg) { list.insert(dg); }
		void operator -=(LostQueueDelegate dg) { list.erase(dg); }
		void operator ()()
		{
			for (Container::iterator it = list.begin(); it != list.end(); ++it)
			{
				(*it)();
			}
		}
	};

	class Graphics : public nitGraphics
	{
		public:
			Graphics(bool windowed);
			~Graphics();

			void AddOnLostDevice(void* delegate);
			void AddOnRender(void* delegate, unsigned int priority);
			void AddOnResetDevice(void* delegate);
			void BeginRTT(std::string texname);
			void EndRTT();
			IDirect3DDevice9* GetDevice();			
			inline unsigned int GetHeight() { return d3dpp.BackBufferHeight; }
			void GetSize(unsigned int* width, unsigned int* height);
			unsigned int GetTextWidth(char* text);
			inline unsigned int GetWidth() { return d3dpp.BackBufferWidth; }
			//IDirect3DTexture9* GetTexture(std::string name);
			IDirect3DTexture9* GetTexture(unsigned int id);
			HRESULT Initialize(unsigned int width, unsigned int height, HWND hWnd);
			bool IsDeviceLost();
			//void LoadTexture(std::string name, std::wstring filename);
			void LoadTexture(unsigned int id, wchar_t* filename);
			void Print(float t);
			void Render(float dt);
			void Reset();
			void Release();
			void Resize(unsigned int width, unsigned int height);
			void SetD3DVERTEXDeclaration();
			void SetD3DVERTEXCOLORDeclaration();
			void Write(const char* text_, float x, float y, float z = 0);
			//D3DXMATRIX proj, view;
			//shared_ptr<RenderTexture> render_texture;
			

		protected:
			HRESULT CreateD3D9(unsigned int width, unsigned int height, HWND hWnd);
			static void InitializeVertexDeclarations(Graphics* gfx);
			bool SetMultisampling(const D3DMULTISAMPLE_TYPE multisample_type, const D3DFORMAT buffer_format, const D3DFORMAT depth_format, DWORD& quality);

			bool windowed;
			RenderQueue render;
			LostQueue on_lost_device, on_reset_device;			

			//shared_ptr<IDirect3D9> d3d;
			IDirect3D9* d3d;
			IDirect3DDevice9* device;
			D3DPRESENT_PARAMETERS d3dpp;

			shared_ptr<Font> font;
			shared_ptr<Textures> textures;
			//shared_ptr<IDirect3DDevice9> device;
	};
}
