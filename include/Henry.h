#pragma once
#include<iostream>
#include "aitank.h"
#include "Map.h"
#include "Node.h"
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
	void setCurrentNode();
	void turretAimMachine();
	void turretfiringMachine();
	float angleNeeded();
	void tankMachine();
	void pathFindingMachine();
	bool isFiring();
	void score(int thisScore, int enemyScore);
	Map* map;
private:
	Node* currentNode;
	Node* targetNode;
	std::list<Node> path;
	enum TurretAimState { AIM, DETECTION, NOAMMO };
	TurretAimState turretAimState = DETECTION;
	enum TurretFiringState { SHOOTINGBASE, SHOOTINGTANK,LOOKINGFORTARGET};
	TurretFiringState turretFiringState = LOOKINGFORTARGET;
	enum TankMovementState { ROTATE, MOVINGTOWARD, IDLE, RUNAWAY,DODGESHELL };
	TankMovementState tankMovementState = IDLE;
	enum PathFindingState {STAND,MOVETOCENTRE};
	PathFindingState pathFindingState = MOVETOCENTRE;
	bool firing = false;
	bool detectedEnemy = false;
	bool detectedShell = false;
	int ShellDetectedTimer = 0;
	bool enemyIsDead = false;
	float angleBetweeThisAndTarget = 0;
	//turrentMachine variables
	bool baseIsDead = false;
	float targetRotationAngle = 0;
	//EnemyTyPE
	enum EnemyType {TANK,BASE,NOTHING};
	EnemyType enemyType = NOTHING;
	bool tankenDamageRecently = false;
	bool detectingEnemy = true;
	//tankMachine variables
	float distanceBetweenThisAndTarget = 0;
	float safeDistance = 80.0f;
	float tankFacingDirection = 0;
	bool turretHasTargetLocked = false;
	int bearingDegrees = 0;
	bool foundPath = false;
	int amountReconstrctedPath = 0;
};

