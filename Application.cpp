#include "Application.h"



void Application::initVars() {
	m_window = nullptr;

	AIdelay.restart();
	attackToPlayer = std::async(&ts::TetrisAI::doNextMove, &tetrisAI);

	ts::Globals::InitVars();
}

void Application::initWindow() {
	m_videoMode.width = 1100;
	m_videoMode.height = 800;
	
	m_window = new sf::RenderWindow(m_videoMode, "Tetris", sf::Style::Titlebar | sf::Style::Close);
}

Application::Application() : playerGame{}, tetrisAI{ ts::Globals::AI_FULLY_PERFORM_MOVE } {
	initVars();
	initWindow();


}

Application::~Application() {
	delete m_window;
}

const bool Application::running() const {
	return m_window->isOpen();
}

void Application::pollEvents() {
	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			break;

		case sf::Event::KeyPressed:
			if (ts::Globals::TURN_BASED) {
				if (attackToPlayer.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
					int attack = attackToPlayer.get();

					tetrisAI.gameState.cancelGarbage(attack);
					playerGame.recieveAttack(attack); // send attack

					attackToPlayer = std::async(&ts::TetrisAI::doNextMove, &tetrisAI);
					AIdelay.restart();

				}
			}

			int attack = playerGame.inputGeneral(m_event.key.code);
			playerGame.gameState.cancelGarbage(attack);
			tetrisAI.recieveAttack(attack);
			switch (m_event.key.code) {
				case sf::Keyboard::Escape:
					m_window->close();
					break;
				case sf::Keyboard::Q:
					tetrisAI.reset();
					break;


			}

			break;
		}

	}
}

void Application::update() {
	pollEvents();

	if (!ts::Globals::TURN_BASED) { // if it isn't turn based, go on a fixed timer 
		if (AIdelay.getElapsedTime().asMilliseconds() >= ts::Globals::AI_MOVE_DELAY_MS) {

			//attackToPlayer = tetrisAI.doNextMove();
			if (attackToPlayer.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
				int attack = attackToPlayer.get();

				tetrisAI.gameState.cancelGarbage(attack);
				playerGame.recieveAttack(attack); // send attack

				attackToPlayer = std::async(&ts::TetrisAI::doNextMove, &tetrisAI);
				AIdelay.restart();

			}


		}
	}

	playerGame.inputRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
	playerGame.inputLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		playerGame.inputDown();
	}
}

void Application::render() {
	// Clear
	m_window->clear(sf::Color::Black);


	// Render
	playerGame.render(m_window, sf::Vector2f(130, 100), 25);
	tetrisAI.render(m_window, sf::Vector2f(690, 100), 25);


	// Display
	m_window->display();

}