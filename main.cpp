
#include "Application.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    ts::Globals::InitVars();
    Application tetrisGame{};


    //// speed testing
    //ts::GameState test{};
    //auto before = clock();
    //for (int i = 0; i < 100000; ++i) {
    //    //ts::evaluate(test, ts::mainFactors);
    //    ts::getAllMoves(test, ts::MINO_T);
    //}
    //auto after = clock();
    //std::cout << "MS: " << after - before << '\n';



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