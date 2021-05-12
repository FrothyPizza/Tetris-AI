
#include "Application.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    
    Application tetrisGame{};


    int fps = 144;
    sf::Clock timer;
    timer.restart();

    while (tetrisGame.running()) {
        tetrisGame.update();

        if (timer.getElapsedTime().asMilliseconds() > 1000 / fps) {
            tetrisGame.render();
            timer.restart();
        }
    }

    return 0;
}