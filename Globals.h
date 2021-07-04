#pragma once
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace ts {

	struct AIFactor {
		float hole;			         // how bad holes are
		float holeCovered;           // how many blocks are above a hole (bad)
		float heightVariance;        // average height differance between adjacent columns
		float averageHeight;         // average height
		float Idependency;           // number of columns with 2 adjacent columns 3 higher than the one
		float partialIdependency;    // number of columns with 2 adjacent columns >=2 higher than this one
		float surfaceParity;	     // https://harddrop.com/wiki/Parity
		float attack;			     // attack
		float attackEffeciency;      // attack / lines cleared
		float clearWithoutAttack;    // how bad it is to clear without attack
		float tSpinSetup;
		float performedTSpin;
		float clearedDoubleOrTriple; // clearing a double or triple is bad
		float highestHeight;
	} static mainFactors = {
		-90, -20, -10, -5, -40, -10, -10, 50.f, 40, -10, 0, 1000, -100, -5  // main with lookaheads
		//-25.f, -10.f, -3.f, -30.f, -30.f, -10.f, -5.f, 20.f, 10.f, -20.f, -30.f, -20.f  // downstacking with lookaheads
		//-30, -20, -4, -5, -20.f, -10, -2, 0, 0, 0 // no lookaheads
	};
	
	class Globals {
	public:
		static int DAS;
		static int ARR;
		static int SDF;

		static int AI_BEST_MOVES_TO_BE_CONSIDERED;
		static int AI_LOOKAHEADS;
		static bool HOLDENABLED;
		static int AI_MOVE_DELAY_MS;
		static bool AI_FULLY_PERFORM_MOVE;

		static bool TURN_BASED;

		static bool AI_USE_UNIQUE_MOVES;

		static std::unordered_map<std::string, sf::Keyboard::Key> CONTROLS;


		static void InitVars();

		static std::unordered_map<std::string, sf::Sound> SOUNDS;

	private:
		static std::unordered_map<std::string, sf::SoundBuffer> SOUND_BUFFERS; ///////////////

		static void InitAllKeys();
		static std::unordered_map<std::string, sf::Keyboard::Key> ALL_KEYS;
	};


	void outputFactors(AIFactor& factor);
	
}

