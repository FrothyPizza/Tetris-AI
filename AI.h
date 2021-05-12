#pragma once

#include "GameState.h"


namespace ts {
	


	enum moves {
		// hard drop
		MOVE_HD,                // 0
		// soft drop all the way
		MOVE_DD,                // 1
		// single right
		MOVE_R,                 // 2
		// single left
		MOVE_L,                 // 3
		// arr right all the way
		MOVE_RR,				// 4
		// arr left all the way
		MOVE_LL,				// 5
		// rotate CW
		ROT_CW,                 // 6
		// rotate CCW
		ROT_CCW,                // 7
		// rotate 180
		ROT_180,                // 8
		// hold
		HOLD                    // 9
	};



	float evaluate(const GameState& gameState, AIFactor& AIfactor);
	float evaluateAttackScore(AIFactor& AIfactor, int attack, int clear);


	struct EndPosition {
		int x;
		int y;
		int rot;
	};
	// returns a vector of MovementLists with every move
	std::vector<std::vector<int>> getAllMoves(GameState& gameState, int mino);

	// returns the shortest possible route to an end position
	// if no path is found within maxMoves amount of moves, return a vector with just -1 in it
	std::vector<int> pathfindToEndPosition(GameState& gameState, int mino, EndPosition& position, int maxMoves);



	// returns a vector of vector of moves containing the best move, when calling pass in depth
	std::vector<int> findBestMove(AIFactor& AIfactor, GameState& currentState, int minoIndex, std::vector<int>& nextList, float thisPathAttackScore, int depth);

	/*
	* Params:
	* * AIfactor: the weights the AI used
	* * currentState: the current GameState
	* * minoIndex: the number cooresponding to the current tetromino
	* * nextList: a vector of the next tetrominos
	* * thisPathTotalAttack: pass in 0, this is used in recursion
	* * depth: the number of previews it considers, 0 for no previews, 1 for one, etc.
	* 
	* Return:
	* *return: a vector of ints representing the move
	* *return: the score of the move (used for looking ahead with recursion)
	*/
	std::pair<std::vector<int>, float> getAIMove(
		AIFactor& AIfactor, GameState& currentState, int minoIndex,
		std::vector<int>& nextList, float thisPathTotalAttackScore, int depth
	);

	// moves the tetromino in the matrix based on the movement list
	// only moves for the index passed in so that the movement can be delayed
	// returns lines cleared
	int executeMove(GameState& gameState, Tetromino& mino, const std::vector<int>& moves, int index, std::vector<int>& nextList);

	// fully performs a movementlist, including hard drop
	// returns lines cleared
	int fullyPerformMove(GameState& gameState, Tetromino& mino, const std::vector<int>& moves, std::vector<int>& nextList);









}