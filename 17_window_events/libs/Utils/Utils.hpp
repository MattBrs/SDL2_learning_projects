#pragma once

#include "SDL2/SDL_rect.h"
#include <vector>

namespace shapes {
	struct Circle {
		float pos_x;
		float pos_y;
		float radius;
	};
}

namespace collisions {
	bool check_collision(
		std::vector<SDL_Rect> &box_colliders_1,
		std::vector<SDL_Rect> &box_colliders_2
	);
	bool check_collision(
		shapes::Circle &circle_collider, 
		shapes::Circle &other_circle
	);
	bool check_collision(
		shapes::Circle &circle_collider,
		SDL_Rect &rect_collider
	);
	double distance_squared(float x1, float y1, float x2, float y2);
}

