#include "Utils.hpp"

bool collisions::check_collision(
	std::vector<SDL_Rect> &box_colliders_1, 
	std::vector<SDL_Rect> &box_colliders_2
) {
	// variables to represent each side of the box colliders
	int left_1, left_2;
	int right_1, right_2;
	int top_1, top_2;
	int bottom_1, bottom_2;

	// analyze first box collider
	for (const auto &box_1: box_colliders_1) {
		left_1 = box_1.x;
		right_1 = box_1.x + box_1.w;
		top_1 = box_1.y;
		bottom_1 = box_1.y + box_1.h;

		// analyze second box collider
		for (const auto &box_2: box_colliders_2) {
			left_2 = box_2.x;
			right_2 = box_2.x + box_2.w;
			top_2 = box_2.y;
			bottom_2 = box_2.y + box_2.h;

			// check if two boxes are colliding
			if (
				(bottom_1 <= top_2
					|| top_1 >= bottom_2
					|| left_1 >= right_2
					|| right_1 <= left_2) == false
			) {
				return true;
			}
		}
	}
	
	return false;
}