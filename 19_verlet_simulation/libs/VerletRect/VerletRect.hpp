#pragma once

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "Types.hpp"
#include "VerletObject.hpp"

using types::Vector2;

namespace verletRect {
	class VerletRect: public verletObject::VerletObject {
	public:
		VerletRect(
			const Vector2<double> &position, 
			const Vector2<double> &size
		);
		~VerletRect();

		void render(SDL_Renderer* renderer) override;

	private:
		SDL_Rect m_rect;
	};
}
