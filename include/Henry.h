#pragma once
#include<iostream>
#include "aitank.h"

class Henry : public AITank 
{
public:
	Henry();
	~Henry();
	void move();
	void reset();
	void collided();
	void markTarget(Position p);
	void markBase(Position p);
	void markShell(Position p);
	void markEnemy(Position p);
	void turretMachine();
	void tankMachine();
	bool isFiring();
	void score(int thisScore, int enemyScore);

private:
	enum TurretState { AIM, DETECTION, SHOOTINGTANK, SHOOTINGBASE, NOAMMO };
	TurretState turretState = DETECTION;
	bool firing = false;
	bool detectedEnemy = false;
	bool enemyIsDead = false;
	float directionOfTarget = 0;
	//turrentMachine variables
	bool baseIsDead = false;
	float turrentFacingDirection = 0;
	enum EnemyType {TANK,BASE,NOTHING};
	EnemyType enemyType = NOTHING;
	bool tankenDamageRecently = false;
	//tankMachine variables
	float distanceBetweenThisAndTarget = 0;
	float safeDistance = 120.f;
	float tankFacingDirection = 0;
	
};

