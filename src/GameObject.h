#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameState;

class GameObject
{
protected:
    GameState *gameState;
    GameObject(GameState *state);

public:
    virtual ~GameObject() = default;
    virtual void render() const = 0;
    virtual void update() = 0;
};

#endif // GAMEOBJECT_H
