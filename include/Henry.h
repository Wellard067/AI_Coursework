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
	void markEnemy(Position p);
	bool isFiring();
	void score(int thisScore, int enemyScore);
	Map* map;
private:
	//maximum row and column number, program uses it to check if something is out for bound
	int mapMaxRow;
	int mapMaxColumn;
	//the centre of the map
	//centre row number
	int centreX;
	//centre column number
	int centreY;
	//maths functions to calculate the data that the AI needs to make decisions
	float calculateTankDeltaR(Node *targetNode); 
	float calculateNeededAngle(Node * targetNode); //needed angle to rotate
	float calculateNeededAngle(Position p);//needed angle to rotate
	float calculateDistance(Position p);//distance between AI and target

	enum EnemyType { TANK, BASE, NOTHING};
	EnemyType enemyType = NOTHING;
	//turretAimMachines and its states, this machines handles the aiming for the turret
	enum TurretAimState { AIM, DETECTION,NOAMMO};
	TurretAimState turretAimState = DETECTION;
	void turretAimMachine();
	void runTurretAimStateMachine();
	//turretFiringMachine and its states, this machines handles the firing for the turret
	enum TurretFiringState { SHOOTING, LOOKING_FOR_TARGET };
	TurretFiringState turretFiringState = LOOKING_FOR_TARGET;
	void turretfiringMachine();
	void runTurretFiringMachine();
	//tankMovementMachine and its states, this machines handles the movement for the tank
	enum TankMovementState { ROTATE, MOVING_TOWARD, IDLE, RUNAWAY, MOVING_BACKWARD };
	TankMovementState tankMovementState = IDLE;
	void tankMovementMachine();
	void runTankMachine(float deltaR);
	//pathFindingMachine and its states, this machines handles the pathing for the tank
	enum PathFindingState { STAND, MOVE_DIAGONAL, COLLISION,MOVE_TO_OPP_X ,MOVE_TO_OPP_Y,FINISH_PRE_PATH};
	PathFindingState pathFindingState = MOVE_DIAGONAL;
	void pathFindingMachine();
	void runPathFindingMachine();
	//records the last area where that the tank is heading to
	enum LastArea { TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT,SPAWN};
	LastArea lastArea = SPAWN;
	//set the currentNode depends on the X and Y position of the AI tank
	void setCurrentNode();
	//go throught the path we found using A*
	void goThorughPath();
	//find the path to a given area of the map, it could be Top Left,Bottom Left etc...
	void findPathTo(LastArea destination, PathFindingState movement);
	//randomize the x position
	int randomizeXPosition(LastArea destination, bool addPrevious);
	//randomize the y position
	int randomizeYPosition(LastArea destination, bool addPrevious);
	//using A* to find the shorest path to a given x and y position.
	void runAStarPathFinding(int x, int y);
	//checks which area where the tank is located on the map
	bool tankLocated(string position);
	//checks if the tank has reached the goal node
	bool tankReachedGoal(int row, int column);
	//x and y position of goal node
	int goalNodeX = 0;
	int goalNodeY = 0;
	//varaible that call the "tankReachedGoal" function and store the result
	bool reachedFinalGoal = false;
	//pointer to AStar object
	AStar* aStar;
	//stores the path nodes
	std::list<Node> path;
	//the node where the tank is located
	Node* currentNode;
	//next node the tank is heading to
	Node* targetNode;
	//checks if the target node exists
	bool targetNodeInitialized = false;
	//the tank only fire a shell if firining is equals to true
	bool firing = false;
	//see if an enemy has been detected
	bool detectedEnemy = false;

	float targetRotationAngle = 0;
	bool detectingEnemy = true;
	//see if turret is facing the target
	bool turretHasTargetLocked = false;
	//distance between AI tank and friendly base
	float friendlyBaseDistance = 0;
	//angle between AI tank and friendly base
	float friendlyBaseAngle = 0;
	//if the AI tank is facing friendly base
	bool facingFriendlyBase = false;

	float enemyBaseDistance = 0;
	float safeDistance = 90.0f;
	//if the path is found
	bool foundPath = false;
	int amountReconstrctedPath = 0;
	//check if collision 
	bool collisionDetected = false;
	int coliisionTimer = 0;


};

