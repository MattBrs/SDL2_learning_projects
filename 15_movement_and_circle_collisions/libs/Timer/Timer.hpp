#pragma once

#include "SDL2/SDL_stdinc.h"

class Timer {
public:
	Timer();
	~Timer();
	
	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 get_ticks();

	bool is_started();
	bool is_paused();
private:
	Uint32 m_started_ticks = 0;
	Uint32 m_paused_ticks = 0;

	bool m_started = false;
	bool m_paused = false;
};