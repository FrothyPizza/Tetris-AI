#pragma once
#include "GameState.h"
#include "SFML/Graphics.hpp"

#include "TetrisRender.h"

// TODO: make non-instant arr and non-instant soft drop options
class PlayerTetrisGame {

private:
	// stuff to handle time
	sf::Clock Rclock;
	bool RclockRestarted{ false };
	sf::Clock Lclock;
	bool LclockRestarted{ false };


public:
	int das{ ts::Globals::DAS };

	std::vector<int> nextList;
	ts::Tetromino curMino;
	ts::GameState gameState;

	sf::RectangleShape rect;

	PlayerTetrisGame();

	void render(sf::RenderWindow* window, sf::Vector2f position, float tileSize);

	// right and left inputs are separated from general because they need to be handled differently
	void inputRight(bool rightPressed);

	// right and left inputs are separated from general because they need to be handled differently
	void inputLeft(bool leftPressed);

	// soft drops all the way
	void inputDown();

	// any input not left or right
	// returns how many lines are sent, if any
	int inputGeneral(int keyCode);

	void recieveAttack(int attack);

};

