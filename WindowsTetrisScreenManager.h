#pragma once


#include "Globals.h"
#include "Tetromino.h"
#include <Windows.h>
#include <vector>

namespace ts {
	class WindowsTetrisScreenManager {
	public:


		// through the console, ask for the top left and bottom right of the matrix on screen
		// so that it can read it properly
		// also ask for the position of the next list and hold
		static void askForMatrixLocation();

		// return the next list as read from the screen
		static std::vector<int> getNextList();

		static int getHold();

		static void updateMatrixFromScreen(int(&matrix)[WIDTH][HEIGHT]);


		enum class Game {
			TETRIO,
			JSTRIS
		};

		static Game playingGame;

	private:

		static Point nextListPositions[5];

		
		


		static int matrixX, matrixY; // x, y
		static int matrixW, matrixH; // width, height
		static int incX, incY;		  // incrememt between tiles

		//https://www.youtube.com/watch?v=xTqvtSfCla0&ab_channel=CasualGamer
		static int get_Bitmap(int x, int y, HDC& hdcMemory, int width, int height);


    public:

        typedef struct {
			unsigned char r, g, b;
        } RGB;

		//https://stackoverflow.com/questions/9018016/how-to-compare-two-colors-for-similarity-difference
		static double ColorDistance(RGB e1, RGB e2) {
			long rmean = ((long)e1.r + (long)e2.r) / 2;
			long r = (long)e1.r - (long)e2.r;
			long g = (long)e1.g - (long)e2.g;
			long b = (long)e1.b - (long)e2.b;
			return sqrt((((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8));
		}
		static std::vector<RGB> tetrominoColors;
		//static std::unordered_map <int, RGB> tetrominoColors;



    


    };
}