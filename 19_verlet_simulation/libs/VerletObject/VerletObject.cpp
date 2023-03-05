#include "VerletObject.hpp"
#include "Types.hpp"
#include <cstdio>

using namespace verletObject;

VerletObject::~VerletObject() {
	// do nothing
}

void VerletObject::update_position(float delta_time) {
	const types::Vector2<double> velocity = 
		m_current_position - m_old_position;

	m_old_position = m_current_position;

	m_current_position = 
		m_current_position + velocity 
		+ m_acceleration * delta_time * delta_time;

	m_acceleration = {};
}

void VerletObject::accelerate(types::Vector2<double> acc) {
	m_acceleration += acc;
}

void VerletObject::render(SDL_Renderer *renderer) {
	// nothing to render because its not a shape but a base implementation
}
