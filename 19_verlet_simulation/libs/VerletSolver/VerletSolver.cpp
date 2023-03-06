#include "VerletSolver.hpp"

#include "Constants.hpp"
#include "SDL2/SDL_rect.h"
#include "Types.hpp"
#include "Utils.hpp"
#include "VerletObject.hpp"
#include "VerletRect.hpp"
#include <cstdio>

using namespace verletSolver;

Solver::~Solver() {
	for (int i = 0; i < m_objects.size(); ++i) {
		delete m_objects[i];
	}
}

void Solver::apply_gravity() {
	for (auto &object: m_objects) {
		object->accelerate(m_gravity);
	}
}

void Solver::update(float delta_time) {
	const int sub_steps = 1;
	const float sub_delta_time = delta_time / sub_steps;

	for (int i = 0; i < sub_steps; ++i) {
		apply_gravity();
		apply_constraint();
		handle_collitions();
		update_position(sub_delta_time);
	}
}

void Solver::update_position(float delta_time) {
	for (auto &object: m_objects) {
		object->update_position(delta_time);
	}
}

bool Solver::add_rect(
	const Vector2<double> &position, const Vector2<double> &size) {
	verletRect::VerletRect* rect = new verletRect::VerletRect(position, size);

	m_objects.push_back(rect);

	return true;
}

void Solver::render(SDL_Renderer* renderer) {
	for (auto &object: m_objects) {
		object->render(renderer);
	}
}

void Solver::apply_constraint() {
	for (auto &object: m_objects) {
		if (
			object->m_current_position.y 
			> constants::WINDOW_HEIGHT - object->m_size.y
		) {

			object->m_current_position.y = 
				constants::WINDOW_HEIGHT - object->m_size.y;
		} else if (object->m_current_position.y < 0) {
			object->m_current_position.y = 0;
		}

		if (
			object->m_current_position.x
			> constants::WINDOW_WIDTH - object->m_size.x
		) {

			object->m_current_position.x = 
				constants::WINDOW_WIDTH - object->m_size.x;
		} else if (object->m_current_position.x < 0) {
			object->m_current_position.x = 0;
		}		
	}
}

void Solver::handle_collitions() {
	/*
	const float response_coeff = 0.75f;

	for (int i = 0; i < m_objects.size(); ++i) {
		verletObject::VerletObject* obj_1 = m_objects[i];

		for (int j = i + 1; j < m_objects.size(); ++j) {
			verletObject::VerletObject* obj_2 = m_objects[j];

			Vector2<double> v = 
				obj_1->m_current_position - obj_2->m_current_position;
			const double distance = v.x * v.x + v.y * v.y;
			const double min_distance = 
				obj_1->m_collider.radius + obj_2->m_collider.radius;

			if (distance < min_distance * min_distance) {
				const double new_distance = sqrt(distance);
				Vector2<double> n = v / new_distance;

				const double mass_ratio_1 = 
					obj_1->m_collider.radius / 
					(obj_1->m_collider.radius + obj_2->m_collider.radius);

				const double mass_ratio_2 = 
					obj_2->m_collider.radius / 
					(obj_2->m_collider.radius + obj_1->m_collider.radius);

				const double delta = 
					response_coeff * 0.5f * (new_distance - min_distance);

				obj_1->m_current_position -= n * (mass_ratio_2 + delta);
				obj_2->m_current_position += n * (mass_ratio_1 + delta);


			}
		}
	}
	*/

	for (int i = 0; i < m_objects.size(); ++i) { 
		verletObject::VerletObject* obj_1 = m_objects[i];

		for (int j = i + 1; j < m_objects.size(); ++j) {
			verletObject::VerletObject* obj_2 = m_objects[j];
			if (check_collision_both_ways(
				obj_1->m_box_collider, obj_2->m_box_collider)
			) {
				SDL_Rect box_1 = obj_1->m_box_collider;
				SDL_Rect box_2 = obj_2->m_box_collider;

				/*
				double collision_radians_1 = 
					atan2(box_1.y - box_2.y, box_1.x - box_2.x);
				double collision_degrees_1 = 
					collision_radians_1 * (180.0/3.141592653589793238463);
				printf("collision degrees box 1:  %f\n", collision_degrees_1);

				double collision_radians_2 = 
					atan2(box_2.y - box_1.y, box_2.x - box_1.x);
				double collision_degrees_2 = 
					collision_radians_2 * (180.0/3.141592653589793238463);
				printf("collision degrees box 2:  %f\n\n", collision_degrees_2);

				Vector2<double> obj_velocity_1 = 
					obj_1->m_current_position - obj_1->m_old_position;
				Vector2<double> obj_velocity_2 = 
					obj_2->m_current_position - obj_2->m_old_position;

				
				obj_1->m_current_position += obj_velocity_1 * 0.3f;
				obj_1->m_collider.pos_x = obj_1->m_current_position.x;
				obj_1->m_collider.pos_y = obj_1->m_current_position.y;

				obj_2->m_current_position += obj_velocity_2 * 0.1f;
				obj_2->m_collider.pos_x = obj_2->m_current_position.x;
				obj_2->m_collider.pos_y = obj_2->m_current_position.y;
				*/

				float difference_x = 
					((float)(box_1.x + box_1.w) / 2) - ((float)(box_2.x + box_2.w) / 2); 
				float difference_y =
						((float)(box_1.y + box_1.h) / 2) 
						- 
						((float)(box_2.y + box_2.h) / 2);

				if (
					abs(difference_x / box_2.w) > abs(difference_y / box_2.h)
				) {
					if (difference_x < 0) {
						float new_pos = box_2.x - box_1.w;
						float vel = 
							(new_pos - obj_1->m_current_position.x) / 2;
						obj_1->m_current_position.x += vel;
						obj_2->m_current_position.x -= vel;
					} else {
						float new_pos = box_2.x + box_2.x;
						float vel = 
							(new_pos - obj_1->m_current_position.x) / 2;
						obj_1->m_current_position.x += vel;
						obj_2->m_current_position.x -= vel;
					}
				} else {
					if (difference_y < 0) {
						float new_pos = box_2.y - box_1.h;
						float vel = (new_pos - obj_1->m_current_position.y) / 2;
						obj_1->m_current_position.y += vel;
						obj_2->m_current_position.y -= vel;
					} else {
						float new_pos = box_2.y + box_2.h;
						float vel = (new_pos - obj_1->m_current_position.y) / 2;
						obj_1->m_current_position.y += vel;
						obj_2->m_current_position.y -= vel;
					}
				}

				obj_1->m_collider.pos_x = obj_1->m_current_position.x;
				obj_1->m_collider.pos_y = obj_1->m_current_position.y;
				obj_2->m_collider.pos_x = obj_2->m_current_position.x;
				obj_2->m_collider.pos_y = obj_2->m_current_position.y;
			}
		}
	}
}


bool Solver::is_clamped(float mid, float a, float b)
{
    if (a > b)
    {
        return mid >= b && mid <= a;
    }
    return mid >= a && mid <= b;
}

bool Solver::check_collision_one_way(SDL_Rect rect_a, SDL_Rect rect_b) {
	float left_a, right_a, top_a, bottom_a;
	float left_b, right_b, top_b, bottom_b;

	left_a = rect_a.x;
	right_a = rect_a.x + rect_a.w;
	top_a = rect_a.y;
	bottom_a = rect_a.y + rect_a.h;

	left_b = rect_b.x;
	right_b = rect_b.x + rect_b.w;
	top_b = rect_b.y;
	bottom_b = rect_b.y + rect_b.h;

    if (
    	(is_clamped(left_a, left_b, right_b) 
		|| is_clamped(right_a, left_b, right_b)) 
		&& 
		(is_clamped(bottom_a, bottom_b, top_b) 
		|| is_clamped(top_a, bottom_b, top_b))
	) {
        return true;
    }
    return false;
}

bool Solver::check_collision_both_ways(SDL_Rect rectA, SDL_Rect rectB) {
    return check_collision_one_way(rectA, rectB) || check_collision_one_way(rectB, rectA);
}