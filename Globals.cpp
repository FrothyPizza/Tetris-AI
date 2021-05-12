#include "Globals.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

int ts::Globals::DAS{ 80 };
int ts::Globals::AI_MOVE_DELAY_MS{ 100 };
int ts::Globals::AI_LOOKAHEADS{ 2 };
int ts::Globals::AI_BEST_MOVES_TO_BE_CONSIDERED{ 15 };
bool ts::Globals::HOLDENABLED{ true };
bool ts::Globals::AI_USE_UNIQUE_MOVES{ false };
bool ts::Globals::AI_FULLY_PERFORM_MOVE{ false };
bool ts::Globals::TURN_BASED{ false };

void ts::Globals::InitVars() {

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

            if (name == "das") DAS = std::stoi(value);
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
}
