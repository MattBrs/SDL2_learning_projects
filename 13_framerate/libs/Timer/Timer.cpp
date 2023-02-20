#include "Timer.hpp"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_timer.h"

Timer::Timer() {
	m_started_ticks = 0;
	m_paused_ticks = 0;

	m_started = false;
	m_paused = false;
}

Timer::~Timer() {
	// do nothing
}

Uint32 Timer::get_ticks() {
	if (!m_started) {
		return 0;
	}

	if (m_paused) {
		return m_paused_ticks;
	}

	return SDL_GetTicks() - m_started_ticks;
}

bool Timer::is_paused() {
	return m_paused && m_started;
}

bool Timer::is_started() {
	return m_started;
}

void Timer::start() {
	m_started_ticks = SDL_GetTicks();
	m_paused_ticks = 0;
	m_started = true;
	m_paused = false;
}

void Timer::stop() {
	m_started = false;
	m_paused = false;
	m_started_ticks = 0;
	m_paused_ticks = 0;
}

void Timer::pause() {
	if (m_started && !m_paused) {
		m_paused_ticks = SDL_GetTicks() - m_started_ticks;
		m_started_ticks = 0;
		m_paused = true;	
	}
}

void Timer::unpause() {
	if (m_started && m_paused) {
		m_paused = false;
		m_started_ticks = SDL_GetTicks() - m_paused_ticks;
		m_paused_ticks = 0;
	}
}