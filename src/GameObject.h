#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class Game;

class GameObject
{
protected:
    Game *gamePointer;
    GameObject(Game *game);

public:
    virtual ~GameObject() = default;
    virtual void render() const = 0;
    virtual void update() = 0;
};

#endif // GAMEOBJECT_H