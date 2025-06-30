#pragma once
#include "Menu.h"
#include "GameState.h"

class MainMenu : public Menu {
private:
    int selectedMenuItem;
    
public:
    MainMenu(GameManager* gm);
    void Update() override;
    void Draw() override;
};