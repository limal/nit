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

struct nitVERTEX;

namespace nit
{	
	class IEffect;
	class FontLoader;
	class Graphics;
	class Mesh;

	struct SCharDescr
	{
		SCharDescr();			

		short srcX;
		short srcY;
		short srcW;
		short srcH;
		short xOff;
		short yOff;
		short xAdv;
		short page;
		unsigned int chnl;

		std::vector<int> kerningPairs;
	};

	struct CHARDESC
	{
		float width, height, x, y, x_adv, x_off, y_off;
	};

	class Font
	{
		public:
			Font(Graphics* gfx, char* filename, wchar_t* texture_filename);
			~Font();
			
			unsigned int GetWidth(const char* text);
			void Print(float t);
			void Write(const char* text_, float x, float y, float z = 0);

		protected:
			Graphics* gfx;
			shared_ptr<IEffect> effect;
			shared_ptr<Mesh> mesh;

			nitVERTEX* v;
			WORD* in;
			nitVERTEX* c_v; // current vertex
			WORD* c_in;
			unsigned int c_num_ver;
			unsigned int c_num_ind;
			unsigned int num_ver;
			unsigned int num_ind;

			D3DXVECTOR3* colors;

			enum
			{
				MAX_CHARS = 16384
			};

			friend class FontLoader;

			SCharDescr* GetChar(int id);

			short fontHeight; // total height of the font
			short base;     // y of base line
			float scaleW;
			float scaleH;
			SCharDescr defChar;

			CHARDESC chars[256];
			float update_time;
	};
}
