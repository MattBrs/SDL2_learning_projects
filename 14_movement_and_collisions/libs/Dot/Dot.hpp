#pragma  once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"

class Dot {
private:
	Dot();
	~Dot();

	void handle_event(SDL_Event &event);
	void move();
	void render(SDL_Renderer* renderer);
public:
	const int DOT_WIDTH = 20;
	const int DOT_HEIGHT = 20;
	const int DOT_MAX_VEL = 10;
	const int LOCAL_WINDOW_WIDTH = 960;
	const int LOCAL_WINDOW_HEIGHT = 720;

	int m_pos_x;
	int m_pos_y;

	int m_vel_x;
	int m_vel_y;

	void handle_movement_input(const SDL_Keycode &keycode, const int &vel);
};