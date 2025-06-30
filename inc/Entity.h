#pragma once
#include "GameTransform.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Entity {
protected:
    GameTransform transform;
    Animation* currentAnimation;
    std::unordered_map<std::string, Animation*> animations;
    bool isActive;

public:
    Entity();
    virtual ~Entity();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const = 0;

    void SetPosition(Vector2 position);
    Vector2 GetPosition() const;
    bool IsActive() const;
    void SetActive(bool active);
};