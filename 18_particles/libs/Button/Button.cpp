#include "Button.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_rect.h"
#include "Texture.hpp"
#include <cstdio>

Button::Button(int x, int y, SDL_Rect texture_clips[]) {
	m_position.x = x;
	m_position.y = y;

	m_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
	m_texture_clips = texture_clips;
}

Button::Button(SDL_Rect texture_clips[]) : Button::Button(0, 0, texture_clips) {}
Button::Button(): Button::Button(NULL) {}

void Button::set_position(int x, int y) {
	m_position.x = x;
	m_position.y = y;
}

Button::~Button() {
	free();
}

void Button::free() {
	m_texture_clips = NULL;
}

void Button::handle_event(SDL_Event *event) {
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		bool inside = true;
		SDL_GetMouseState(&x, &y);

		// doing this to check mouse event position
		// and change sprite accordingly
		if (x < m_position.x) {
			inside = false;
		} else if (x > m_position.x + BUTTON_WIDTH) {
			inside = false;
		} else if (y < m_position.y) {
			inside = false;
		} else if (y > m_position.y + BUTTON_HEIGHT) {
			inside = false;
		}

		if (!inside) {
			m_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
			return;
		}

		// if we are inside a button
		// we check what action has been performed
		switch (event->type) {
			case SDL_MOUSEMOTION: 
				m_current_sprite = BUTTON_SPRITE_MOUSE_OVER; 
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP: 
				m_current_sprite = BUTTON_SPRITE_MOUSE_UP;
				break;
		}
	}
}

void Button::render(SDL_Renderer *renderer, texture::Texture &button_texture) {
	// render button texture with selected clipping
	button_texture.render(m_position.x, m_position.y, renderer, &m_texture_clips[m_current_sprite]);
}

void Button::set_texture_clips(SDL_Rect texture_clips[]) {
	m_texture_clips = texture_clips;
}

