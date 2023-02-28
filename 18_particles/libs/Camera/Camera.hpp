#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
namespace camera {
	class Camera {
	public:
		const int MAX_VEL = 600;

		Camera(int pos_x, int pos_y, int width, int height);
		~Camera();

		void handle_event(SDL_Event &event);
		void move(float delta_time);

		int get_pos_x();
		int get_pos_y();
		int get_width();
		int get_heigth();
		SDL_Rect& get_camera_rect();
	private:
		SDL_Rect m_camera;
		int m_vel_x;
		int m_vel_y;

		void handle_movement_input(const SDL_Keycode &keycode, int vel);
	};
}