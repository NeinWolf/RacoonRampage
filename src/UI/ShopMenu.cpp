#include "ShopMenu.h"
#include "SaveSystem.h"
#include "raylib.h"

ShopMenu::ShopMenu(GameManager* gm) : Menu(gm), selectedItem(0) {
    items.push_back({"Knife", 10, 15, 1.0f});
    items.push_back({"Bat", 20, 20, 0.8f});
    items.push_back({"Chain", 30, 25, 0.6f});
}

void ShopMenu::Update() {
    if (IsKeyPressed(KEY_UP)) {
        selectedItem = (selectedItem - 1 + (int)items.size() + 1) % ((int)items.size() + 1);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        selectedItem = (selectedItem + 1) % ((int)items.size() + 1);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (selectedItem == (int)items.size()) {
            gameManager->ResetGame();
            gameManager->SetGameState(GameState::MAIN_MENU);
            return;
        }

        Player* player = gameManager->GetPlayer();
        const ShopItem& item = items[selectedItem];
        if (player->GetScraps() >= item.cost) {
            player->AddScraps(-item.cost);
            player->SetWeapon(std::make_unique<Weapon>(item.name, item.damage, item.speed));
            SaveSystem::SaveScraps(player->GetScraps());
            SaveSystem::SaveWeapon(item.name);
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        gameManager->ResetGame();
        gameManager->SetGameState(GameState::MAIN_MENU);
    }
}

void ShopMenu::Draw() {
    DrawText("SHOP", 360, 80, 40, YELLOW);

    Player* player = gameManager->GetPlayer();
    DrawText(TextFormat("SCRAPS: %d", player->GetScraps()), 320, 140, 20, WHITE);

    for (size_t i = 0; i < items.size(); ++i) {
        Color color = (selectedItem == (int)i) ? GREEN : WHITE;
        DrawText(TextFormat("%s - %d scraps (DMG %d)", items[i].name.c_str(), items[i].cost, items[i].damage), 250, 200 + i * 40, 20, color);
    }

    Color exitColor = (selectedItem == (int)items.size()) ? GREEN : WHITE;
    DrawText("EXIT", 360, 200 + items.size() * 40, 20, exitColor);
}
