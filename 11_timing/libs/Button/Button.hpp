#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "Texture.hpp"

enum ButtonSprite {
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

class Button {
	public:
		Button();
		Button(SDL_Rect texture_clips[]);
		Button(int x, int y,SDL_Rect texture_clips[]);
		~Button();

		void free();
		void render(SDL_Renderer* renderer, texture::Texture &button_texture);
		void handle_event(SDL_Event* event);

		void set_position(int x, int y);
		void set_texture_clips(SDL_Rect texture_clips[]);
	private:
		SDL_Point m_position;
		ButtonSprite m_current_sprite;
		SDL_Rect* m_texture_clips;
};
