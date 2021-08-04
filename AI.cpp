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

			fullyPerformMove(allNewStates[i], mino, allMoves[i], newNext);
			float evaluation = evaluate(allNewStates[i], AIfactor);
			float atkScore = evaluateAttackScore(AIfactor, allNewStates[i].lastPlacementAttack(), allNewStates[i].lastPlacementClear(), allNewStates[i].getLastTSpinAfterHardDrop());
			evaluation += atkScore;

			attackEvaluations.push_back(atkScore);
			evaluations.push_back(evaluation);
		}


		if (depth == 0) { // if depth is 0 then it doesnt have to go through the trouble of sorting the evaluations and looking ahead
			for (size_t i = 0; i < evaluations.size(); ++i) {
				if (evaluations[i] + attackEvaluations[i] > bestScore) {
					bestMove = allMoves[i];
					bestScore = evaluations[i] + attackEvaluations[i];
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
						if (j == bestIndexes[c]) // if the index being considered has already been added, don't add it
							bestIndexesHasJ = true;
					//for (size_t c = 0; c < bestIndexes.size(); ++c)
					//	if (allNewStates[j) == allNewStates[bestIndexes[c))) // if the gamestate is a duplicate, don't consider it
					//		bestIndexesHasJ = true;
					if (evaluations[j] > bestEvaluation && bestIndexesHasJ == false) {
						bestEvaluation = evaluations[j];
						bestIndex = j;
					}
				}
				bestIndexes.push_back(bestIndex);

			}


			for (size_t i = 0; i < bestIndexes.size(); ++i) {
				GameState thisState{ allNewStates[bestIndexes[i]] };
				Tetromino thisMino{ nextList[0] };
				std::vector<int> newNext{ nextList.begin(), nextList.end() };
				std::vector<int> move = findBestMove(AIfactor, thisState, nextList[0], newNext, thisPathAttackScore + attackEvaluations[bestIndexes[i]], depth - 1);
				fullyPerformMove(thisState, thisMino, move, newNext);

				float evaluation = evaluate(thisState, AIfactor) + 
					evaluateAttackScore(AIfactor, thisState.lastPlacementAttack(), thisState.lastPlacementClear(), thisState.getLastTSpinAfterHardDrop());

				if (evaluation > bestScore) {
					bestScore = evaluation;
					bestMove = allMoves[bestIndexes[i]];
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

			fullyPerformMove(allNewStates[i], mino, allMovesWithHold[i], newNext);
			float evaluation = evaluate(allNewStates[i], AIfactor);
			float atkScore = evaluateAttackScore(
				AIfactor, allNewStates[i].lastPlacementAttack(), allNewStates[i].lastPlacementClear(), allNewStates[i].getLastTSpinAfterHardDrop()
			);
			evaluation += atkScore;

			attackEvaluations.push_back(atkScore);
			evaluations.push_back(evaluation);
		}


		if (depth == 0) {
			for (size_t i = 0; i < evaluations.size(); ++i) {
				if (evaluations[i] + attackEvaluations[i] > bestScore) {
					bestMove = std::vector<int>{ HOLD };
					bestScore = evaluations[i] + attackEvaluations[i];

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
						if (j == bestIndexes[c]) // if the index being considered has already been added, don't add it
							bestIndexesHasJ = true;
					//for (size_t c = 0; c < bestIndexes.size(); ++c)
					//	if (allNewStates[j) == allNewStates[bestIndexes[c))) // if the gamestate is a duplicate, don't consider it
					//		bestIndexesHasJ = true;
					if (evaluations[j] > bestEvaluation && bestIndexesHasJ == false) {
						bestEvaluation = evaluations[j];
						bestIndex = j;
					}
				}
				bestIndexes.push_back(bestIndex);

			}


			for (size_t i = 0; i < bestIndexes.size(); ++i) {
				GameState thisState{ allNewStates[bestIndexes[i]] };
				Tetromino thisMino{ holdNext[0] };
				std::vector<int> newNext{ holdNext.begin(), holdNext.end() };
				std::vector<int> move = findBestMove(AIfactor, thisState, holdNext[0], newNext, thisPathAttackScore + attackEvaluations[bestIndexes[i]], depth - 1);
				fullyPerformMove(thisState, thisMino, move, newNext);

				float evaluation = evaluate(thisState, AIfactor) + evaluateAttackScore(AIfactor, thisState.lastPlacementAttack(), thisState.lastPlacementClear(), thisState.getLastTSpinAfterHardDrop());;
				if (evaluation > bestScore) {
					bestScore = evaluation;
					//bestMove = allMovesWithHold[bestIndexes[i));
					bestMove.erase(bestMove.begin(), bestMove.end());
					bestMove.push_back(HOLD);
					i = bestIndexes.size(); // if holding is better, don't evaluate any more
				}
			}
		}

	}

	return bestMove;
	
	
}





// find the percent completion of a shape
// i.e. the T spin shape could be
// A O # O A
// A # # # A
// 1 1 # 1 1
// 
// where # is empty, 
// 1 is a block,
// A means empty or a block but are biased toward being a block,
// L means full all the way down to the left, R for the right,
// O can be used twice; it means one or the other, but not both, and not neither (xor)
//
// If any of the squares that should be empty are full, return 0 percent completion
float shapePercentCompletion(const ts::GameState& gameState, const std::vector<std::vector<char>>& shape, std::vector<ts::Point>& holePositions) {

	if (holePositions.size() == 0) return 0;

	// since 0,0 is in top left, this is kinda confusing
	int furthestDownY = 1;
	int furthestUpY = ts::HEIGHT-2;
	for (auto& h : holePositions) {
		if (h.y > furthestDownY) furthestDownY = h.y;
		if (h.y < furthestUpY) furthestUpY = h.y;
	}


	int filterLength{ (int)shape[0].size() };
	int filterHeight{ (int)shape.size() };

	// find all of the NECESSARY relative positions of the slide
	std::vector<ts::Point> emptyPositions; emptyPositions.reserve(6);
	std::vector<ts::Point> blockPositions; emptyPositions.reserve(6);
	std::vector<ts::Point> eitherPositions; eitherPositions.reserve(6);
	std::vector<ts::Point> xorPositions; xorPositions.reserve(6);


	for (size_t i = 0; i < shape.size(); ++i)
		for (size_t j = 0; j < shape[i].size(); ++j) {
			if (shape[i][j] == '#')
				emptyPositions.push_back({ (int)j, (int)i });
			if (shape[i][j] == '1')
				blockPositions.push_back({ (int)j, (int)i });
			if (shape[i][j] == 'A')
				eitherPositions.push_back({ (int)j, (int)i });
			if (shape[i][j] == 'O')
				xorPositions.push_back({ (int)j, (int)i });
		}

	int totalEithers = eitherPositions.size();
	int correctEithers = 0;
	ts::Point goodPosition{-1,-1};
	bool foundPosition{ false };
	
	for (int x = -1; x < ts::WIDTH; ++x) {
		//for (int y = furthestUpY-filterHeight; y < furthestDownY+filterHeight; ++y) {
		for (int y = 0; y < ts::HEIGHT+1; ++y) {
			// if any of the necessary empty positions are full, then move on b/c this is bad
			for (auto& empty : emptyPositions)
				if (gameState.matrix[x + empty.x][y + empty.y] != -1)
					goto NEXT_POS;
			// if any of the necessary block positions are empty, then move on b/c this is bad
			for (auto& block : blockPositions)
				if (x + block.x >= 0 && x + block.x < ts::WIDTH) // out of bounds counts as it being a block
					if (gameState.matrix[x + block.x][y + block.y] == -1)
						goto NEXT_POS;
			for (auto& either : eitherPositions)
				if (x + either.x >= 0 && x + either.x < ts::WIDTH) // out of bounds counts as it being a block
					if (gameState.matrix[x + either.x][y + either.y] != -1) ++correctEithers; // if it is a block, then increase the number of correct eithers
			
			foundPosition = true;
			goodPosition.x = x;
			goodPosition.y = y;
			//std::cout << goodPosition.x << " " << goodPosition.y << '\n';
		NEXT_POS:;
		}
	}
	if (goodPosition.x == -1 && goodPosition.y == -1) return 0;

	////std::vector<ts::Point> holesInLineWithShape{};
	//int holesInLine = 0;
	//for (size_t i = 0; i < holePositions.size(); ++i) {
	//	for (int h = goodPosition.x; h < goodPosition.x + filterHeight; ++h)
	//		if (holePositions[i].y == h && holePositions[i].x > goodPosition.x && holePositions[i].x < goodPosition.x + filterLength)
	//			//holesInLineWithShape.push_back(holePositions[i]);
	//			++holesInLine;
	//}
	//totalEithers += holesInLine * 5;


	std::vector<int> uniqueHoleYs;
	for (auto& hole : holePositions)
		if (hole.x < goodPosition.x || hole.x > goodPosition.x + filterLength 
			&& hole.y >= goodPosition.y && hole.y < goodPosition.y + filterHeight)
			if(std::find(uniqueHoleYs.begin(), uniqueHoleYs.end(), hole.y) != uniqueHoleYs.end()) 
				uniqueHoleYs.push_back(hole.y);
	if (uniqueHoleYs.size() == filterHeight) return 0;
	totalEithers *= uniqueHoleYs.size() + 1;
	//std::cout << "X: " << goodPosition.x << " Y: " << goodPosition.y << '\n';

	for (int x = goodPosition.x; x >= 0; --x) {
		for (int y = 1; y < filterHeight; ++y) {
			++totalEithers;
			if (gameState.matrix[x][y] != -1) ++correctEithers;
		}
	}

	for (int x = goodPosition.x + filterLength; x < ts::WIDTH; ++x) {
		for (int y = 1; y < filterHeight; ++y) {
			++totalEithers;
			if (gameState.matrix[x][y] != -1) ++correctEithers;
		}
	}

	bool firstXOR = (gameState.matrix[xorPositions[0].x + goodPosition.x][xorPositions[0].y + goodPosition.y] != -1);
	bool secondXOR = (gameState.matrix[xorPositions[1].x + goodPosition.x][xorPositions[1].y + goodPosition.y] != -1);
	if (firstXOR && secondXOR) return 0;
	if (firstXOR || secondXOR) // having an overhang is very good, so increase the return by a lot
		correctEithers += 3;
	if (!(firstXOR && secondXOR))
		totalEithers += 1;


	
	if(foundPosition) return (float)correctEithers / totalEithers;
	return 0.f;
}


float ts::evaluate(const GameState& gameState, AIFactor& AIfactor) {
	float score{ 0 };

	// if dead (need to work on this)
	for (int i = 3; i < 6; ++i)
		for (int j = 3; j < 5; ++j)
			if (gameState.matrix[i][j] >= 0) return -10000;


	//find holes
	std::vector<Point> holePositions;
	holePositions.reserve(10);
	int holeCount{ 0 };
	int coverdHoleCount{ 0 };
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT - 1; ++j) {
			if (gameState.matrix[i][j] >= 0 && gameState.matrix[i][j + 1] == -1) {
				holeCount++;
				// punish long holes but only up to 4 deep
				for (int c = 0; c < 4; ++c) {
					if (gameState.matrix[i][j + c] == -1 && gameState.matrix[i][j + c - 1] == -1) {
						++holeCount;
						holePositions.push_back({ (int)i, (int)j });
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
	int highestHeight{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		averageHeight += columnHeights[i];
		if ((columnHeights[i] > highestHeight)) highestHeight = columnHeights[i];
	}
	averageHeight /= columnHeights.size();
	score += averageHeight * AIfactor.averageHeight;
	score += highestHeight * AIfactor.highestHeight;
	if (averageHeight > 10) score += averageHeight * AIfactor.averageHeight;
	//if (highestHeight > 12) score += highestHeight * AIfactor.highestHeight * 2;

	// Calculate I dependencies and height variance
	int Idependencies{ 0 };
	int variance{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		int leftHeight{ 0 };
		int thisHeight{ columnHeights[i] };
		int rightHeight{ 0 };

		// if it isn't index 0 or last
		if (i >= 1 && i < columnHeights.size() - 1) {
			variance += abs(columnHeights[i - 1] - thisHeight);
			variance += abs(columnHeights[i + 1] - thisHeight);
		}

		// calculate I dependecies 
		if (i == 0) leftHeight = HEIGHT;
		else leftHeight = columnHeights[i - 1];

		if (i == columnHeights.size() - 1) rightHeight = HEIGHT;
		else rightHeight = columnHeights[i + 1];

		if (leftHeight - thisHeight >= 3 && rightHeight - thisHeight >= 3) Idependencies++;
	}
	score += Idependencies * AIfactor.Idependency;
	score += variance * AIfactor.heightVariance;

	

	// Calculate parity
	int parity{ 0 };
	for (size_t i = 0; i < columnHeights.size(); ++i) {
		int color = (columnHeights[i] % 2) ^ (i % 2);
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

	const static std::vector<std::vector<char>> Tshape{
	{'A', 'O', '#', 'O', 'A'},
	{'A', '#', '#', '#', 'A'},
	{'1', '1', '#', '1', '1'}
	};
	score += shapePercentCompletion(gameState, Tshape, holePositions) * AIfactor.tSpinSetup;


	return score;
}




float ts::evaluateAttackScore(AIFactor& AIfactor, int attack, int clear, int tSpin) {
	float score{ 0 };

	// total attack sent
	score += attack * attack * AIfactor.attack; // attack squared to encourage larger clears

	if (clear == 2 || clear == 3) score += AIfactor.clearedDoubleOrTriple;

	// clear without attack
	if (clear > 0 && attack == 0) {
		score += clear * AIfactor.clearWithoutAttack + AIfactor.clearedDoubleOrTriple;
	}

	// attack efficiency
	if (clear > 0 && attack > 0) {
		score += static_cast<float>(attack) / static_cast<float>(clear) * AIfactor.attackEffeciency;
	}

	if (tSpin == GameState::TSPIN)
		score += clear * AIfactor.performedTSpin + AIfactor.tSpinSetup; // since this destroys a setup, increase its value by the amount lost
	else if (tSpin == GameState::TSPIN_MINI)
		score += clear * AIfactor.performedTSpin / 3;


	//return 0; ///////////////////////////////////////////////////////////////////////////////////////////////
	return score;
}





std::vector<std::vector<int>> ts::getAllMoves(GameState& gameState, int mino) {



	std::vector<std::vector<int>> allMoves;
	allMoves.reserve(40);

	// all of the positions in the air before hard drop
	std::vector<std::vector<int>> skyMoves;
	skyMoves.reserve(30);
	

	if (mino == MINO_L || mino == MINO_J || mino == MINO_T)
		skyMoves = ljtMoves;
	if (mino == MINO_Z || mino == MINO_S)
		skyMoves = szMoves;
	if (mino == MINO_I)
		skyMoves = iMoves; 
	if (mino == MINO_O) 
		skyMoves = oMoves;



	//// Potential unique moves solution plan ////
	// 1. find all holes
	// 2. for each hole, try placing the tetromino in the hole
	// 3. if the tetromino fits in that spot and it can't move up (no sky access), then it is a new unique move
	// 4. pathfind to the unique spot(s)

	if (Globals::AI_USE_UNIQUE_MOVES) {

		std::vector<Point> holes; holes.reserve(5);
		for (int i = 0; i < WIDTH; ++i) {
			for (int j = 0; j < HEIGHT - 1; ++j) {
				if (gameState.matrix[i][j] >= 0 && gameState.matrix[i][j + 1] == -1)
					holes.push_back({ i, j });

			}
		}


	}
	



	//// Find unique positions /////////////////////////////////////////////////////////////
	// (doesn't work)
	if (Globals::AI_USE_UNIQUE_MOVES) {

		Tetromino m{ mino };

		int possibleMinoRotations{ 2 };
		if (mino == MINO_I || mino == MINO_S || mino == MINO_Z)
			possibleMinoRotations = 2;
		else if (mino == MINO_O)
			possibleMinoRotations = 1;
		else 
			possibleMinoRotations = 4;

		std::vector<int> rotationActions;
		if (possibleMinoRotations == 2)
			rotationActions = std::vector<int>{ 0, 1 };
		if (possibleMinoRotations == 1)
			rotationActions = std::vector<int>{ 0 };
		if (possibleMinoRotations == 4)
			rotationActions = std::vector<int>{ 0, 1, 2, -1 };

		std::vector<int> fakeNext{ }; // fake next list so the fullyPerformMove call is valid
										// this is kinda hackey, but whatever

		// endPositions is all ending positions of the tetromino for each move
		std::vector<EndPosition> endPositions;
		endPositions.reserve(skyMoves.size());

		int highestY{ HEIGHT }; // start at bottom
		int lowestY{ 0 }; // the higher the number, the lower the elevation
		// add hard drop without pasting to the moves
		// and then perfrom the move so that the end position each move is known
		for (size_t i = 0; i < skyMoves.size(); ++i) {
			skyMoves[i].push_back(MOVE_DD);

			// reset the mino
			m.setTetromino(mino);

			// perform the move
			fullyPerformMove(gameState, m, skyMoves[i], fakeNext);

			// add this endpostion to the vector
			endPositions.push_back(EndPosition{ m.x, m.y, m.rotation });
			//std::cout << m.rotation;

			if (m.y < highestY) highestY = m.y;
			if (m.y > lowestY) lowestY = m.y;
		}

		//for (size_t i = 0; i < endPositions.size(); ++i) { /////////////////////////////////////////////////////////////////////////////////////////
			//std::cout << endPositions[i].x << " " << endPositions[i].y << " " << endPositions[i].rot << '\n';
		//}

		std::vector<EndPosition> newEndPositions;
		newEndPositions.reserve(5);
		for (int i = 0; i < WIDTH; ++i) {
			//for (int j = highestY; j < lowestY; ++j) {
			for (int j = 0; j < HEIGHT; ++j) {

				for (int r = 0; r < possibleMinoRotations; ++r) {
					m.setTetromino(mino);

					if(rotationActions[r] != 0) gameState.rotate(m, rotationActions[r]);

					m.x = i;
					m.y = j;

					if (gameState.matrixContains(m)) continue;
					gameState.hardDropWithoutPaste(m);

					bool isNew{ true };
					for (size_t c = 0; c < endPositions.size(); ++c) {
						// if it is the same as an already existing end position

						bool sameRot = false;
						if (mino == MINO_I || mino == MINO_S || mino == MINO_Z)
							if ((m.rotation == 3 || m.rotation == 1) && (endPositions[c].rot == 3 || endPositions[c].rot == 1))
								sameRot = true;
						if (m.x == endPositions[c].x &&
							m.y == endPositions[c].y &&
							(m.rotation == endPositions[c].rot || sameRot)) {
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
		}
		//std::cout << "\nUnique End Positions " << newEndPositions.size() << ", Non unique: " << endPositions.size();

		for (size_t i = 0; i < endPositions.size(); ++i) { /////////////////////////////////////////////////////////////////////////////////////////
			std::cout << endPositions[i].x << " " << endPositions[i].y << " " << endPositions[i].rot << '\n';
		}
		std::cout << '\n';
		for (size_t i = 0; i < newEndPositions.size(); ++i) {
			std::cout << newEndPositions[i].x << " " << newEndPositions[i].y << " " << newEndPositions[i].rot << '\n'; ////////////////////////
			std::vector<int> move = pathfindToEndPosition(gameState, mino, newEndPositions[i], 5);
			if(move.size() != 0)
				allMoves.push_back(move);
		}



	}





	// Add sky moves to allMoves ////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < skyMoves.size(); ++i) {
		// add hard drop to the move
		if(!Globals::AI_USE_UNIQUE_MOVES) skyMoves[i].push_back(MOVE_HD);
		else skyMoves[i][skyMoves[i].size() - 1] = MOVE_HD;

		// add it to allMoves
		allMoves.push_back(skyMoves[i]);
	}




	return allMoves;
}



// WIP
std::vector<int> ts::pathfindToEndPosition(GameState& gameState, int mino, EndPosition& position, int maxMoves) {

	// every possible action it can make
	static const int moves[5]{ MOVE_DD, MOVE_R, MOVE_L, ROT_CW, ROT_CCW };


	


	// Brute force because why not
	// 
	Tetromino startMino{ mino };

	std::vector<std::vector<int>> allMoves;

	for (int a = 0; a < 5; ++a)
		for(int b = 0; b < 5; ++b)
			for (int c = 0; c < 5; ++c)
				for (int d = 0; d < 5; ++d)
					for (int e = 0; e < 5; ++e)
						for (int f = 0; f < 5; ++f)
							allMoves.push_back({ moves[a],moves[b],moves[c],moves[d],moves[e],moves[f],MOVE_DD });


	std::vector<int> fakeNext{};
	for (auto& i : allMoves) {
		Tetromino mino{ startMino };
		fullyPerformMove(gameState, mino, i, fakeNext);
		if (mino.x == position.x && mino.y == position.y && mino.rotation == position.rot) {
			i[i.size() - 1] = MOVE_HD;
			return i;
		}
	}




	return std::vector<int>{};
}




int ts::executeMove(GameState& gameState, Tetromino& mino, const std::vector<int>& moves, int index, std::vector<int>& nextList) {
	switch (moves[index]) {
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









// This doesn't work, ignore
std::pair<std::vector<int>, float> ts::getAIMove(AIFactor& AIfactor, GameState& currentState, int minoIndex, std::vector<int>& nextList, float thisPathTotalAttackScore, int depth) {

	std::pair<std::vector<int>, float> bestMove{};

	// generate all possible moves
	std::vector<std::vector<int>> allMoves{ getAllMoves(currentState, minoIndex) };

	// for each move, execute it and lookahead
	for (size_t i = 0; i < allMoves.size(); ++i) {
		GameState newState{ currentState };
		Tetromino mino{ minoIndex };
		std::vector<int> newNext{ nextList };

		fullyPerformMove(newState, mino, allMoves[i], newNext);

		float evaluation = evaluate(newState, AIfactor);
		float atkScore = evaluateAttackScore(AIfactor, newState.lastPlacementAttack(), newState.lastPlacementClear(), newState.getLastTSpinAfterHardDrop());
		
		float overallScore;
		if(depth > 0)
			overallScore = getAIMove(AIfactor, newState, nextList[0], newNext, atkScore + thisPathTotalAttackScore, depth - 1).second;
		else {
			overallScore = evaluation + atkScore;
		}
		if (overallScore > bestMove.second) {
			bestMove = std::pair<std::vector<int>, float>{ allMoves[i], overallScore };
		}
	}



	return bestMove;
}