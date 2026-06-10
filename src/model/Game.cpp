#include "../../include/model/Game.hpp"

bool Game::load() {
    return map.load("assets/grass.png", "assets/path.png", "assets/tower_base.png");
}
