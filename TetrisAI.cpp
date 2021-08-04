#include "TetrisAI.h"
#include "TetrisRender.h"

namespace ts {
	

	TetrisAI::TetrisAI(bool inst) :
		gameState{},
		rect{},
		curMino{ 0 },
		nextList{},
		currentMove{},
		instant{ inst },
		totalAttack{ 0 },
		totalKeysPressed{ 0 },
		piecesPlaced{ 0 },
		dead{ false },
		factors{ mainFactors } {
		//reserve some number
		nextList.reserve(100);


		for (int i = 0; i < 5; ++i) pushOntoNextlist(nextList);

		curMino.setTetromino(nextList.front());
		nextList.erase(nextList.begin());

		//gameState.canHold = true;



		// Potential t spin setup for testing

		//for (int j = 1; j <= 2; ++j) {
		//	for (int i = 0; i < 10; ++i) {
		//		gameState.matrix[i][HEIGHT - j] = 1;
		//	}
		//}
		//gameState.matrix[3][HEIGHT - 1] = -1;
		//gameState.matrix[2][HEIGHT - 2] = -1;
		//gameState.matrix[3][HEIGHT - 2] = -1;
		//gameState.matrix[4][HEIGHT - 2] = -1;
		//gameState.matrix[4][HEIGHT - 3] = 1;
		//curMino.setTetromino(MINO_T);


		// 2 wide downstack testing
		//for (int i = 0; i < WIDTH-2; ++i) 
		//	for (int j = 7; j < HEIGHT; ++j) 
		//		gameState.matrix[i][j] = 0;


	}

	TetrisAI::TetrisAI(std::vector<int> next, bool inst) : TetrisAI(inst) {
		nextList = next;
		
	}

	TetrisAI::TetrisAI(AIFactor factor) : TetrisAI(true) {
		factors = factor;
	}

	int TetrisAI::doNextMove() {
		//if (isDead()) return 0;
		if(!Globals::HOLDENABLED) gameState.canHold = false; ///////////////////////////////////////////////////////////////////////////

		if (dead) {
			//gameState.resetMatrix();
			dead = false;
			//std::cout << piecesPlaced << '\n';
			return 0;
		}
		if (instant) return completelyPerformMove();
		else return performSingleMove();
	}

	void TetrisAI::recieveAttack(int atk) {
		gameState.recieveAttack(atk);
	}

	void TetrisAI::reset() {
		gameState.resetMatrix();
	}

	void TetrisAI::render(sf::RenderWindow* window, sf::Vector2f position, float tileSize) {
		renderTetris(window, position, tileSize, rect, gameState, curMino, nextList);
	}

	int TetrisAI::performSingleMove() {
		int thisMoveAttack{ 0 };
		if (currentMove.empty()) {
			generateNextMove();
		}

		// if it is the last move of the list, check it for tspin and calculate attack
		// , otherwise, just execute the move
		if (currentMove.at(0) == MOVE_HD) {
			int mino = curMino.mino;

			int clear = executeMove(gameState, curMino, currentMove, 0, nextList);
			int attack = gameState.lastPlacementAttack();


			piecesPlaced++;
			totalAttack += attack;
			thisMoveAttack = attack;


			if (clear <= 0) gameState.placeGarbage();
			//std::cout << "APP: " << (float)totalAttack / (float)piecesPlaced << attack << std::endl;
			//if(rand() % 3 == 0)recieveAttack(1); ////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//gameState.placeGarbage(); /////////////////////////////////////
		}
		else {
			executeMove(gameState, curMino, currentMove, 0, nextList);
		}

		int lastMove = currentMove.at(0);
		currentMove.erase(currentMove.begin());

		if (currentMove.empty() && lastMove != HOLD) {
			curMino.setTetromino(nextList.front());
			if (gameState.matrixContains(curMino)) dead = true; // check to see if it dies
			nextList.erase(nextList.begin());
			if (nextList.size() < 14) pushOntoNextlist(nextList);
		}

		return thisMoveAttack;
	}

	int TetrisAI::completelyPerformMove() {
		generateNextMove();

		int cleared = fullyPerformMove(gameState, curMino, currentMove, nextList);
		int Tspin = gameState.getLastTSpinAfterHardDrop();
		int mino = curMino.mino;

		int attack = gameState.lastPlacementAttack();



		if (currentMove.at(0) != HOLD) {
			curMino.setTetromino(nextList.front());
			if (gameState.matrixContains(curMino)) dead = true;
			nextList.erase(nextList.begin());

			totalAttack += attack;
			piecesPlaced++;
			gameState.placeGarbage();
		}

		if (nextList.size() < 14) pushOntoNextlist(nextList);



		/*std::cout << "APP: " << (float)totalAttack / (float)piecesPlaced << attack << std::endl;*/
		return attack;
	}

	void TetrisAI::generateNextMove() {
		currentMove = findBestMove(factors, gameState, curMino.mino, nextList, 0, Globals::AI_LOOKAHEADS);
		totalKeysPressed += currentMove.size();
		//std::cout << "\nKPP = " << (float)totalKeysPressed / piecesPlaced;
		//currentMove = getAIMove(factors, gameState, curMino.mino, nextList, 0, Globals::AI_LOOKAHEADS).first;
		//for (int i = 0; i < currentMove.size(); ++i) std::cout << '\n' << currentMove.at(i);
	}



}