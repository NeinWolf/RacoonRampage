#pragma once
#include "../Game/GameManager.h"

class GameManager;

class Menu {
protected:
    GameManager* gameManager;
    
public:
    Menu(GameManager* gm) : gameManager(gm) {}
    virtual ~Menu() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};