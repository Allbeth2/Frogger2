#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameState;

class GameObject
{
protected:
    GameState *gameState;
    bool visible_ = true;
    GameObject(GameState *state);

public:
    virtual ~GameObject() = default;
    virtual void render() const = 0;
    virtual void update() = 0;

    void setVisible(bool visible) { visible_ = visible; }
    bool isVisible() const { return visible_; }
};

#endif // GAMEOBJECT_H
