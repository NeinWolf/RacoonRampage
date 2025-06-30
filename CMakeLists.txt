cmake_minimum_required(VERSION 3.15)
project(RaccoonRampage CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Include headers
include_directories(inc)
include_directories("C:/raylib/raylib/src")

if(WIN32)
    link_directories("C:/raylib/raylib/src")
endif()

set(SOURCES
    # Components
    src/Components/Ability.cpp
    src/Components/Animation.cpp
    src/Components/Projectile.cpp
    src/Components/Weapon.cpp
    
    # Entities
    src/Entities/Entity.cpp
    src/Entities/Player.cpp
    src/Entities/Enemy.cpp

    # Enemies (inside Entities)
    src/Entities/Enemies/Pizzabox.cpp
    src/Entities/Enemies/Slime.cpp
    src/Entities/Enemies/SodaCan.cpp
    
    # Game
    src/Game/GameManager.cpp
    
    # Systems
    src/Systems/AudioManager.cpp
    src/Systems/HUD.cpp
    src/Systems/SaveSystem.cpp
    src/Systems/WaveManager.cpp
    
    # UI
    src/UI/MainMenu.cpp
    src/UI/PauseMenu.cpp
    src/UI/SettingsMenu.cpp
    src/UI/ShopMenu.cpp
    
    # Main
    src/main.cpp
)
set(HEADERS
        inc/Ability.h 
        inc/Animation.h 
        inc/AudioManager.h 
        inc/Enemy.h 
        inc/EnemyType.h 
        inc/Entity.h 
        inc/GameManager.h 
        inc/GameState.h 
        inc/GameTransform.h 
        inc/HUD.h 
        inc/MainMenu.h 
        inc/Menu.h 
        inc/PauseMenu.h 
        inc/Pizzabox.h 
        inc/Player.h 
        inc/Projectile.h 
        inc/SaveSystem.h 
        inc/SettingsMenu.h 
        inc/Slime.h 
        inc/SodaCan.h 
        inc/Utils.h
        inc/WaveManager.h
        inc/Weapon.h
        inc/ShopMenu.h
)


add_executable(RaccoonRampage ${SOURCES} ${HEADERS})

target_compile_features(RaccoonRampage PRIVATE cxx_std_20)

# Link Raylib + platform deps
if(WIN32)
    target_link_libraries(RaccoonRampage PRIVATE raylib opengl32 gdi32 winmm)
elseif(UNIX)
    target_link_libraries(RaccoonRampage PRIVATE raylib m pthread dl GL X11 Xrandr Xinerama Xcursor Xxf86vm Xi)
endif()