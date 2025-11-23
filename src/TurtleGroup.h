#ifndef TURTLEGROUP_H
#define TURTLEGROUP_H

#include <istream>

#include "Platform.h"

class PlayState;

class TurtleGroup : public Platform
{
    int turtleCount; //el numero de tortugas
    bool sinking;  //bool para saber si se submergen
    int currentFrame = 0; //frameactual
    float animationTimer = 0.0f; //timer de animacion

    static constexpr float ANIMATION_RATE = 0.5f; // Tiempo en segundos por frame
    static constexpr int TOTAL_FRAMES = 7; // numero de frames en la animacion de sinking

public:
    TurtleGroup(PlayState* state, Texture* texture, Point2D<float> pos, Vector2D<float> vel, int turtleCount, bool sinking, int currentFrame);
	TurtleGroup(PlayState* state, std::fstream& file, int lineNumber);
    virtual ~TurtleGroup() = default;
    
    void update() override;
    void render() const override;
    Collision checkCollision(const SDL_FRect& otherRect) override;
};

#endif // TURTLEGROUP_H
