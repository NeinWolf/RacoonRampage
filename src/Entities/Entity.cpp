#include "Entity.h"

Entity::Entity() : isActive(true) {}

Entity::~Entity() {
    for (auto& anim : animations) {
        delete anim.second;
    }
}

void Entity::SetPosition(Vector2 position) {
    transform.position = position;
}

Vector2 Entity::GetPosition() const {
    return transform.position;
}

bool Entity::IsActive() const {
    return isActive;
}

void Entity::SetActive(bool active) {
    isActive = active;
}