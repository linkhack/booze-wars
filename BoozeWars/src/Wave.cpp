#include "Wave.h"



Wave::Wave(int n, float d)
{
	totalEnemies = n;
	timeDelay = d;
	time = 0;
}


Wave::~Wave()
{
}

bool Wave::waveIsFinished() 
{
	return totalEnemies == 0;
}
bool Wave::spawnEnemy(float dT)
{
	time += dT;
	if (time >= timeDelay && totalEnemies>0) {
		totalEnemies--;
		time = 0;
		return true;
	}
	else {
		return false;
	}
}
