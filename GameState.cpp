#include "GameState.h"


namespace ts {

	void pushOntoNextlist(std::vector<int>& next) {
		std::array<int, 7> nums = { 0, 1, 2, 3, 4, 5, 6 };
		std::random_shuffle(nums.begin(), nums.end());
		next.reserve(7);
		for (unsigned i = 0; i < nums.size(); ++i) {
			next.push_back(nums[i]);
		}
	}


	bool operator==(const GameState& state1, const GameState& state2) {
		return (state1.matrix == state2.matrix &&
			state1.canHold == state2.canHold &&
			state1.combo == state2.combo &&
			state1.hold == state2.hold
			);
	}



	GameState::GameState() :
		hold{ -1 },
		canHold{ true },
		combo{ 0 },
		lastMoveWasKick{ false },
		totalLinesCleared{ 0 },
		totalLinesSent{ 0 },
		b2b{ 0 }
	{
		incomingGarbage.reserve(20);
		memset(matrix, -1, sizeof(int)* WIDTH* HEIGHT);

		//for (int i = 0; i < WIDTH; ++i) {
		//	matrix.push_back(std::vector<int>{});
		//	for (int j = 0; j < HEIGHT; ++j)
		//		matrix[i].push_back(-1);
		//}
	}


	// returns whether it could and also performs the hold
	bool GameState::performHold(GameState& state, Tetromino& mino, std::vector<int>& next) {
		if (canHold) {
			int newHold = mino.mino;
			if (!(state.hold >= 0 && state.hold < 7)) {
				mino.setTetromino(next.front());
				next.erase(next.begin());
			}
			else {
				mino.setTetromino(state.hold);
			}
			state.hold = newHold;
			canHold = false;
			return true;
		}
		return false;

	}


	// check if a tetromino is inside of a block of the matrix
	bool GameState::matrixContains(const Tetromino& mino) {
		bool contains = false;

		// check the coordinates of each tile of the tetromino
		// if the cooresponding tile of the Matrix is a block, return true
		for (int i = 0; i < 4; ++i) {
			int x = mino.data[i].x + mino.x;
			int y = mino.data[i].y + mino.y;


			if (x >= WIDTH || x < 0 || y >= HEIGHT) // check if it's out of bounds
				contains = true;
			else if (y >= 0)
				if (matrix[x][y] >= 0)
					contains = true;

		}
		return contains;
	}


	// returns if it succeded
	bool GameState::softDrop(Tetromino& mino) {
		mino.y++;
		if (matrixContains(mino)) {
			mino.y--;
			return false;
		}
		else {
			lastMoveWasRot = false;
			lastMoveWasKick = false;
			return true;
		}
	}

	// moves the tetromino down as far as it can go an then pastes it to the matrix
	// returns how far it moved down
	int GameState::hardDrop(Tetromino& mino) {
		canHold = true;
		for (int i = 0; i < HEIGHT+10; i++) {
			mino.y++;
			if (matrixContains(mino)) {
				// if the hard drop moved the tetromino, the last move wasn't a kick
				if (i != 0) lastMoveWasKick = false;
				mino.y--;
				lastTSpin = isTSpin(mino);
				pasteToMatrix(mino);

				lastClear = clearLines();
				lastAttack = getAttack(lastTSpin, mino.mino, lastClear);

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//std::cout << "\nmino pos: " << mino.x << " " << mino.y << " " << mino.rotation << '\n';


				// handle back to back
				if (lastClear == 4 || lastTSpin != NO_TSPIN && lastClear > 0)
					b2b++;
				else
					if (lastClear > 0) b2b = 0;

				if (lastClear == 0) lastAttack = 0;
				return i;
				break;
			}
		}
		return 0;
	}


	int GameState::clearLines() {
		int c;
		int lines{ 0 };
		for (int i = 0; i < HEIGHT; ++i) {
			c = 0;
			for (int j = 0; j < WIDTH; ++j) {
				if (matrix[j][i] > -1) c++;
			}

			if (c == WIDTH) {
				lines++;
				// Move the rest down
				for (int y = i; y >= 0; --y) {
					for (int x = 0; x < WIDTH; ++x) {
						if (y - 1 < 0) matrix[x][y] = -1;
						else matrix[x][y] = matrix[x][y - 1];

					}
				}
			}
		}
		if (lines > 0) combo++;
		else combo = 0;
		return lines;
	}



	void GameState::hardDropWithoutPaste(Tetromino& mino) {
		for (int i = 0; i < 30; i++) {
			mino.y++;
			if (matrixContains(mino)) {
				mino.y--;
				if(i > 0) lastMoveWasRot = false;
				break;
			}
		}
	}


	int GameState::lastPlacementAttack() {
		return lastAttack;
	}

	int GameState::lastPlacementClear() {
		return lastClear;
	}

	void GameState::pasteToMatrix(const Tetromino& mino) {
		for (int i = 0; i < 4; ++i) {
			int x = mino.data[i].x + mino.x;
			int y = mino.data[i].y + mino.y;
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
				matrix[x][y] = mino.mino;
			}
		}
	}

	// 0 doesn't move it at all
	// if the movement fails, it moves it back to where it was
	bool GameState::moveX(Tetromino& mino, int x) {
		mino.x += x;
		if (matrixContains(mino)) {
			mino.x -= x;
			return false;
		}
		else {
			lastMoveWasKick = false;
			lastMoveWasRot = false;
			return true;
		}
	}

	void GameState::arrX(Tetromino& mino, int x) {
		for (int i = 0; i < WIDTH; ++i) {
			if(!moveX(mino, x)) return; // as soon as it fails, stop trying to move it
		}
	}

	// 0 doesn't rotate, 1 rotates right, -1 rotates left
	// 2 rotates 180 degrees
	void GameState::rotate(Tetromino& mino, int dir) {
		// don't rotate if it's O
		if (mino.mino == MINO_O) return;

		Tetromino oldMino{ mino };

		// if it's an i piece then use the precomputed rotations
		if (mino.mino == MINO_I) {
			int rot = mino.rotation;


			if (dir == 2) {
				if (rot == 0) rot = 2;
				if (rot == 1) rot = 3;
				if (rot == 2) rot = 0;
				if (rot == 3) rot = 1;
			}
			else {
				rot += dir;
			}

			if (rot < 0) rot = 3;
			if (rot > 3) rot = 0;

			for (int i = 0; i < 4; ++i) {
				mino.data[i].x = IrotationData[rot][i].x;
				mino.data[i].y = IrotationData[rot][i].y;
			}
		}
		else { // if it isn't an I piece
			if (dir == 1) {
				for (int i = 0; i < 4; ++i) {
					int x = mino.data[i].x; int y = mino.data[i].y;
					mino.data[i].x = y * -1;
					mino.data[i].y = x;
				}
			}
			if (dir == -1) {
				for (int i = 0; i < 4; ++i) {
					int x = mino.data[i].x; int y = mino.data[i].y;
					mino.data[i].x = y;
					mino.data[i].y = x * -1;
				}
			}
			if (dir == 2) {
				for (int i = 0; i < 4; ++i) {
					int x = mino.data[i].x; int y = mino.data[i].y;
					mino.data[i].x = y;
					mino.data[i].y = x * -1;
				}
				for (int i = 0; i < 4; ++i) {
					int x = mino.data[i].x; int y = mino.data[i].y;
					mino.data[i].x = y;
					mino.data[i].y = x * -1;
				}
			}
		}

		bool kickSuccess = false;
		if (matrixContains(mino)) kickSuccess = wallKick(mino, dir); // if the rotation fails, then attempt a kick
		else lastMoveWasRot = true; // the direction doesn't matter
		if (matrixContains(mino) && !kickSuccess) { // if the kick doesn't work, then reset the mino
			for (int i = 0; i < 4; ++i) {
				mino.data[i].x = oldMino.data[i].x;
				mino.data[i].y = oldMino.data[i].y;
			}
		}
		else {
			lastMoveWasRot = true;
			if (kickSuccess) lastMoveWasKick = true;
			if (dir == 2) {
				switch (mino.rotation) {
				case 0:
					mino.rotation = 2;
					break;
				case 1:
					mino.rotation = 3;
					break;
				case 2:
					mino.rotation = 0;
					break;
				case 3:
					mino.rotation = 1;
					break;
				}
				//if (mino.rotation == 0) mino.rotation = 2;
				//if (mino.rotation == 1) mino.rotation = 3;
				//if (mino.rotation == 2) mino.rotation = 0;
				//if (mino.rotation == 3) mino.rotation = 1;
			}
			else {
				mino.rotation += dir;
			}
			if (mino.rotation < 0) mino.rotation = 3;
			if (mino.rotation > 3) mino.rotation = 0;
		}
	}

	// this doesn't have to be very efficient since the AI rarely uses kicks
	bool GameState::wallKick(Tetromino& mino, int dir) {
		int newRotation = mino.rotation;

		newRotation += dir;
		if (newRotation < 0) newRotation = 3;
		if (newRotation > 3) newRotation = 0;


		int storeX = mino.x; int storeY = mino.y;

		// if it isn't O or I
		if (mino.mino < 5) {

			//// get kick data
			//const Point kicks[5]{
			//	{wallKickData[mino.rotation][0].x - wallKickData[newRotation][0].x, wallKickData[mino.rotation][0].y - wallKickData[newRotation][0].y},
			//	{wallKickData[mino.rotation][1].x - wallKickData[newRotation][1].x, wallKickData[mino.rotation][1].y - wallKickData[newRotation][1].y},
			//	{wallKickData[mino.rotation][2].x - wallKickData[newRotation][2].x, wallKickData[mino.rotation][2].y - wallKickData[newRotation][2].y},
			//	{wallKickData[mino.rotation][3].x - wallKickData[newRotation][3].x, wallKickData[mino.rotation][3].y - wallKickData[newRotation][3].y},
			//	{wallKickData[mino.rotation][4].x - wallKickData[newRotation][4].x, wallKickData[mino.rotation][4].y - wallKickData[newRotation][4].y}
			//};

			//for (int i = 0; i < 5; ++i) {
			//	mino.x += kicks[i].x;
			//	mino.y += kicks[i].y;

			//	if (!matrixContains(mino)) {
			//		return true;
			//	}
			//	else {
			//		mino.x = storeX;
			//		mino.y = storeY;
			//	}
			//}



			for (int i = 0; i < 5; ++i) {
				//mino.x += IwallKickData[mino.rotation][newRotation][i].x;
				//mino.y += IwallKickData[mino.rotation][newRotation][i].y;
				int nd = 0;
				if (dir == 1) nd = 0;
				if (dir == -1) nd = 1;
				
				mino.x += ZSTLJwallKickData[mino.rotation][nd][i].x;
				mino.y += ZSTLJwallKickData[mino.rotation][nd][i].y;
				if (!matrixContains(mino)) {
					return true;
				}
				else {
					mino.x = storeX;
					mino.y = storeY;
				}
			}
			return false;
		}
		else {

			for (int i = 0; i < 5; ++i) {
				//mino.x += IwallKickData[mino.rotation][newRotation][i].x;
				//mino.y += IwallKickData[mino.rotation][newRotation][i].y;
				int nd = 0;
				if (dir == 1) nd = 0;
				if (dir == -1) nd = 1;

				mino.x += IwallKickData[mino.rotation][nd][i].x;
				mino.y += IwallKickData[mino.rotation][nd][i].y;
				if (!matrixContains(mino)) {
					return true;
				}
				else {
					mino.x = storeX;
					mino.y = storeY;
				}
			}
			return false;
		}

		return false;
	}

	void GameState::placeGarbage() {
		if (incomingGarbage.size() == 0) return;
		for (unsigned int i = 0; i < incomingGarbage.size(); ++i) {
			if (incomingGarbage.at(i) > 0) {
				int holeIndex = rand() % WIDTH;
				// move everything up
				for (int w = 0; w < WIDTH; ++w) {
					for (int h = 0; h < HEIGHT; h++) {
						if (matrix[w][h] >= -1) {
							if (h - incomingGarbage.at(i) >= 0) matrix[w][h - incomingGarbage.at(i)] = matrix[w][h];
							matrix[w][h] = -1;
						}

					}
				}
				// place garbage
				for (int w = 0; w < WIDTH; ++w) {
					for (int h = 0; h < incomingGarbage.at(i); h++) {
						if (w != holeIndex) {
							if (HEIGHT - h - 1 >= 0) matrix[w][HEIGHT - h - 1] = 7;
						}
						else {
							if (HEIGHT - h - 1 >= 0) matrix[w][HEIGHT - h - 1] = -1;
						}
					}
				}
			}
		}
		incomingGarbage.erase(incomingGarbage.begin(), incomingGarbage.end());
	}

	void GameState::recieveAttack(int atk) {
		if (atk > 0) incomingGarbage.push_back(atk);
	}

	void GameState::cancelGarbage(int& atk) {
		if (atk == 0) return;
		if (incomingGarbage.size() == 0) return;

		for (unsigned int i = 0; i < incomingGarbage.size(); ++i) {
			int startGarbage = incomingGarbage.at(i);
			incomingGarbage.at(i) -= atk;
			if (incomingGarbage.at(i) >= 0) {
				atk = 0;
				return;
			}
			else {
				incomingGarbage.at(i) = 0;
				atk -= startGarbage;
			}

		}

	}

	void GameState::resetMatrix() {
		for (int i = 0; i < WIDTH; ++i)
			for (int j = 0; j < HEIGHT; ++j)
				matrix[i][j] = -1;

	}



	int GameState::getAttack(int wasTSpin, int minoIndex, int cleared) {
		if (cleared == 0) return 0;

		int attack = 0;

		if (cleared == 2 || cleared == 3) attack = cleared - 1; // 1 attack for 2 lines, 2 for 3 lines

		if (minoIndex == MINO_T) {
			if (wasTSpin == TSPIN_MINI) {
				if (cleared == 1) attack = 1; // mini tspin
				else if (cleared == 2) attack = 4; // mini tspin double
				
			}
			if (wasTSpin == TSPIN) {
				if (cleared == 1) attack = 2; // tspin single
				if (cleared == 2) attack = 4; // tspin double
				if (cleared == 3) attack = 6; // tspin triple

			}
		}

		//if it was a Tetris, attack = 4
		if (cleared == 4)
			attack = 4;

		if (b2b > 0 && (cleared == 4 || wasTSpin != NO_TSPIN)) attack += 1;

		// add combo
		if (combo >= 13) attack += 5;
		else attack += comboTable[combo];

		return attack;
	}
	// pass in the tetromino where it was before hard drop, and then test it int the matrix before hard drop
	int GameState::isTSpin(Tetromino& mino) {
		if (mino.mino != MINO_T) return NO_TSPIN;
		if (!lastMoveWasRot) return NO_TSPIN;

		int cornersFilled = 0;
		// C T C
		// T T T
		// C 0 C
		int mainCellsFilled = 0; // main cells filled are the cells adjacent to the t's penis
		// M T M
		// T T T
		// 0 0 0
		Tetromino corner{ mino };
		for (int i = 0; i < 4; ++i) {
			corner.data[i].x = 0;
			corner.data[i].y = 0;
		}

		corner.data[0].x = -1;
		corner.data[0].y = -1;
		if (matrixContains(corner)) {
			cornersFilled++;
			if (mino.rotation == 3 || mino.rotation == 0) mainCellsFilled++;
		}

		corner.data[0].x = -1;
		corner.data[0].y = 1;
		if (matrixContains(corner)) {
			cornersFilled++;
			if (mino.rotation == 3 || mino.rotation == 2) mainCellsFilled++;
		}

		corner.data[0].x = 1;
		corner.data[0].y = -1;
		if (matrixContains(corner)) {
			cornersFilled++;
			if (mino.rotation == 0 || mino.rotation == 1) mainCellsFilled++;
		}

		corner.data[0].x = 1;
		corner.data[0].y = 1;
		if (matrixContains(corner)) {
			cornersFilled++;
			if (mino.rotation == 1 || mino.rotation == 2) mainCellsFilled++;
		}

		if (cornersFilled >= 3 && mainCellsFilled >= 2) return TSPIN;
		else if (cornersFilled >= 3) return TSPIN_MINI; // mini t spin doesnt need main cells filled
		return NO_TSPIN;

	}




}