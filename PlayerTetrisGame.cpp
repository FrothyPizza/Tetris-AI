#include "PlayerTetrisGame.h"

#include "AI.h"

PlayerTetrisGame::PlayerTetrisGame() : gameState{}, curMino{ NULL }, rect{}  {


	for (int i = 0; i < 5; ++i) ts::pushOntoNextlist(nextList);


	curMino.setTetromino(nextList.front());
	nextList.erase(nextList.begin());


	//// testing
	//const static std::vector<std::vector<char>> shape{
	//{'A', 'O', '#', 'O', 'A'},
	//{'A', '#', '#', '#', 'A'},
	//{'1', '1', '#', '1', '1'}
	//};

	//std::vector<ts::Point> emptyPositions; emptyPositions.reserve(6);
	//std::vector<ts::Point> blockPositions; emptyPositions.reserve(6);
	//for (size_t i = 0; i < shape.size(); ++i)
	//	for (size_t j = 0; j < shape[i].size(); ++j) {
	//		if (shape[i][j] == '#') emptyPositions.push_back({ (int)j, (int)i });
	//		if (shape[i][j] == '1') blockPositions.push_back({ (int)j, (int)i });
	//	}

	//int x = 3; int y = ts::HEIGHT - 3;
	//for (auto& empty : emptyPositions)
	//	gameState.matrix[x + empty.x][y + empty.y] = 1;
	//for (auto& block : blockPositions)
	//	gameState.matrix[x + block.x][y + block.y] = 2;

	//int x = 3; int y = ts::HEIGHT-3;
	//bool foundAnO{ false };
	//for (size_t i = 0; i < shape.size(); ++i)
	//	for (size_t j = 0; j < shape[i].size(); ++j) {
	//		switch (shape[i][j]) {
	//		case '#':
	//			gameState.matrix[x + j][y + i] = -1;
	//			break;
	//		case '1':
	//			gameState.matrix[x + j][y + i] = 0;
	//			break;
	//		case 'A':
	//			gameState.matrix[x + j][y + i] = 1;
	//			break;
	//		case 'O':
	//			if (foundAnO)  ; // then both o's are blocks, meaning bad
	//			foundAnO = true;
	//			break;
	//		case 'R':
	//			for(int k = 0; k < ts::WIDTH-j-x; ++k)
	//				gameState.matrix[x + j + k][y + i] = 3;
	//			break;
	//		case 'L':
	//			for (int k = ts::WIDTH - j - x - 1; k >= 0; --k)
	//				gameState.matrix[k][y + i] = 3;
	//			break;
	//		}
	//	}

}

void PlayerTetrisGame::render(sf::RenderWindow* window, sf::Vector2f position, float tileSize) {
	renderTetris(window, position, tileSize, rect, gameState, curMino, nextList);
}

//void PlayerTetrisGame::inputRight(bool rightPressed) {
//	if (rightPressed && RclockRestarted == false) {
//		Rclock.restart();
//		RclockRestarted = true;
//		gameState.moveX(curMino, 1);
//	}
//	else {
//		if (rightPressed == false) RclockRestarted = false;
//	}
//	if (Rclock.getElapsedTime().asMilliseconds() > das && rightPressed) {
//		gameState.arrX(curMino, 1);
//		LclockRestarted = false;
//	}
//
//
//
//}
//
//void PlayerTetrisGame::inputLeft(bool leftPressed) {
//	if (leftPressed && LclockRestarted == false) {
//		Lclock.restart();
//		LclockRestarted = true;
//		gameState.moveX(curMino, -1);
//	}
//	else {
//		if (leftPressed == false) LclockRestarted = false;
//	}
//	if (Lclock.getElapsedTime().asMilliseconds() > das && leftPressed) {
//		//m_gameState.moveX(m_currentTetromino, -1);
//		gameState.arrX(curMino, -1);
//		RclockRestarted = false;
//	}
//}

void PlayerTetrisGame::inputRight(bool rightPressed, bool leftPressed) {
	//if (leftPressed) return;
	if (leftPressed && RARRClock.getElapsedTime().asMilliseconds() > 0 && rightPressed) {
		Lclock.restart();
	}


	if (rightPressed && RclockRestarted == false) {
		Rclock.restart();
		RclockRestarted = true;
		gameState.moveX(curMino, 1);

	}
	else {
		if (rightPressed == false) RclockRestarted = false;
	}
	if (arr > 0) {
		if (Rclock.getElapsedTime().asMilliseconds() > das && rightPressed) {
			if (RARRClock.getElapsedTime().asMilliseconds() > arr) {
				RARRClock.restart();
				gameState.moveX(curMino, 1);
			}

		}
	}
	else {
		if (Rclock.getElapsedTime().asMilliseconds() > das && rightPressed) {
			gameState.arrX(curMino, 1);
			//RclockRestarted = false;
		}
	}
}

void PlayerTetrisGame::inputLeft(bool leftPressed, bool rightPressed) {
	//if (rightPressed) return;
	if (rightPressed && LARRClock.getElapsedTime().asMilliseconds() > 0 && leftPressed) {
		Rclock.restart();
	}

	if (leftPressed && LclockRestarted == false) {
		Lclock.restart();
		LclockRestarted = true;
		gameState.moveX(curMino, -1);

	}
	else {
		if (leftPressed == false) LclockRestarted = false;
	}
	if (arr > 0) {
		if (Lclock.getElapsedTime().asMilliseconds() > das && leftPressed) {
			if (LARRClock.getElapsedTime().asMilliseconds() > arr) {
				LARRClock.restart();
				gameState.moveX(curMino, -1);
			}
		}
	}
	else {
		if (Lclock.getElapsedTime().asMilliseconds() > das && leftPressed) {
			gameState.arrX(curMino, -1);
			//LclockRestarted = false;
		}
	}
}

void PlayerTetrisGame::inputDown() {
	if (sdf == 0) {
		gameState.hardDropWithoutPaste(curMino);
	}
	else {
		if (SDFClock.getElapsedTime().asMilliseconds() > sdf) {
			SDFClock.restart();
			gameState.softDrop(curMino);
		}
	}
}

int PlayerTetrisGame::inputGeneral(int keyCode) {
	// hard drop
	if (keyCode == ts::Globals::CONTROLS["hard_drop"]) {
		

		int mino = curMino.mino;
		ts::Tetromino oldMino{ curMino };
		ts::GameState oldState{ gameState };
		gameState.hardDrop(curMino);
		curMino.setTetromino(nextList.front());
		nextList.erase(nextList.begin());
		if (nextList.size() < 14) ts::pushOntoNextlist(nextList);
		//piecesPlaced++;

		int clear = gameState.lastPlacementClear();
		if (clear <= 0) gameState.placeGarbage();

		int attack = gameState.lastPlacementAttack();

		//std::cout << "Eval: " << ts::evaluate(gameState, ts::mainFactors)<<'\n';
		//// handle back to back
		//if (clear == 4 || gameState.getLastTSpin() != gameState.NO_TSPIN && clear > 0) {
		//	gameState.b2b++;
		//}
		//else {
		//	if (clear > 0) gameState.b2b = 0;
		//}

		//if (clear == 0) attack = 0;


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//for (int i = 0; i < 5; ++i)
		//	std::cout << '\n';
		//ts::getAllMoves(gameState, curMino.mino);

		if (clear == 0) {
			ts::Globals::SOUNDS["hard_drop"].play();
		}
		else {
			ts::Globals::SOUNDS["line_clear"].setPitch(gameState.combo / 16.f + 1);
			ts::Globals::SOUNDS["line_clear"].play();
		}
		


		return attack;
	}

	if (keyCode == ts::Globals::CONTROLS["hold"]) {
		gameState.performHold(gameState, curMino, nextList);
	}
	if (keyCode == ts::Globals::CONTROLS["rot_cw"]) {
		gameState.rotate(curMino, 1);
	}
	if (keyCode == ts::Globals::CONTROLS["rot_ccw"]) {
		gameState.rotate(curMino, -1);
	}
	if (keyCode == ts::Globals::CONTROLS["rot_180"]) {
		gameState.rotate(curMino, 2);
	}
	if (keyCode == ts::Globals::CONTROLS["restart"]) {
		gameState.resetMatrix();
	}

	if ((keyCode == ts::Globals::CONTROLS["rot_cw"] ||
		keyCode == ts::Globals::CONTROLS["rot_ccw"] ||
		keyCode == ts::Globals::CONTROLS["rot_180"]) &&
		gameState.getLastTSpinBeforeHardDrop(curMino) != 0) {
		//std::cout << "Tspin\n";
		//ts::Globals::SOUNDS["t_spin"].play();
	}


	return 0;
}

void PlayerTetrisGame::recieveAttack(int attack) {
	gameState.recieveAttack(attack);
}
