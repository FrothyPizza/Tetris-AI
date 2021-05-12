#include "TetrisRender.h"

#include <iostream>


void setColor(sf::RectangleShape& rect, int tileIndex) {
	switch (tileIndex) {
	case 0:
		rect.setFillColor(sf::Color(227, 91, 2));
		break;
	case 1:
		rect.setFillColor(sf::Color(33, 65, 198));
		break;
	case 2:
		rect.setFillColor(sf::Color(215, 15, 55));
		break;
	case 3:
		rect.setFillColor(sf::Color(89, 177, 1));
		break;
	case 4:
		rect.setFillColor(sf::Color(175, 41, 138));
		break;
	case 5:
		rect.setFillColor(sf::Color(15, 155, 215));
		break;
	case 6:
		rect.setFillColor(sf::Color(227, 159, 2));
		break;
	case 7:
		rect.setFillColor(sf::Color(153, 153, 153));
		break;
	case 8:
		rect.setFillColor(sf::Color(200, 200, 200));
	}
}



void renderTetris(sf::RenderWindow* window, sf::Vector2f position, float tileSize, sf::RectangleShape& tile, 
				  ts::GameState& gameState, const ts::Tetromino& mino, std::vector<int>& nextList) {

	tile.setSize(sf::Vector2f(tileSize, tileSize));

	// Draw matrix
	// don't render the top two tiles
	for (int i = 0; i < ts::WIDTH; ++i) {
		for (int j = 0; j < ts::HEIGHT; ++j) {
			if (gameState.matrix[i][j] > -1) {
				tile.setPosition(sf::Vector2f(tileSize * i, tileSize * (j - ts::YMARGIN)) + position);
				setColor(tile, gameState.matrix[i][j]);
				window->draw(tile);
			}
		}
	}


	// Draw lines of the matrix
	sf::RectangleShape verticalLine{ sf::Vector2f(1.f, (ts::HEIGHT - ts::YMARGIN) * tileSize) };
	sf::RectangleShape horizontalLine{ sf::Vector2f((ts::WIDTH) * tileSize, 1.f) };
	verticalLine.setFillColor(sf::Color(255, 255, 255, 30));
	horizontalLine.setFillColor(sf::Color(255, 255, 255, 30));
	for (int i = 0; i < ts::WIDTH + 1; ++i) {
		if (i == 0 || i == ts::WIDTH) 	verticalLine.setFillColor(sf::Color(255, 255, 255));
		else 	verticalLine.setFillColor(sf::Color(255, 255, 255, 30));
		verticalLine.setPosition(sf::Vector2f(i * tileSize, 0) + position);
		window->draw(verticalLine);
	}
	for (int i = 0; i < ts::HEIGHT - ts::YMARGIN + 1; ++i) {
		if (i == 0 || i == ts::HEIGHT - ts::YMARGIN) horizontalLine.setFillColor(sf::Color(255, 255, 255, 255));
		else 	horizontalLine.setFillColor(sf::Color(255, 255, 255, 30));
		horizontalLine.setPosition(sf::Vector2f(0, i * tileSize) + position);
		window->draw(horizontalLine);
	}


	// Draw attack bar
	if (gameState.incomingGarbage.size() > 0) {
		int totatk = 0;
		for (unsigned int i = 0; i < gameState.incomingGarbage.size(); ++i) {
			totatk += gameState.incomingGarbage.at(i);
		}

		sf::RectangleShape garbageLine{ sf::Vector2f(3.f, totatk * tileSize) };
		garbageLine.setPosition(sf::Vector2f(0, static_cast<float>(ts::HEIGHT - totatk - ts::YMARGIN) * tileSize) + position);
		garbageLine.setFillColor(sf::Color::Red);
		window->draw(garbageLine);
	}



	// Draw ghost
	ts::Tetromino ghost{ mino };
	gameState.hardDropWithoutPaste(ghost);
	for (int i = 0; i < 4; ++i) {
		tile.setFillColor(sf::Color(125, 125, 125, 125));
		tile.setPosition(sf::Vector2f(tileSize * (ghost.x + ghost.data[i].x),
			tileSize * (ghost.y + ghost.data[i].y - ts::YMARGIN)) + position);
		window->draw(tile);
	}

	// Draw current tetromino
	for (int i = 0; i < 4; ++i) {
		setColor(tile, mino.mino);
		tile.setPosition(sf::Vector2f(tileSize * (mino.x + mino.data[i].x),
			tileSize * (mino.y + mino.data[i].y - ts::YMARGIN)) + position);
		window->draw(tile);
	}


	// Draw next list
	ts::Tetromino next{ NULL };
	for (int i = 0; i < 5; ++i) {
		next.setTetromino(nextList.at(i));
		next.x = ts::WIDTH + 3; next.y = ts::YMARGIN + i * 3;
		sf::Vector2f posModifier{};
		if (next.mino == ts::MINO_I || next.mino == ts::MINO_O) posModifier.x += 0.5;
		for (int i = 0; i < 4; ++i) {
			setColor(tile, next.mino);
			tile.setPosition(sf::Vector2f(tileSize * (next.x + next.data[i].x),
				tileSize * (next.y + next.data[i].y - ts::YMARGIN)) + position + posModifier * tileSize);
			window->draw(tile);
		}
	}

	// Draw hold
	if (gameState.hold >= 0 && gameState.hold < 7) {
		ts::Tetromino hold{ gameState.hold };
		hold.x = -3;
		hold.y = ts::YMARGIN + 1;
		for (int i = 0; i < 4; ++i) {
			setColor(tile, hold.mino);
			tile.setPosition(sf::Vector2f(tileSize * (hold.x + hold.data[i].x),
				tileSize * (hold.y + hold.data[i].y - ts::YMARGIN)) + position);
			window->draw(tile);
		}
	}




}
