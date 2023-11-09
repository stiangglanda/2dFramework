#pragma once

#include <SDL2/SDL.h>
#include "TSingleton.h"

#define g_pTimer Timer::Get();

class Timer : public TSingleton<Timer>
{
public:
	Timer();
	void Update();
	float GetElapsed() { return m_fElapsed; }

private:
	float m_fElapsed;
	float m_fCurTime;
	float m_fLastTime;
};
