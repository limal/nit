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

	class IEffect : public Effect
	{
		public:
			IEffect(Graphics* gfx, wchar_t* filename);
			~IEffect();

			void Begin(unsigned int* num_passes);
			void BeginPass(unsigned int pass);
			void CommitChanges();
			void End();
			void EndPass();
			void GetParameterByName(char* name);
			void GetTechniqueByName(char* name);
			void OnLostDevice();
			void OnResetDevice();
			void SetFloat(char* name, const float f);
			void SetMatrix(char* name, const D3DXMATRIX* matrix);
			void SetTechnique(char* name);
			void SetTexture(char* name, IDirect3DTexture9* texture);
			void SetVector(char* name, const D3DXVECTOR4* vector);

		protected:
			ID3DXEffect* effect;
			std::map<char*, D3DXHANDLE> params;
	};
}
