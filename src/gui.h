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

struct nitVERTEXSCREEN;
struct nitUPDATE;

namespace nit
{	
	class IEffect;
	class Graphics;
	class Mesh;

	class IGui : public ::Gui
	{
		public:
			IGui(nitUPDATE& update, Graphics* gfx);
			~IGui();

			void AddBitmap(unsigned int id, nitGUIBITMAP bitmap);
			void Command(unsigned int command_id);
			void Draw(const nitVERTEXSCREEN* vsrc, const unsigned int num_vsrc, const WORD* isrc, const unsigned int num_isrc);
			unsigned int GetTextWidth(char* text);
			void Text(const char* text, float x, float y);
			unsigned int GetHeight();
			unsigned int GetWidth();
			unsigned int GetActive() { return active; };
			unsigned int GetHot() { return hot; }
			int GetMouseX();
			int GetMouseY();
			unsigned int GetTextureSize() { return tex_size; }
			void Render(Graphics* gfx, float t);
			void RenderBitmap(unsigned int key, const float x, const float y, const float z);
			void SetActive(unsigned int id) { active = id; }
			void SetHot(unsigned int id) { hot = id; }
			void SetPosition(const float x, const float y);

			unsigned int active, hot;
			bool mouse_up, mouse_down;

		protected:
			void Initialize();
			void LoadBitmaps();

			nitUPDATE& update;
			Graphics* gfx;
			shared_ptr<IEffect> effect;
			unsigned int tex_size;

			nitVERTEXSCREEN* vertices, * v;
			WORD* indices, * k;
			unsigned int num_ver;
			unsigned int num_ind;

			D3DXVECTOR2 pos;

			std::map<unsigned int, nitGUIBITMAP> bitmaps;

			enum
			{
				MAX_VERTICES = 16384
			};
	};
}
