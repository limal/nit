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
#include "effect.h"
#include "graphics.h"

using namespace nit;

IEffect::IEffect(Graphics* gfx, wchar_t* filename) : effect(0)
{
	ID3DXBuffer* errors = 0;

	IDirect3DDevice9* device = gfx->GetDevice();

	D3DXCreateEffectFromFile(device, filename, 0, 0, D3DXSHADER_DEBUG, 0, &effect, &errors);

	// TODO
	//
	// Remove dependency to dxerr.lib from March 2009 DirectX SDK

	if (errors)
	{
		MessageBoxA(NULL, (char*)errors->GetBufferPointer(), 0, 0);
	}

	gfx->AddOnLostDevice(&MakeDelegate(this, &IEffect::OnLostDevice));
	gfx->AddOnResetDevice(&MakeDelegate(this, &IEffect::OnResetDevice));
}

IEffect::~IEffect()
{
	effect->Release();
}

void IEffect::Begin(unsigned int* num_passes)
{
	effect->Begin(num_passes, 0);
}

void IEffect::BeginPass(unsigned int pass)
{
	effect->BeginPass(pass);
}

void IEffect::CommitChanges()
{
	effect->CommitChanges();
}

void IEffect::End()
{
	effect->End();
}

void IEffect::EndPass()
{
	effect->EndPass();
}

void IEffect::GetParameterByName(char* name)
{
	params[name] = effect->GetParameterByName(0, name);
}

void IEffect::GetTechniqueByName(char* name)
{
	params[name] = effect->GetTechniqueByName(name);
}

void IEffect::OnLostDevice()
{
	effect->OnLostDevice();
}

void IEffect::OnResetDevice()
{
	effect->OnResetDevice();
}

void IEffect::SetFloat(char* name, const float f)
{
	effect->SetFloat(params[name], f);
}

void IEffect::SetMatrix(char* name, const D3DXMATRIX *matrix)
{
	effect->SetMatrix(params[name], matrix);
}

void IEffect::SetTechnique(char* name)
{
	effect->SetTechnique(params[name]);
}

void IEffect::SetTexture(char* name, IDirect3DTexture9* texture)
{
	effect->SetTexture(params[name], texture);
}

void IEffect::SetVector(char* name, const D3DXVECTOR4* vector)
{
	effect->SetVector(params[name], vector);
}
