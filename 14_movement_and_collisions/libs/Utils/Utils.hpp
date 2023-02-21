#pragma once

#include "SDL2/SDL_rect.h"
#include <vector>

namespace collisions {
	bool check_collision(
		std::vector<SDL_Rect> &box_colliders_1,
		std::vector<SDL_Rect> &box_colliders_2
	);
}