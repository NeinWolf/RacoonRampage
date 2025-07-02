#pragma once
#include "Menu.h"
#include "GameState.h"

class CreditsMenu : public Menu {
public:
    CreditsMenu(GameManager* gm);
    void Update() override;
    void Draw() override;
};
