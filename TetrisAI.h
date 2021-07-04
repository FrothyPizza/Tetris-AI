#pragma once



#pragma once

#include "AI.h"
#include <SFML/Graphics.hpp>


/*
Class TetrisAI

Stores a gamestate, nextlist, current tetromino,
*/
namespace ts {
	class TetrisAI {
	public:
		GameState gameState;
		Tetromino curMino;
		std::vector<int> nextList;

		AIFactor factors;

		std::vector<int> currentMove;

		int piecesPlaced;
		int totalKeysPressed;
		int totalAttack;

		// default, makes own next list
		TetrisAI(AIFactor factor);
		TetrisAI(bool inst);
		TetrisAI(std::vector<int> next, bool inst);

		// Does the next move. If instant is true, fully perform it at once.
		// If instant is false, perform a single one of the moves
		int doNextMove();
		void recieveAttack(int atk);

		void reset();


		bool isInstant() { return instant; }
		bool isDead() { return dead; }


		// rendering stuff
		sf::RectangleShape rect;
		void render(sf::RenderWindow* window, sf::Vector2f position, float tileSize);

		bool dead;
	private:
		bool instant;
		// Performs the next move in the list this is storing
		// return attack (if any)
		int performSingleMove();

		// Completely performs a MovementList
		// return attack (if any)
		int completelyPerformMove();

		// Generates the next move and stores it in the MovementList
		void generateNextMove();
	};

}