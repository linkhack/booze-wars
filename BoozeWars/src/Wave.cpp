#include "Wave.h"



Wave::Wave(std::list<wavetuple> waveList)
{
	waveParts = waveList;
	currentEnemies = 0;
	currentDelay = 0;
	time = 0;
}


Wave::~Wave()
{
}

bool Wave::waveIsFinished() 
{
	return waveParts.empty()&&currentEnemies==0;
}

bool Wave::spawnEnemy(float dT)
{
	time += dT;
	if (!waveParts.empty() && currentEnemies == 0) {
		if(time>= currentGroupDelay){
			wavetuple currentWave = waveParts.front();
			waveParts.pop_front();
			currentEnemies = std::get<0>(currentWave);
			currentDelay = std::get<1>(currentWave);
			currentGroupDelay = std::get<2>(currentWave);
			time = 0;
		}
		return false;
	}
	if (time >= currentDelay && currentEnemies>0) {
		currentEnemies--;
		time = 0;
		return true;
	}
	else {
		return false;
	}
}
