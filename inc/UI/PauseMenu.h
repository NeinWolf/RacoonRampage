#pragma once
#include "Menu.h"

class PauseMenu : public Menu {
private:
    int selectedMenuItem;
    
public:
    PauseMenu(GameManager* gm);
    void Update() override;
    void Draw() override;
};