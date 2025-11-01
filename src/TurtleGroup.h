#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H

#include "Platform.h"
#include <SDL3/SDL.h>

/**
 * Represents a group of turtles that swim upstream.
 */
class TurtleGroup : public Platform
{
private:
    int numTurtles;
    bool canDive;
    bool isSubmerged;
    int nextStateChange;
    int diveInterval;
    
public:
    TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int numTurtles, bool canDive);
    virtual ~TurtleGroup() = default;
    virtual void update() override;
    virtual Collision checkCollision(const SDL_FRect& otherRect) override;
    virtual void render() const override;
private:
    void updateDivingState();
};

#endif // TURTLEGROUP_H