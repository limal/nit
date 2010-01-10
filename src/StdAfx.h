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

#ifndef H_W_STDAFX
#define H_W_STDAFX

#ifndef _DEBUG
#undef _SECURE_SCL
#define _SECURE_SCL 0
#undef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING 0
#endif

#include "FastDelegate.h"
#include <windows.h>
#include <windowsx.h> // TODO: remove GET_X_LPARAM
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include "nit.h"

using namespace fastdelegate; // TODO: remove all using namespace
using namespace std; // TODO: remove

#define ReleaseCOM(x) {if(x){x->Release();x=NULL;}}

#endif // H_W_STDAFX
