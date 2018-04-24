#pragma once
#include <tuple>
#include <list>

/*
first: number of enemies
second: time between enemies
third: time between group of enemies
*/
typedef std::tuple<int, float, float> wavetuple; 

class Wave
{
private:
	std::list<wavetuple> waveParts;
	int currentEnemies;
	float currentDelay;
	float currentGroupDelay;
	float time;
	bool spawningEnemies;
public:
	Wave(std::list<wavetuple> waveList);
	~Wave();

	bool waveIsFinished();
	bool spawnEnemy(float dT);
};

