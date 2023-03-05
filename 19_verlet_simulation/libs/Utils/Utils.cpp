#include "Utils.hpp"
#include <cstdio>

bool collisions::check_collision(
	std::vector<SDL_Rect> &box_colliders_1, 
	std::vector<SDL_Rect> &box_colliders_2
) {
	// variables to represent each side of the box colliders
	float left_1, left_2;
	float right_1, right_2;
	float top_1, top_2;
	float bottom_1, bottom_2;

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

bool collisions::check_collision(
		SDL_Rect &box_1,
		SDL_Rect &box_2
) {
	float left_1, left_2;
	float right_1, right_2;
	float top_1, top_2;
	float bottom_1, bottom_2;

	left_1 = box_1.x;
	right_1 = box_1.x + box_1.w;
	top_1 = box_1.y;
	bottom_1 = box_1.y + box_1.h;

	left_2 = box_2.x;
	right_2 = box_2.x + box_2.w;
	top_2 = box_2.y;
	bottom_2 = box_2.y + box_2.h;

	if (
		(
			bottom_1 <= top_2
			|| top_1 >= bottom_2
			|| left_1 >= right_2
			|| right_1 <= left_2
		) == false
	) {
		return true;
	}

	return false;
}

bool collisions::check_collision(
		shapes::Circle &circle_collider, 
		shapes::Circle &other_circle
) {
	float total_radius_squared = circle_collider.radius + other_circle.radius;
	total_radius_squared *= total_radius_squared;

	if (distance_squared(
		circle_collider.pos_x, 
		circle_collider.pos_y, 
		other_circle.pos_x, 
		other_circle.pos_y
		) < total_radius_squared
	) {
		return true;
	}

	return false;;
}

bool collisions::check_collision(
		shapes::Circle &circle_collider,
		SDL_Rect &rect_collider
) {
	float closest_x;
	float closest_y;

	if (circle_collider.pos_x < rect_collider.x) {
		closest_x = rect_collider.x;
	} else if (circle_collider.pos_x > rect_collider.x + rect_collider.w) {
		closest_x = rect_collider.x + rect_collider.w;
	} else {
		closest_x = circle_collider.pos_x;
	}

	if (circle_collider.pos_y < rect_collider.y) {
		closest_y = rect_collider.y;
	} else if (circle_collider.pos_y > rect_collider.y + rect_collider.h) {
		closest_y = rect_collider.y + rect_collider.h;
	} else {
		closest_y = circle_collider.pos_y;
	}

	if (distance_squared(
		circle_collider.pos_x, 
		circle_collider.pos_y, 
		closest_x, 
		closest_y) < circle_collider.radius * circle_collider.radius
	) {
		return true;
	}

	return false;
}

double collisions::distance_squared(float x1, float y1, float x2, float y2) {
	float delta_x = x2 - x1;
	float delta_y = y2 - y1;

	return (delta_x * delta_x) + (delta_y * delta_y);
}
