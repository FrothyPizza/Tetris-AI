#include "Globals.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include <SFML/System/Time.hpp>


int ts::Globals::DAS{ 80 };
int ts::Globals::ARR{ 0 };
int ts::Globals::SDF{ 0 };
int ts::Globals::AI_MOVE_DELAY_MS{ 100 };
int ts::Globals::AI_LOOKAHEADS{ 2 };
int ts::Globals::AI_BEST_MOVES_TO_BE_CONSIDERED{ 15 };
bool ts::Globals::HOLDENABLED{ true };
bool ts::Globals::AI_USE_UNIQUE_MOVES{ false };
bool ts::Globals::AI_FULLY_PERFORM_MOVE{ false };
bool ts::Globals::TURN_BASED{ false };

std::unordered_map<std::string, sf::Keyboard::Key> ts::Globals::ALL_KEYS;
std::unordered_map<std::string, sf::Keyboard::Key> ts::Globals::CONTROLS;
std::unordered_map<std::string, sf::SoundBuffer> ts::Globals::SOUND_BUFFERS;
std::unordered_map<std::string, sf::Sound> ts::Globals::SOUNDS;


void ts::Globals::InitVars() {
	InitAllKeys();

	SOUND_BUFFERS["hard_drop"].loadFromFile("Sounds/hard_drop.wav");
	SOUNDS["hard_drop"].setBuffer(SOUND_BUFFERS["hard_drop"]);
	SOUNDS["hard_drop"].setVolume(50.f);

	SOUND_BUFFERS["line_clear"].loadFromFile("Sounds/line_clear.wav");
	SOUNDS["line_clear"].setBuffer(SOUND_BUFFERS["line_clear"]);

	SOUND_BUFFERS["t_spin"].loadFromFile("Sounds/t_spin.wav");
	SOUNDS["t_spin"].setBuffer(SOUND_BUFFERS["t_spin"]);
	SOUNDS["t_spin"].setVolume(25.f);

    std::ifstream file("config.ini");
    if (file.is_open()) {
        std::string line;

        while (getline(file, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                line.end());

            if (line[0] == '[' || line.empty())
                continue;

            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            std::cout << name << " " << value << '\n';

            if (name == "sdf") SDF = std::stoi(value);
            if (name == "das") DAS = std::stoi(value);
            if (name == "arr") ARR = std::stoi(value);
            if (name == "input_delay") AI_MOVE_DELAY_MS = std::stoi(value);
            if (name == "lookaheads") AI_LOOKAHEADS = std::stoi(value);
            if (name == "lookahead_accuracy") AI_BEST_MOVES_TO_BE_CONSIDERED = std::stoi(value);
            if (name == "hold_enabled") HOLDENABLED = std::stoi(value);
            if (name == "use_unique_moves") AI_USE_UNIQUE_MOVES = std::stoi(value);
            if (name == "fully_perform_move") AI_FULLY_PERFORM_MOVE = std::stoi(value);
        }

    }
    else {
        std::cout << "Config file not opened";
    }

    if (AI_LOOKAHEADS == 0) {
        mainFactors = AIFactor{ -30, -20, -4, -5, -20.f, -10, -2, 0, 0, 0 };
    }

	std::cout << '\n';
	std::ifstream controlsFile("controls.ini");
	if (controlsFile.is_open()) {
		std::string line;

		while (getline(controlsFile, line)) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());

			if (line[0] == '[' || line.empty())
				continue;

			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			std::cout << name << " " << value << '\n';

			CONTROLS[name] = ALL_KEYS[value];


		}
		
	}
	else {
		std::cout << "Controls file not opened";
	}
}


void ts::outputFactors(AIFactor& factor) {
	std::cout << '\n'
		<< factor.attack << ", "
		<< factor.attackEffeciency << ", "
		<< factor.averageHeight << ", "
		<< factor.clearWithoutAttack << ", "
		<< factor.heightVariance << ", "
		<< factor.hole << ", "
		<< factor.holeCovered << ", "
		<< factor.Idependency << ", "
		<< factor.partialIdependency << ", "
		<< factor.surfaceParity << "\n";
}


void ts::Globals::InitAllKeys() {

#define INSERT_KEY_MAPPING(identifier) ALL_KEYS.insert({#identifier, sf::Keyboard::identifier })

	INSERT_KEY_MAPPING(Unknown);
	INSERT_KEY_MAPPING(A);
	INSERT_KEY_MAPPING(B);
	INSERT_KEY_MAPPING(C);
	INSERT_KEY_MAPPING(D);
	INSERT_KEY_MAPPING(E);
	INSERT_KEY_MAPPING(F);
	INSERT_KEY_MAPPING(G);
	INSERT_KEY_MAPPING(H);
	INSERT_KEY_MAPPING(I);
	INSERT_KEY_MAPPING(J);
	INSERT_KEY_MAPPING(K);
	INSERT_KEY_MAPPING(L);
	INSERT_KEY_MAPPING(M);
	INSERT_KEY_MAPPING(N);
	INSERT_KEY_MAPPING(O);
	INSERT_KEY_MAPPING(P);
	INSERT_KEY_MAPPING(Q);
	INSERT_KEY_MAPPING(R);
	INSERT_KEY_MAPPING(S);
	INSERT_KEY_MAPPING(T);
	INSERT_KEY_MAPPING(U);
	INSERT_KEY_MAPPING(V);
	INSERT_KEY_MAPPING(W);
	INSERT_KEY_MAPPING(X);
	INSERT_KEY_MAPPING(Y);
	INSERT_KEY_MAPPING(Z);
	INSERT_KEY_MAPPING(Num0);
	INSERT_KEY_MAPPING(Num1);
	INSERT_KEY_MAPPING(Num2);
	INSERT_KEY_MAPPING(Num3);
	INSERT_KEY_MAPPING(Num4);
	INSERT_KEY_MAPPING(Num5);
	INSERT_KEY_MAPPING(Num6);
	INSERT_KEY_MAPPING(Num7);
	INSERT_KEY_MAPPING(Num8);
	INSERT_KEY_MAPPING(Num9);
	INSERT_KEY_MAPPING(Escape);
	INSERT_KEY_MAPPING(LControl);
	INSERT_KEY_MAPPING(LShift);
	INSERT_KEY_MAPPING(LAlt);
	INSERT_KEY_MAPPING(LSystem);
	INSERT_KEY_MAPPING(RControl);
	INSERT_KEY_MAPPING(RShift);
	INSERT_KEY_MAPPING(RAlt);
	INSERT_KEY_MAPPING(RSystem);
	INSERT_KEY_MAPPING(Menu);
	INSERT_KEY_MAPPING(LBracket);
	INSERT_KEY_MAPPING(RBracket);
	INSERT_KEY_MAPPING(SemiColon);
	INSERT_KEY_MAPPING(Comma);
	INSERT_KEY_MAPPING(Period);
	INSERT_KEY_MAPPING(Quote);
	INSERT_KEY_MAPPING(Slash);
	INSERT_KEY_MAPPING(BackSlash);
	INSERT_KEY_MAPPING(Tilde);
	INSERT_KEY_MAPPING(Equal);
	INSERT_KEY_MAPPING(Dash);
	INSERT_KEY_MAPPING(Space);
	INSERT_KEY_MAPPING(Return);
	INSERT_KEY_MAPPING(BackSpace);
	INSERT_KEY_MAPPING(Tab);
	INSERT_KEY_MAPPING(PageUp);
	INSERT_KEY_MAPPING(PageDown);
	INSERT_KEY_MAPPING(End);
	INSERT_KEY_MAPPING(Home);
	INSERT_KEY_MAPPING(Insert);
	INSERT_KEY_MAPPING(Delete);
	INSERT_KEY_MAPPING(Add);
	INSERT_KEY_MAPPING(Subtract);
	INSERT_KEY_MAPPING(Multiply);
	INSERT_KEY_MAPPING(Divide);
	INSERT_KEY_MAPPING(Left);
	INSERT_KEY_MAPPING(Right);
	INSERT_KEY_MAPPING(Up);
	INSERT_KEY_MAPPING(Down);
	INSERT_KEY_MAPPING(Numpad0);
	INSERT_KEY_MAPPING(Numpad1);
	INSERT_KEY_MAPPING(Numpad2);
	INSERT_KEY_MAPPING(Numpad3);
	INSERT_KEY_MAPPING(Numpad4);
	INSERT_KEY_MAPPING(Numpad5);
	INSERT_KEY_MAPPING(Numpad6);
	INSERT_KEY_MAPPING(Numpad7);
	INSERT_KEY_MAPPING(Numpad8);
	INSERT_KEY_MAPPING(Numpad9);
	INSERT_KEY_MAPPING(F1);
	INSERT_KEY_MAPPING(F2);
	INSERT_KEY_MAPPING(F3);
	INSERT_KEY_MAPPING(F4);
	INSERT_KEY_MAPPING(F5);
	INSERT_KEY_MAPPING(F6);
	INSERT_KEY_MAPPING(F7);
	INSERT_KEY_MAPPING(F8);
	INSERT_KEY_MAPPING(F9);
	INSERT_KEY_MAPPING(F10);
	INSERT_KEY_MAPPING(F11);
	INSERT_KEY_MAPPING(F12);
	INSERT_KEY_MAPPING(F13);
	INSERT_KEY_MAPPING(F14);
	INSERT_KEY_MAPPING(F15);
	INSERT_KEY_MAPPING(Pause);

#undef INSERT_KEY_MAPPING
}