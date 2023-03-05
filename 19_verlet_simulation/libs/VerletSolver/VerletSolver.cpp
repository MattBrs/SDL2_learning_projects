#include "VerletSolver.hpp"

#include "Constants.hpp"
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
}