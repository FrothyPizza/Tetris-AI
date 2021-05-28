#include "AI.h"
#include <algorithm>



std::vector<int> ts::findBestMove(AIFactor& AIfactor, GameState& currentState, int minoIndex, std::vector<int>& nextList, float thisPathAttackScore, int depth) {
	

	std::vector<int> bestMove{};
	float bestScore{ -100000.f };
	{

		std::vector<std::vector<int>> allMoves = getAllMoves(currentState, minoIndex);
		std::vector<float> evaluations{};
		std::vector<float> attackEvaluations{};
		std::vector<GameState> allNewStates{};
		evaluations.reserve(allMoves.size());
		allNewStates.reserve(allMoves.size());
		attackEvaluations.reserve(allMoves.size());


		for (size_t i = 0; i < allMoves.size(); ++i) {
			allNewStates.push_back(GameState{ currentState });
	
			//GameState newState{ currentState };
			Tetromino mino{ minoIndex };
			std::vector<int> newNext{ nextList };

			fullyPerformMove(allNewStates.at(i), mino, allMoves.at(i), newNext);
			float evaluation = evaluate(allNewStates.at(i), AIfactor);
			float atkScore = evaluateAttackScore(AIfactor, allNewStates.at(i).lastPlacementAttack(), allNewStates.at(i).lastPlacementClear());
			evaluation += atkScore;

			attackEvaluations.push_back(atkScore);
			evaluations.push_back(evaluation);
		}


		if (depth == 0) { // if depth is 0 then it doesnt have to go through the trouble of sorting the evaluations and looking ahead
			for (size_t i = 0; i < evaluations.size(); ++i) {
				if (evaluations.at(i) + attackEvaluations.at(i) > bestScore) {
					bestMove = allMoves.at(i);
					bestScore = evaluations.at(i) + attackEvaluations.at(i);
				}
			}
		}
		else {
			int bestMovesToBeConsidered = Globals::AI_BEST_MOVES_TO_BE_CONSIDERED;
			//// since O only has 8 moves, only consider 8 to prevent error
			//if (minoIndex == MINO_O && AI_BEST_MOVES_TO_BE_CONSIDERED > 8) bestMovesToBeConsidered = 9;
			if ((unsigned int)bestMovesToBeConsidered > allMoves.size()) bestMovesToBeConsidered = allMoves.size();


			std::vector<int> bestIndexes{};
			for (int i = 0; i < bestMovesToBeConsidered; ++i) {
				int bestIndex = 0;
				float bestEvaluation = -10000;
				// loop through all evaluations and find the best one that hasn't already been considered
				for (unsigned int j = 0; j < evaluations.size(); ++j) {
					bool bestIndexesHasJ{ false };
					for (size_t c = 0; c < bestIndexes.size(); ++c)
						if (j == bestIndexes.at(c)) // if the index being considered has already been added, don't add it
							bestIndexesHasJ = true;
					//for (size_t c = 0; c < bestIndexes.size(); ++c)
					//	if (allNewStates.at(j) == allNewStates.at(bestIndexes.at(c))) // if the gamestate is a duplicate, don't consider it
					//		bestIndexesHasJ = true;
					if (evaluations.at(j) > bestEvaluation && bestIndexesHasJ == false) {
						bestEvaluation = evaluations.at(j);
						bestIndex = j;
					}
				}
				bestIndexes.push_back(bestIndex);

			}


			for (size_t i = 0; i < bestIndexes.size(); ++i) {
				GameState thisState{ allNewStates.at(bestIndexes.at(i)) };
				Tetromino thisMino{ nextList.at(0) };
				std::vector<int> newNext{ nextList.begin(), nextList.end() };
				std::vector<int> move = findBestMove(AIfactor, thisState, nextList.at(0), newNext, thisPathAttackScore + attackEvaluations.at(bestIndexes.at(i)), depth - 1);
				fullyPerformMove(thisState, thisMino, move, newNext);

				float evaluation = evaluate(thisState, AIfactor) + evaluateAttackScore(AIfactor, thisState.lastPlacementAttack(), thisState.lastPlacementClear());

				if (evaluation > bestScore) {
					bestScore = evaluation;
					bestMove = allMoves.at(bestIndexes.at(i));
				}
			}
		}

	}

	if (currentState.canHold) {
		std::vector<int> holdNext{ nextList };
		GameState holdState{ currentState };
		Tetromino holdMino{ minoIndex };
		holdState.performHold(holdState, holdMino, holdNext);

		std::vector<std::vector<int>> allMovesWithHold = getAllMoves(holdState, holdMino.mino);
		std::vector<float> evaluations{};
		std::vector<float> attackEvaluations{};
		std::vector<GameState> allNewStates{};
		evaluations.reserve(allMovesWithHold.size());
		allNewStates.reserve(allMovesWithHold.size());
		attackEvaluations.reserve(allMovesWithHold.size());

		for (size_t i = 0; i < allMovesWithHold.size(); ++i) {
			allNewStates.push_back(holdState);
			Tetromino mino{ holdMino.mino };
			std::vector<int> newNext{ holdNext };

			fullyPerformMove(allNewStates.at(i), mino, allMovesWithHold.at(i), newNext);
			float evaluation = evaluate(allNewStates.at(i), AIfactor);
			float atkScore = evaluateAttackScore(AIfactor, allNewStates.at(i).lastPlacementAttack(), allNewStates.at(i).lastPlacementClear());
			evaluation += atkScore;

			attackEvaluations.push_back(atkScore);
			evaluations.push_back(evaluation);
		}


		if (depth == 0) {
			for (size_t i = 0; i < evaluations.size(); ++i) {
				if (evaluations.at(i) + attackEvaluations.at(i) > bestScore) {
					bestMove = std::vector<int>{ HOLD };
					bestScore = evaluations.at(i) + attackEvaluations.at(i);

				}
			}
		}
		else {
			int bestMovesToBeConsidered = Globals::AI_BEST_MOVES_TO_BE_CONSIDERED;
			//// since O only has 8 moves, only consider 8 to prevent error
			//if (minoIndex == MINO_O && AI_BEST_MOVES_TO_BE_CONSIDERED > 8) bestMovesToBeConsidered = 8;
			if ((unsigned int)bestMovesToBeConsidered > allMovesWithHold.size()) bestMovesToBeConsidered = allMovesWithHold.size();


			std::vector<int> bestIndexes{};
			for (int i = 0; i < bestMovesToBeConsidered; ++i) {
				int bestIndex = 0;
				float bestEvaluation = -10000;
				// loop through all evaluations and find the best one that hasn't already been considered
				for (unsigned int j = 0; j < evaluations.size(); ++j) {
					bool bestIndexesHasJ{ false };
					for (size_t c = 0; c < bestIndexes.size(); ++c)
						if (j == bestIndexes.at(c)) // if the index being considered has already been added, don't add it
							bestIndexesHasJ = true;
					//for (size_t c = 0; c < bestIndexes.size(); ++c)
					//	if (allNewStates.at(j) == allNewStates.at(bestIndexes.at(c))) // if the gamestate is a duplicate, don't consider it
					//		bestIndexesHasJ = true;
					if (evaluations.at(j) > bestEvaluation && bestIndexesHasJ == false) {
						bestEvaluation = evaluations.at(j);
						bestIndex = j;
					}
				}
				bestIndexes.push_back(bestIndex);

			}


			for (size_t i = 0; i < bestIndexes.size(); ++i) {
				GameState thisState{ allNewStates.at(bestIndexes.at(i)) };
				Tetromino thisMino{ holdNext.at(0) };
				std::vector<int> newNext{ holdNext.begin(), holdNext.end() };
				std::vector<int> move = findBestMove(AIfactor, thisState, holdNext.at(0), newNext, thisPathAttackScore + attackEvaluations.at(bestIndexes.at(i)), depth - 1);
				fullyPerformMove(thisState, thisMino, move, newNext);

				float evaluation = evaluate(thisState, AIfactor) + evaluateAttackScore(AIfactor, thisState.lastPlacementAttack(), thisState.lastPlacementClear());;
				if (evaluation > bestScore) {
					bestScore = evaluation;
					//bestMove = allMovesWithHold.at(bestIndexes.at(i));
					bestMove.erase(bestMove.begin(), bestMove.end());
					bestMove.push_back(HOLD);
					i = bestIndexes.size(); // if holding is better, don't evaluate any more
				}
			}
		}

	}

	return bestMove;
	
	
}

//// This doens't work
//std::pair<std::vector<int>, float> ts::getAIMove(AIFactor& AIfactor, GameState& currentState, int minoIndex, std::vector<int>& nextList, float thisPathTotalAttackScore, int depth) {
//
//	if (depth == 0) {
//		return std::pair<std::vector<int>, float>{std::vector<int>{}, evaluate(currentState, AIfactor) + thisPathTotalAttackScore};
//	}
//
//	std::pair<std::vector<int>, float> bestMove{};
//
//
//	// no hold
//	{
//
//		// generate all possible moves
//		std::vector<std::vector<int>> allMoves{ getAllMoves(currentState, minoIndex) };
//
//		// for each move, execute it and lookahead
//		for (size_t i = 0; i < allMoves.size(); ++i) {
//			GameState oldState{ currentState };
//			GameState newState{ currentState };
//			Tetromino mino{ minoIndex };
//			std::vector<int> newNext{ nextList };
//
//			int clear = fullyPerformMove(newState, mino, allMoves.at(i), newNext);
//			int Tspin = oldState.isTSpin(mino);
//			int attack = newState.getAttack(Tspin, minoIndex, clear);
//			float evaluation = evaluate(newState, AIfactor);
//			float atkScore = evaluateAttackScore(AIfactor, attack, clear);
//			
//			float overallScore = getAIMove(AIfactor, newState, nextList.at(0), newNext, atkScore + thisPathTotalAttackScore, depth - 1).second;
//
//			if (overallScore > bestMove.second) {
//				bestMove = std::pair<std::vector<int>, float>{ allMoves.at(i), overallScore };
//			}
//		}
//
//
//	}
//
//
//	return bestMove;
//}




// find the percent completion of a shape
// i.e. the T spin shape could be
// A A O # O A A
// L 1 # # # 1 R
// L 1 1 # 1 1 R
// 
// where # is empty, 
// 1 is a block,
// A means empty or a block,
// L means full all the way down to the left, R for the right,
// O can be used twice; it means one or the other, but not both, and not neither (xor)
//
// If any of the squares that should be empty are full, return 0 percent completion
float shapePercentCompletion(std::vector<std::vector<int>>& shape) {
	return 0.f;
}


float ts::evaluate(const GameState& gameState, AIFactor& AIfactor) {
	float score{ 0 };

	//// if dead
	//for (int i = 3; i < 6; ++i)
	//	for (int j = 4; j < 6; ++j)
	//		if (gameState.matrix[i][j] >= 0) return -1000;


	//find holes
	int holeCount{ 0 };
	int coverdHoleCount{ 0 };
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT - 1; ++j) {
			if (gameState.matrix[i][j] >= 0 && gameState.matrix[i][j + 1] == -1) {
				holeCount++;
				// punish deep holes but only up to 3 deep
				for (int c = 0; c < 3; ++c) {
					if (gameState.matrix[i][j + c] == -1 && gameState.matrix[i][j + c - 1] == -1) {
						++holeCount;
					}
				}
				// blocks above a hole
				// only up to 5
				for (int c = 0; c < 5; ++c) {
					if (gameState.matrix[i][j - c] >= 0)
						++coverdHoleCount;
				}
			}
		}
	}
	score += holeCount * AIfactor.hole;
	score += coverdHoleCount * AIfactor.holeCovered;


	// create a vector containing column heights
	std::vector<int> columnHeights{}; // create a vector with values of 0
	columnHeights.reserve(WIDTH);
	for (int i = 0; i < WIDTH; ++i) {
		int thisColHeight{ 0 };
		for (int j = 0; j < HEIGHT; ++j) {
			// if a tile is found
			if (gameState.matrix[i][j] >= 0) {
				thisColHeight = HEIGHT - j;
				j = HEIGHT; // continue to next column
			}
		}
		columnHeights.push_back(thisColHeight);
	}


	// calculate average height
	float averageHeight{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		averageHeight += columnHeights.at(i);
	}
	averageHeight /= columnHeights.size();
	score += averageHeight * AIfactor.averageHeight;


	// Calculate I dependencies and height variance
	int Idependencies{ 0 };
	int variance{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		int leftHeight{ 0 };
		int thisHeight{ columnHeights.at(i) };
		int rightHeight{ 0 };

		// if it isn't index 0 or last
		if (i >= 1 && i < columnHeights.size() - 1) {
			variance += abs(columnHeights.at(i - 1) - thisHeight);
			variance += abs(columnHeights.at(i + 1) - thisHeight);
		}

		// calculate I dependecies 
		if (i == 0) leftHeight = HEIGHT;
		else leftHeight = columnHeights.at(i - 1);

		if (i == columnHeights.size() - 1) rightHeight = HEIGHT;
		else rightHeight = columnHeights.at(i + 1);

		if (leftHeight - thisHeight >= 3 && rightHeight - thisHeight >= 3) Idependencies++;
	}
	score += Idependencies * AIfactor.Idependency;
	score += variance * AIfactor.heightVariance;

	

	// Calculate parity
	int parity{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		int color = (columnHeights.at(i) % 2) ^ (i % 2);
		if (color == 1) ++parity;
		else --parity;
	}
	parity /= 2;
	parity = abs(parity);
	score += parity * AIfactor.surfaceParity;
	// 1  0  1  0
	// 0  1  0  1
	// 1  0  1  0
	// 0  1  0  1
	// 1  0  1  0




	return score;
}




float ts::evaluateAttackScore(AIFactor& AIfactor, int attack, int clear) {
	float score{ 0 };

	// total attack sent
	score += attack * attack * AIfactor.attack; // attack squared to encourage larger clears


	// clear without attack
	if (clear > 0 && attack == 0) {
		score += clear * AIfactor.clearWithoutAttack;
	}

	float atkeff{ 0 };
	atkeff = static_cast<float>(attack) / static_cast<float>(clear);
	// attack efficiency
	if (clear > 0 && attack > 0) {
		score += atkeff * AIfactor.attackEffeciency;
	}



	//return 0; ///////////////////////////////////////////////////////////////////////////////////////////////
	return score;
}





std::vector<std::vector<int>> ts::getAllMoves(GameState& gameState, int mino) {



	std::vector<std::vector<int>> allMoves;
	allMoves.reserve(60);

	// all of the positions in the air before hard drop
	std::vector<std::vector<int>> skyMoves;
	skyMoves.reserve(30);


	// no rotation sky positions
	if (mino == MINO_L || mino == MINO_J || mino == MINO_S || mino == MINO_Z || mino == MINO_T) {
		skyMoves.push_back(std::vector<int>{MOVE_LL});
		skyMoves.push_back(std::vector<int>{MOVE_LL, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_L});
		skyMoves.push_back(std::vector<int>{});
		skyMoves.push_back(std::vector<int>{MOVE_RR});
		skyMoves.push_back(std::vector<int>{MOVE_RR, MOVE_L});
		skyMoves.push_back(std::vector<int>{MOVE_R, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_R});

		// all of the sky positions are the same for L, J, and T
		if (mino == MINO_L || mino == MINO_J || mino == MINO_T) {
			// 180 rotation
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_LL});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_LL, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_L});
			skyMoves.push_back(std::vector<int>{ROT_180});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_RR});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_RR, MOVE_L});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_R, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_180, MOVE_R});


			// CCW rotation
			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_LL});
			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_L, MOVE_L});
			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_L});

			skyMoves.push_back(std::vector<int>{ROT_CCW});

			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_R, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_RR});
			skyMoves.push_back(std::vector<int>{MOVE_RR, ROT_CCW});
			skyMoves.push_back(std::vector<int>{MOVE_RR, ROT_CCW, MOVE_L});


			// CW rotation
			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_LL});
			skyMoves.push_back(std::vector<int>{MOVE_LL, ROT_CW});
			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_L});
			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_L, MOVE_L});


			skyMoves.push_back(std::vector<int>{ROT_CW});

			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_R, MOVE_R});
			skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_LL});


		}
	}

	// S and Z are the same
	if (mino == MINO_Z || mino == MINO_S) {
		// rotation position 2
		skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_LL});
		skyMoves.push_back(std::vector<int>{MOVE_LL, ROT_CW});
		skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_L});

		skyMoves.push_back(std::vector<int>{ROT_CCW});

		skyMoves.push_back(std::vector<int>{ROT_CW});
		skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_R});
		skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_R, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_RR, ROT_CW});
		skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_RR});


	}


	if (mino == MINO_I) {
		// no rotation
		skyMoves.push_back(std::vector<int>{MOVE_LL});
		skyMoves.push_back(std::vector<int>{MOVE_L, MOVE_L});
		skyMoves.push_back(std::vector<int>{MOVE_L});

		skyMoves.push_back(std::vector<int>{});

		skyMoves.push_back(std::vector<int>{MOVE_RR});
		skyMoves.push_back(std::vector<int>{MOVE_R, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_R});


		// rotated once
		skyMoves.push_back(std::vector<int>{ROT_CCW});
		skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_L});
		skyMoves.push_back(std::vector<int>{MOVE_LL, ROT_CW});
		skyMoves.push_back(std::vector<int>{MOVE_LL, ROT_CCW});
		skyMoves.push_back(std::vector<int>{ROT_CCW, MOVE_LL});

		skyMoves.push_back(std::vector<int>{ROT_CW});
		skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_RR, ROT_CCW});
		skyMoves.push_back(std::vector<int>{MOVE_RR, ROT_CW});
		skyMoves.push_back(std::vector<int>{ROT_CW, MOVE_RR});
	}

	if (mino == MINO_O) {
		// only one rotation for this
		skyMoves.push_back(std::vector<int>{MOVE_LL});
		skyMoves.push_back(std::vector<int>{MOVE_LL, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_L, MOVE_L});
		skyMoves.push_back(std::vector<int>{MOVE_L});
		skyMoves.push_back(std::vector<int>{});

		skyMoves.push_back(std::vector<int>{MOVE_RR});
		skyMoves.push_back(std::vector<int>{MOVE_RR, MOVE_L});
		skyMoves.push_back(std::vector<int>{MOVE_R, MOVE_R});
		skyMoves.push_back(std::vector<int>{MOVE_R});
	}




	//// Find unique positions /////////////////////////////////////////////////////////////
	// (doesn't work)
	if (Globals::AI_USE_UNIQUE_MOVES) {
		Tetromino m{ mino };

		std::vector<int> fakeNext{ 0 }; // fake next list so the fullyPerformMove call is valid
										// this is kinda hackey, but whatever

		// endPositions is all ending positions of the tetromino for each move
		std::vector<EndPosition> endPositions;
		endPositions.reserve(skyMoves.size());

		int highestY{ HEIGHT }; // start at bottom
		int lowestY{ 0 }; // the higher the number, the lower the elevation
		// add hard drop without pasting to the moves
		// and then perfrom the move so that the end position each move is known
		for (size_t i = 0; i < skyMoves.size(); ++i) {
			skyMoves.at(i).push_back(MOVE_DD);

			// reset the mino
			m.setTetromino(mino);

			// perform the move
			fullyPerformMove(gameState, m, skyMoves.at(i), fakeNext);

			// add this endpostion to the vector
			endPositions.push_back(EndPosition{ m.x, m.y, m.rotation });


			if (m.y < highestY) highestY = m.y;
			if (m.y > lowestY) lowestY = m.y;
		}

		std::vector<EndPosition> newEndPositions;
		newEndPositions.reserve(5);
		for (int i = 0; i < WIDTH; ++i) {
			for (int j = highestY; j < lowestY; ++j) {
				m.setTetromino(mino);
				m.x = i;
				m.y = j;

				if (gameState.matrixContains(m)) continue;
				gameState.hardDropWithoutPaste(m);

				bool isNew{ true };
				for (size_t c = 0; c < endPositions.size(); ++c) {
					// if it is the same as an already existing end position
					if (m.x == endPositions.at(c).x &&
					    m.y == endPositions.at(c).y &&
					    m.rotation == endPositions.at(c).rot) {
						isNew = false;
						break;
					}
				}

				if (isNew) {
					endPositions.push_back(EndPosition{ m.x, m.y, m.rotation });
					newEndPositions.push_back(EndPosition{ m.x, m.y, m.rotation });
				}
				
			}
		}
		//std::cout << "\nUnique End Positions " << newEndPositions.size() << ", Non unique: " << endPositions.size();

		for (size_t i = 0; i < newEndPositions.size(); ++i) {
			//allMoves.push_back(pathfindToEndPosition(gameState, mino, newEndPositions.at(i), 5));
		}


	}





	// Add sky moves to allMoves ////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < skyMoves.size(); ++i) {
		// add hard drop to the move
		if(!Globals::AI_USE_UNIQUE_MOVES) skyMoves.at(i).push_back(MOVE_HD);
		else skyMoves.at(i).at(skyMoves.at(i).size() - 1) = MOVE_HD;

		// add it to allMoves
		allMoves.push_back(skyMoves.at(i));
	}




	return allMoves;
}



// WIP
std::vector<int> ts::pathfindToEndPosition(GameState& gameState, int mino, EndPosition& position, int maxMoves) {

	// every possible action it can make
	static const int moves[5]{ MOVE_DD, MOVE_R, MOVE_L, ROT_CW, ROT_CCW };


	// create a tetromino at the starting position
	Tetromino startMino{ mino };







	return std::vector<int>();
}




int ts::executeMove(GameState& gameState, Tetromino& mino, const std::vector<int>& moves, int index, std::vector<int>& nextList) {
	switch (moves.at(index)) {
	case HOLD:
		gameState.performHold(gameState, mino, nextList);
		break;
	case MOVE_R:
		gameState.moveX(mino, 1);
		break;
	case MOVE_RR:
		gameState.arrX(mino, 1);
		break;
	case MOVE_LL:
		gameState.arrX(mino, -1);
		break;
	case MOVE_L:
		gameState.moveX(mino, -1);
		break;
	case ROT_CW:
		gameState.rotate(mino, 1);
		break;
	case ROT_CCW:
		gameState.rotate(mino, -1);
		break;
	case ROT_180:
		gameState.rotate(mino, 2);
		break;
	case MOVE_DD:
		gameState.hardDropWithoutPaste(mino);
		break;
	case MOVE_HD:
		gameState.hardDrop(mino);
		return gameState.lastPlacementAttack();
		break;

	}
	return 0;
}

int ts::fullyPerformMove(GameState& gameState, Tetromino& mino, const std::vector<int>& moves, std::vector<int>& nextList) {
	int clear = 0;
	for (size_t i = 0; i < moves.size(); ++i) {
		clear += executeMove(gameState, mino, moves, i, nextList);
	}
	return clear;
	
}
