#include "Wave.h"


Wave::Wave()
{

}

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

int Wave::spawnEnemy(float dT)
{
	time += dT;
	if (!waveParts.empty() && currentEnemies == 0) {
		if(time>= currentGroupDelay){
			wavetuple currentWave = waveParts.front();
			waveParts.pop_front();
			currentType = currentWave.type;
			currentEnemies = currentWave.numberOfEnemies;
			currentDelay = currentWave.timeBetweenEnemies;
			currentGroupDelay = currentWave.timeToNextWave;
			time = 0;
		}
		return false;
	}
	if (time >= currentDelay && currentEnemies>0) {
		currentEnemies--;
		time = 0; 
		return currentType;
	}
	else {
		return -1;
	}
}
