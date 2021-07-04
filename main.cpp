
#include "Application.h"
#include "WindowsTetrisScreenManager.h"


//void Stealth() {
//    HWND Stealth;
//    AllocConsole();
//    Stealth = FindWindowA("ConsoleWindowClass", NULL);
//    ShowWindow(Stealth, 0);
//}

int main() {


    srand(static_cast<unsigned int>(time(NULL)));

    //int matrix[ts::WIDTH][ts::HEIGHT];
    //ts::WindowsTetrisScreenManager::askForMatrixLocation();
    //while (true) ts::WindowsTetrisScreenManager::getNextList();
    //while(true)     ts::WindowsTetrisScreenManager::updateMatrixFromScreen(matrix);


    //Stealth();

    ts::Globals::InitVars();
    Application tetrisGame{};










    //// speed testing
    //float total = 0;
    //ts::GameState test{};
    //ts::Tetromino mino{ 2 };
    //for(int i = 0; i < 5; ++i) test.hardDrop(mino);
    //auto before = clock();
    //for (int i = 0; i < 10000; ++i) {
    //    total += ts::evaluate(test, ts::mainFactors);
    //    //ts::getAllMoves(test, ts::MINO_T);
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