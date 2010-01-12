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
#include "timer.h"

using namespace nit;

Timer::Timer() : previous(0)
{
	__int64 counts_per_second = 0;

	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_second);
	seconds_per_count = 1.0f / (float)counts_per_second;

	Reset();
}

Timer::~Timer()
{
}

float Timer::GetDelta()
{
	__int64 current = 0;
	
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	float dt = (current - previous) * seconds_per_count;
	previous = current;

	return dt;
}

void Timer::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&previous);
}