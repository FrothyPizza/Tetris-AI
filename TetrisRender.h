#pragma once


#include <SFML/Graphics.hpp>
#include "GameState.h"


void renderTetris(sf::RenderWindow* window, sf::Vector2f position, float tileSize, sf::RectangleShape& tile
	, ts::GameState& gameState, const ts::Tetromino& mino, std::vector<int>& nextList);
