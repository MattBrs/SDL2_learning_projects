#pragma once

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "Types.hpp"
#include "VerletObject.hpp"
#include <vector>

using types::Vector2;

namespace verletSolver {
	class Solver {
	public:
		~Solver();

		void update(float delta_time);
		bool add_rect(
			const Vector2<double> &position, 
			const Vector2<double> &size
		);
		void render(SDL_Renderer* renderer);
		void handle_collitions();

	private:
		std::vector<verletObject::VerletObject*> m_objects;
		types::Vector2<double> m_gravity {0.0f, 1000.0f};

		void update_position(float delta_time);
		void apply_gravity();
		void apply_constraint();

		bool isclamped(float mid, float A, float B);
		bool checkcollisiononeway(SDL_Rect rectA, SDL_Rect rectB);
		bool checkcollisionbothways(SDL_Rect rectA, SDL_Rect rectB);

	};
}
