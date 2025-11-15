#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H

#include <istream>

#include "Platform.h"

class TurtleGroup : public Platform
{
    int turtleCount;
    bool sinking;
    int currentFrame = 0;
    float animationTimer = 0.0f;

    static constexpr float ANIMATION_RATE = 0.5f; // Tiempo en seonds por frame
    static constexpr int TOTAL_FRAMES = 7; // numero de frames en la animacion de siking

public:
    TurtleGroup(Game* game, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool sinking, int currentFrame);
    
    virtual ~TurtleGroup() = default;
    
    void update() override;
    void render() const override;
    Collision checkCollision(const SDL_FRect& otherRect);
};

#endif // TURTLEGROUP_H
