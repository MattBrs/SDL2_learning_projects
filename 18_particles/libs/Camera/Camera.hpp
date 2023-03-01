#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
namespace camera {
	class Camera {
	public:
		const int MAX_VEL = 600;
		// camera moves if player enters the padding zone 
		// (it surrounds the window)
		const int DEFAULT_CAMERA_PADDING = 100;  

		Camera(int pos_x, int pos_y, int width, int height);
		~Camera();

		void handle_event(SDL_Event &event);
		void move(float delta_time);
		
		void move_with_player_pos(const int &player_x, const int &player_y);
		void set_camera_x(int pos_x);
		void set_camera_y(int pos_y);
		void set_camera_automovement(bool automovement_enabled);

		int get_pos_x();
		int get_pos_y();
		int get_width();
		int get_heigth();
		SDL_Rect& get_camera_rect();
	private:

		SDL_Rect m_camera;
		int m_vel_x = 0;
		int m_vel_y = 0;
		int m_camera_padding = DEFAULT_CAMERA_PADDING;
		bool m_camera_automovement = false;

		void handle_movement_input(const SDL_Keycode &keycode, int vel);
	};
}