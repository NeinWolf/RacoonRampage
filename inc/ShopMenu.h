#pragma once
#include "Menu.h"
#include "Weapon.h"
#include <vector>

struct ShopItem {
    std::string name;
    int cost;
    int damage;
    float speed;
};

class ShopMenu : public Menu {
private:
    int selectedItem;
    std::vector<ShopItem> items;

public:
    ShopMenu(GameManager* gm);
    void Update() override;
    void Draw() override;
};
