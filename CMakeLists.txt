cmake_minimum_required(VERSION 3.15)
project(RaccoonRampage CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Include headers
include_directories(inc)
include_directories("C:/raylib/raylib/src")
include_directories("C:/raylib/raylib/src/external")

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
    
    # Main
    src/main.cpp
)
include_directories(
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc/Components
    ${CMAKE_SOURCE_DIR}/inc/Entities
    ${CMAKE_SOURCE_DIR}/inc/Entities/Enemies
    ${CMAKE_SOURCE_DIR}/inc/Game
    ${CMAKE_SOURCE_DIR}/inc/Systems
    ${CMAKE_SOURCE_DIR}/inc/UI
)

add_executable(RaccoonRampage ${SOURCES} ${HEADERS})

# Link Raylib + platform deps
if(WIN32)
    target_link_libraries(RaccoonRampage PRIVATE raylib opengl32 gdi32 winmm)
elseif(UNIX)
    target_link_libraries(RaccoonRampage PRIVATE raylib m pthread dl GL X11 Xrandr Xinerama Xcursor Xxf86vm Xi)
endif()