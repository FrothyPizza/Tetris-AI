#pragma once

#include "TetrisAI.h"






// this runs a genetic algorithm to find the best ai factors
// it runs until program is terminated, stdouting the best ai factors after each generation
// currently not functional
class Population {
public:

	

	Population(int popSize = 100);
	void run();

	ts::AIFactor bestFactorSoFar();

private:
	int populationSize;
	int movesPerGeneration{ 500 };
	int movesSoFar{ 0 };

	std::vector<ts::TetrisAI> population;
	ts::AIFactor bestFactor;

	int generation{ 0 };
	float randomModifier{ 5.f };

	ts::AIFactor getChampion();
	void createNewPopulation();
	void mutate(ts::AIFactor& factor);
	
	float random();
	ts::AIFactor generateRandomFactor();

};

