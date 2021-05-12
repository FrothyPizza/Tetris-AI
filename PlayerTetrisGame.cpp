#include "PlayerTetrisGame.h"

#include "AI.h"

PlayerTetrisGame::PlayerTetrisGame() : gameState{}, curMino{ NULL }, rect{}  {


	for (int i = 0; i < 5; ++i) ts::pushOntoNextlist(nextList);


	curMino.setTetromino(nextList.front());
	nextList.erase(nextList.begin());

}

void PlayerTetrisGame::render(sf::RenderWindow* window, sf::Vector2f position, float tileSize) {
	renderTetris(window, position, tileSize, rect, gameState, curMino, nextList);
}

void PlayerTetrisGame::inputRight(bool rightPressed) {
	if (rightPressed && RclockRestarted == false) {
		Rclock.restart();
		RclockRestarted = true;
		gameState.moveX(curMino, 1);
	}
	else {
		if (rightPressed == false) RclockRestarted = false;
	}
	if (Rclock.getElapsedTime().asMilliseconds() > das && rightPressed) {
		gameState.arrX(curMino, 1);
		LclockRestarted = false;
	}



}

void PlayerTetrisGame::inputLeft(bool leftPressed) {
	if (leftPressed && LclockRestarted == false) {
		Lclock.restart();
		LclockRestarted = true;
		gameState.moveX(curMino, -1);
	}
	else {
		if (leftPressed == false) LclockRestarted = false;
	}
	if (Lclock.getElapsedTime().asMilliseconds() > das && leftPressed) {
		//m_gameState.moveX(m_currentTetromino, -1);
		gameState.arrX(curMino, -1);
		RclockRestarted = false;
	}
}

void PlayerTetrisGame::inputDown() {
	gameState.hardDropWithoutPaste(curMino);
}

int PlayerTetrisGame::inputGeneral(int keyCode) {
	// hard drop
	if (keyCode == sf::Keyboard::Space) {
		int mino = curMino.mino;
		ts::Tetromino oldMino{ curMino };
		ts::GameState oldState{ gameState };
		gameState.hardDrop(curMino);
		curMino.setTetromino(nextList.front());
		nextList.erase(nextList.begin());
		if (nextList.size() < 14) ts::pushOntoNextlist(nextList);
		int clear = gameState.clearLines();
		//piecesPlaced++;
		int tSpin = oldState.isTSpin(oldMino);

		if (clear <= 0) gameState.placeGarbage();

		int attack = gameState.getAttack(tSpin, mino, clear);

		// handle back to back;
		if (clear == 4 || tSpin != gameState.NO_TSPIN && clear > 0) {
			gameState.b2b++;
		}
		else {
			if (clear > 0) gameState.b2b = 0;
		}

		if (clear == 0) attack = 0;
		return attack;
	}

	if (keyCode == sf::Keyboard::C) {
		gameState.performHold(gameState, curMino, nextList);
	}
	if (keyCode == sf::Keyboard::Up) {
		gameState.rotate(curMino, 1);
	}
	if (keyCode == sf::Keyboard::Z) {
		gameState.rotate(curMino, -1);
	}
	if (keyCode == sf::Keyboard::X) {
		gameState.rotate(curMino, 2);
	}
	if (keyCode == sf::Keyboard::Q) {
		gameState.resetMatrix();
	}



	return 0;
}

void PlayerTetrisGame::recieveAttack(int attack) {
	gameState.recieveAttack(attack);
}
