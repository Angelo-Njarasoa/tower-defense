#include <SFML/Graphics.hpp>
#include <memory>
#include "model/Game.hpp"
#include "view/GameView.hpp"
#include "view/SoundManager.hpp"
#include "controller/GameController.hpp"

int main() {
    // Fullscreen at desktop resolution; a scaled view maps the logical 768x620 canvas
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Tower Defense", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Scale the logical canvas (768x620) to fill the screen while keeping aspect ratio
    sf::View scaledView(sf::FloatRect(0.f, 0.f, 768.f, 620.f));
    float scaleX = desktop.width  / 768.f;
    float scaleY = desktop.height / 620.f;
    float scale  = std::min(scaleX, scaleY);
    float vpW    = (768.f * scale) / desktop.width;
    float vpH    = (620.f * scale) / desktop.height;
    scaledView.setViewport(sf::FloatRect((1.f - vpW) / 2.f, (1.f - vpH) / 2.f, vpW, vpH));
    window.setView(scaledView);

    auto game = std::make_shared<Game>();
    if (!game->load()) return -1;

    auto gameView  = std::make_shared<GameView>(window, game);
    auto sound     = std::make_shared<SoundManager>();
    GameController controller(window, game, gameView);
    controller.subscribe(sound);

    sf::Clock clock;
    while (controller.isRunning()) {
        sf::Event event;
        while (window.pollEvent(event))
            controller.handleEvent(event);

        float dt = clock.restart().asSeconds();
        controller.update(dt);
        controller.render();
    }
    return 0;
}
