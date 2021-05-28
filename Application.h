#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>

#include "PlayerTetrisGame.h"
#include "TetrisAI.h"
#include "Population.h"
#include <future>


class Application {

private:

	sf::RenderWindow* m_window;
	sf::Event m_event;
	sf::VideoMode m_videoMode;

	float m_tileSize;
	sf::RectangleShape m_tile;

	PlayerTetrisGame playerGame;

	std::future<int> attackToPlayer;
	ts::TetrisAI tetrisAI;
	sf::Clock AIdelay{};

	//Population population;

	

	void initVars();
	void initWindow();


public:

	Application();
	virtual ~Application();

	const bool running() const;

	void pollEvents();
	void update();
	void render();

};

