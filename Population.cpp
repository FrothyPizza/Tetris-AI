#include "Population.h"



Population::Population(int popSize) : populationSize{ popSize } {

	population.reserve(popSize);
	for (int i = 0; i < popSize; ++i)
		population.push_back(ts::TetrisAI{ generateRandomFactor() });
		//population.push_back(ts::TetrisAI{ ts::mainFactors }); // start it out with some decent factors
	mutate(population.at(1).factors);

	ts::outputFactors(population.at(0).factors);
}

void Population::run() {
	for (size_t i = 0; i < population.size(); ++i) {
		if(!population[i].isDead()) population[i].doNextMove();
	}

	++movesSoFar;

	if (movesSoFar > movesPerGeneration) {
		createNewPopulation();
		movesSoFar = 0;
	}
}
ts::AIFactor Population::bestFactorSoFar() {
	return bestFactor;
}

ts::AIFactor Population::getChampion() {
	float highestScore{ -10000 };
	ts::AIFactor best = bestFactor;

	for (size_t i = 0; i < population.size(); ++i) {
		float score = 0;
		score += population[i].totalAttack*3;
		score += population[i].piecesPlaced;
		if (population[i].isDead()) score -= 1000;

		if (score > highestScore) {
			best = population[i].factors;
			highestScore = score;
		}
	}

	return best;
}

void Population::createNewPopulation() {

	randomModifier = 10.f - (generation / 5.f);
	if (randomModifier < 2.f) randomModifier = 2.f;



	ts::AIFactor champion = getChampion();
	bestFactor = champion;

	std::cout << "Generation " << generation << '\n';
	ts::outputFactors(champion);

	population.clear();

	population.reserve(populationSize);
	for (int i = 0; i < populationSize; ++i) {
		ts::AIFactor mutated = champion;
		mutate(mutated);
		population.push_back(ts::TetrisAI{ mutated });
	}

	population[0].factors = champion; // add the champion to the next generation so it doens't take any steps back

	++generation;
}

float Population::random() {
	return (((float)rand() / (RAND_MAX) * 2) - 1.f) * randomModifier;
}

void Population::mutate(ts::AIFactor& factor) {
	//float change = ((float)rand() / (RAND_MAX) * 2) - 1.f;
	factor.attack += random();
	factor.attackEffeciency += random();
	factor.averageHeight += random();
	factor.clearWithoutAttack += random();
	factor.heightVariance += random();
	factor.hole += random();
	factor.holeCovered += random();
	factor.Idependency += random();
	factor.partialIdependency += random();
	factor.surfaceParity += random();
}

ts::AIFactor Population::generateRandomFactor() {
	//float bound = 150;
	float bound = 30;
	// there's prob a better way to do this, but whatever
	return ts::AIFactor{
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
		(float)(rand() % ((int)bound * 2)) - bound,
	};
}
