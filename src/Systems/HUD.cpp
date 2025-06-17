#include "HUD.h"
#include "raylib.h"

void HUD::Draw(Player* player, int score, int wave) {
    // Health hearts
    for (int i = 0; i < player->GetMaxHealth() / 10; i++) {
        Color heartColor = (i < player->GetHealth() / 10) ? RED : GRAY;
        DrawText("<3", 10 + i * 25, 10, 20, heartColor);
    }
    
    // Stamina dashes
    for (int i = 0; i < player->GetMaxStamina() / 5; i++) {
        Color dashColor = (i < player->GetStamina() / 5) ? BLUE : GRAY;
        DrawText("_/ ", 10 + i * 15, 35, 20, dashColor);
    }
    
    // Score
    DrawText(TextFormat("SCORE: %04d", score), 350, 10, 20, WHITE);
    
    // Scraps
    DrawText(TextFormat("SCRAPS: %d", player->GetScraps()), 650, 10, 20, YELLOW);
    
    // Wave
    DrawText(TextFormat("WAVE: %d", wave), 10, 550, 20, WHITE);
    
    // Abilities (bottom-left)
    const auto& abilities = player->GetAbilities();
    for (size_t i = 0; i < abilities.size(); i++) {
        Rectangle abilityRect = {10.0f + static_cast<float>(i) * 60.0f, 500.0f, 50.0f, 50.0f};
        Color abilityColor = abilities[i]->CanUse() ? GREEN : GRAY;
        DrawRectangleRec(abilityRect, abilityColor);
        DrawRectangleLinesEx(abilityRect, 2, BLACK);
        DrawText(abilities[i]->GetName().substr(0, 1).c_str(), 
                abilityRect.x + 20, abilityRect.y + 20, 20, WHITE);
    }
    
    // Weapon (bottom-right)
    Rectangle weaponRect = {730, 500, 50, 50};
    DrawRectangleRec(weaponRect, BROWN);
    DrawRectangleLinesEx(weaponRect, 2, BLACK);
    DrawText(player->GetWeapon()->GetName().substr(0, 1).c_str(), 
            weaponRect.x + 20, weaponRect.y + 20, 20, WHITE);
}