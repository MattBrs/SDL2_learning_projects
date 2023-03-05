#pragma once

#include "SDL2/SDL_render.h"
#include "Types.hpp"
#include "VerletObject.hpp"

namespace verletCircle {
	class VerletCircle: verletObject::VerletObject {
	public:
		void render(SDL_Renderer* renderer);
	};
}
