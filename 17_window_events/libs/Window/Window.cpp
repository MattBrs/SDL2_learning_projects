#include "Window.hpp"
#include "Constants.hpp"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_video.h"
#include <cstdio>
#include <sstream>

Window::Window() {
	m_window = NULL;
	m_window_width = 0;
	m_window_heigth = 0;
	m_is_fullscreen = false;
	m_is_minimized = false;
	m_mouse_focus = false;
	m_keyboard_focus = false;
}

bool Window::init() {
	m_window = SDL_CreateWindow(
		"Resizable window test", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		constants::WINDOW_WIDTH, 
		constants::WINDOW_HEIGHT, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	if (m_window == NULL) {
		printf("Error while initializing window: %s\n", SDL_GetError());
		return false;
	}

	m_window_width = constants::WINDOW_WIDTH;
	m_window_heigth = constants::WINDOW_HEIGHT;
	m_keyboard_focus = true;
	m_mouse_focus = true; 

	return true;
}

SDL_Renderer* Window::create_renderer() {
	return SDL_CreateRenderer(
		m_window, 
		-1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
}

Window::~Window() {
	free();
}

void Window::free() {
	if (m_window != NULL) {
		SDL_DestroyWindow(m_window);
		m_window = NULL;
	}
}

void Window::handle_event(SDL_Event &event, SDL_Renderer* renderer) {
	if (event.type == SDL_WINDOWEVENT) {
		bool update_caption = false;


		switch (event.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_window_width = event.window.data1;
			m_window_heigth = event.window.data2;
			SDL_RenderPresent(renderer);
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			// window was covered by something and now it's not 
			SDL_RenderPresent(renderer);
			break;
		case SDL_WINDOWEVENT_ENTER:
			m_mouse_focus = true;
			update_caption = true;
			break;
		case SDL_WINDOWEVENT_LEAVE:
			m_mouse_focus = false;
			update_caption = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_keyboard_focus = true;
			update_caption = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_keyboard_focus = false;
			update_caption = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			m_is_minimized = false;
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			m_is_minimized = true;
			break;

		case SDL_WINDOWEVENT_RESTORED:
			m_is_minimized = false;
			break;
		}

		if (update_caption) {
			std::stringstream caption;
			caption << 
				"Window events - " 
				<< "MouseFocus: " << (m_mouse_focus ? "true":"false") 
				<< " KeyboardFocus: " << (m_keyboard_focus ? "true":"false");

			SDL_SetWindowTitle(m_window, caption.str().c_str());
		}

		return;
	}  

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
		if (m_is_fullscreen) {
			SDL_SetWindowFullscreen(m_window, SDL_TRUE);
			m_is_fullscreen = false;
		} else {
			SDL_SetWindowFullscreen(m_window, SDL_FALSE);
			m_is_fullscreen = true;
			m_is_minimized = false;
		}
	}
}

int Window::get_window_width() {
	return m_window_width;
}

int Window::get_window_heigth() {
	return m_window_heigth;
}

bool Window::has_keyboard_focus() {
	return m_keyboard_focus;
}

bool Window::has_mouse_focus() {
	return m_mouse_focus;
}

bool Window::is_minimized() {
	return m_is_minimized;
}
