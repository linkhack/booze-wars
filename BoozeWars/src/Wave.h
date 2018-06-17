#pragma once
#include <list>
#include "Enemy.h"

/*
first: number of enemies
second: time between enemies
third: time between group of enemies
*/

struct wavetuple
{
	int type;
	int numberOfEnemies;
	float timeBetweenEnemies;
	float timeToNextWave;

	wavetuple(int a, int b, float c, float d) :
		type(a), numberOfEnemies(b), timeBetweenEnemies(c), timeToNextWave(d) {}
};

class Wave
{
private:
	std::list<wavetuple> waveParts;
	int currentEnemies;
	int currentType;
	float currentDelay;
	float currentGroupDelay;
	float time;
	bool spawningEnemies;
	bool isFinished;
	Wave* nextWave;
public:
	Wave();
	Wave(std::list<wavetuple> waveList, Wave* wave);
	~Wave();

	bool waveIsFinished();
	int spawnEnemy(float dT);
	Wave* getNextWave();
};

