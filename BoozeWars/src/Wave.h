#pragma once
class Wave
{
private:
	int totalEnemies;
	float timeDelay;
	float time;
public:
	Wave(int n, float d);
	~Wave();

	bool waveIsFinished();
	bool spawnEnemy(float dT);
};

