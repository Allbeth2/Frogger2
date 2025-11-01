#ifndef PLATFORM_H
#define PLATFORM_H

#include "Crosser.h"

class Platform : public Crosser {
public:
	using Crosser::Crosser;
	virtual ~Platform() = default;
	virtual Collision checkCollision(const SDL_FRect& otherRect) override;
};

#endif // PLATFORM_H