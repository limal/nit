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

struct D3DVERTEX;

namespace nit
{
	class IModel : public Model
	{
		public:
			IModel();
			~IModel();

			void AddTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
			void Draw(IDirect3DDevice9* device);
			void Load(wchar_t* filename);


		protected:
			std::vector<D3DVERTEX> vertices;
			std::vector<WORD> indices;
	};
}
