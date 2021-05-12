#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <vector>

#include "Tetromino.h"
#include "Globals.h"


namespace ts {

	void pushOntoNextlist(std::vector<int>& next);


	struct GameState {

		int matrix[WIDTH][HEIGHT];
		std::vector<int> incomingGarbage;

		int hold;
		bool canHold;

		// total combo. if piece placed without lines cleared, combo is set to 0
		int combo;

		// the number of consecutive tspins/tetrisis without any line clears inbetween
		int b2b;

		// stores what the previous move was before hard drop
		int lastMove;
		// used to detect if piece placed was a t
		bool lastMoveWasKick;

		// overall lines sent since creation
		int totalLinesSent;
		// total lines cleared since creation
		int totalLinesCleared;

		GameState();

		bool performHold(GameState& state, Tetromino& mino, std::vector<int>& next);

		bool matrixContains(const Tetromino& mino);

		// moves down one
		// retruns whether it succeeded
		bool softDrop(Tetromino& mino);

		// moves tetromino all the way down and pastes it to the matrix
		int  hardDrop(Tetromino& mino);

		// returns lines cleared
		// must manually call
		int clearLines();


		void hardDropWithoutPaste(Tetromino& mino);
		
		// moves specified amount
		// -1 for left, 1 for right
		// return whether it succeeded
		bool moveX(Tetromino& mino, int x);

		// if -1, move all the way left
		// if 1, move all the way right
		void arrX(Tetromino& mino, int x);

		// rotates in specified direction
		// 1 for clockwise
		// -1 for counterclockwise
		void rotate(Tetromino& mino, int dir);

		// must manually call
		void placeGarbage();

		void recieveAttack(int atk);

		// decreases incoming garbage by atk
		void cancelGarbage(int& atk);

		// resets the matrix to all -1s
		void resetMatrix();

		// returns the attack
		int getAttack(int wasTSpin, int minoIndex, int cleared);

		enum tSpinTypes {
			NO_TSPIN, // not a tspin
			TSPIN_MINI,
			TSPIN
		};
		
		// returns if it was a tspin based off of if the last move was a rotation or kick and the position of the mino
		int isTSpin(Tetromino& mino);

		friend bool operator== (const GameState& state1, const GameState& state2);

	private:
		void pasteToMatrix(const Tetromino& mino);

		bool wallKick(Tetromino& mino, int dir);

	};

}