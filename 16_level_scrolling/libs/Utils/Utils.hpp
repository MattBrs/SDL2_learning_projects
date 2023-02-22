#pragma once

#include "SDL2/SDL_rect.h"
#include <vector>

namespace shapes {
	struct Circle {
		int pos_x;
		int pos_y;
		int radius;
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
	double distance_squared(int x1, int y1, int x2, int y2);
}

