#include "Tetromino.h"


namespace ts {

	Tetromino::Tetromino(int minoIndex) :
		mino{ minoIndex }, data{ } {

		if (minoIndex == MINO_O || minoIndex == MINO_I) x = 5; else x = 4;
		y = YMARGIN - TETROMINO_START_Y;
		rotation = 0;

		//data.reserve(4);
		for (int i = 0; i < 4; ++i) {
			//data.push_back(tetrominoData[minoIndex][i]);
			data[i] = tetrominoData[minoIndex][i];
		}
	}

	// resets the tetromino x, y, and rotation and makes it the new tetromino
	void Tetromino::setTetromino(int minoIndex) {
		mino = minoIndex;
		if (minoIndex == MINO_O || minoIndex == MINO_I) x = 5; else x = 4;
		y = YMARGIN - TETROMINO_START_Y;
		rotation = 0;

		for (int i = 0; i < 4; ++i) {
			data[i] = tetrominoData[minoIndex][i];
		}
	}


}