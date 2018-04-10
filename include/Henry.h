#pragma once
#include<iostream>
#include "aitank.h"
#include "Map.h"
#include "Node.h"
#include "AStar.h"
using namespace std;
class Henry : public AITank 
{
public:
	Henry();
	~Henry();
	void move();
	void reset();
	void collided();
	void markTarget(Position p);
	void markNodeAsNotTraversable(Position p);
	void markBase(Position p);
	void markShell(Position p);
	float getDistance(Position p);
	void markEnemy(Position p);
	bool isFiring();
	void score(int thisScore, int enemyScore);
	Map* map;
private:
	float calculateDeltaR(Node *targetNode);
	float calculateNeededAngle(Node * targetNode);
	float calculateNeededAngle(Position p);

	void setCurrentNode();
	enum EnemyType { TANK, BASE, NOTHING };
	EnemyType enemyType = NOTHING;

	enum TurretAimState { AIM, DETECTION, NO_AMMO };
	TurretAimState turretAimState = DETECTION;
	enum TurretFiringState { SHOOTING_BASE, SHOOTING_TANK, LOOKING_FOR_TARGET };
	TurretFiringState turretFiringState = LOOKING_FOR_TARGET;
	enum TankMovementState { ROTATE, MOVING_TOWARD, IDLE, RUNAWAY, MOVING_BACKWARD };
	TankMovementState tankMovementState = IDLE;
	enum PathFindingState { STAND, MOVE_TO_CENTRE, MOVEAROUND_MAP, DODGE_SHELL, COLLISION,MOVE_TO_OTHER_SIDE };
	PathFindingState pathFindingState = MOVE_TO_OTHER_SIDE;
	enum LastLocation { TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT, MIDDLE };
	LastLocation lastLocation = MIDDLE;

	void turretAimMachine();
	void runTurretAimStateMachine();
	void turretfiringMachine();
	void runTurretFiringMachine();
	void tankMachine();
	void runTankMachine(float deltaR);
	void pathFindingMachine();

	void runPathFindingMachine();

	void findPath(int x, int y);

	AStar* aStar;
	std::list<Node> path;
	Node* currentNode;
	Node* targetNode;

	bool targetNodeInitialize = false;
	bool firing = false;
	bool detectedEnemy = false;

	bool detectedShell = false;
	int shellX = 0, shellY = 0;
	int ShellDetectedTimer = 0;

	float angleBetweeThisAndTarget = 0;
	float targetRotationAngle = 0;
	//EnemyTyPE
	bool detectingEnemy = true;
	//tankMachine variables
	float targetDistance = 0;
	float safeDistance = 90.0f;
	float tankFacingDirection = 0;
	bool turretHasTargetLocked = false;
	int bearingDegrees = 0;
	bool foundPath = false;
	int amountReconstrctedPath = 0;
	bool collisionDetected = false;
	int coliisionTimer = 0;
	int mapMaxRow;
	int mapMaxColumn;
};

