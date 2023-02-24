#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
class Window {
public:
	Window();
	~Window();

	bool init();
	void free();
	void handle_event(SDL_Event &event, SDL_Renderer* renderer);
	SDL_Renderer* create_renderer();

	int get_window_width();
	int get_window_heigth();

	bool has_mouse_focus();
	bool has_keyboard_focus();
	bool is_minimized();
private:
	SDL_Window* m_window;
	int m_window_heigth;
	int m_window_width;

	bool m_mouse_focus;
	bool m_keyboard_focus;
	bool m_is_fullscreen;
	bool m_is_minimized;
};