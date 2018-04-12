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

	enum TurretAimState { AIM, DETECTION,NOAMMO};
	TurretAimState turretAimState = DETECTION;
	enum TurretFiringState { SHOOTING, LOOKING_FOR_TARGET };
	TurretFiringState turretFiringState = LOOKING_FOR_TARGET;
	enum TankMovementState { ROTATE, MOVING_TOWARD, IDLE, RUNAWAY, MOVING_BACKWARD };
	TankMovementState tankMovementState = IDLE;
	enum PathFindingState { STAND, MOVE_TO_CENTRE, MOVE_DIAGONAL, COLLISION,MOVE_TO_OPP_X ,MOVE_TO_OPP_Y,FINISH_PRE_PATH};
	PathFindingState pathFindingState = MOVE_DIAGONAL;
	enum LastLocation { TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT, MIDDLE };
	LastLocation lastLocation = MIDDLE;

	void turretAimMachine();
	void runTurretAimStateMachine();
	void turretfiringMachine();
	void runTurretFiringMachine();
	void goThorughPath();
	void tankMovementMachine();
	void runTankMachine(float deltaR);
	void pathFindingMachine();

	void findPathTo(LastLocation destination, PathFindingState movement);

	void runPathFindingMachine();

	void runAStarPathFinding(int x, int y);

	bool tankLocated(string position);

	bool tankReachedGoal(int row, int column);

	int randomizeXPosition(LastLocation destination, bool addPrevious);

	int randomizeYPosition(LastLocation destination, bool addPrevious);

	AStar* aStar;
	std::list<Node> path;
	Node* currentNode;
	Node* targetNode;

	bool targetNodeInitialized = false;
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
	float friendlyBaseDistance = 0;
	float enemyBaseDistance = 0;
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
	bool reachedFinalGoal = false;
	int finalGoalX=0;
	int finalGoalY=0;
	bool friendlyBaseSpotted = false;
	int centreX;
	int centreY;
};

