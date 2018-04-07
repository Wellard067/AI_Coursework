#include "Henry.h"



Henry::Henry()
{
	map = new Map();
	clearMovement();
}


Henry::~Henry()
{
}

void Henry::move()
{
	if (backward)
	{
 		goBackward();
	}
	setCurrentNode();
	turretAimMachine();
	turretfiringMachine();
	pathFindingMachine();
	tankMachine();
}

void Henry::reset()
{
	path.clear(); //clear the old path 
}

void Henry::collided()
{

}

void Henry::markTarget(Position p)
{
	detectedEnemy = true;
	enemyType = BASE;
	float xDistanceBetweenAITankAndTarget = getX() - p.getX();
	float yDistanceBetweenAITankAndTarget = getY() - p.getY();
	//Using tan(angle) = y/x to work out the angle between the target and our AI tank
	angleBetweeThisAndTarget = atan2(yDistanceBetweenAITankAndTarget, xDistanceBetweenAITankAndTarget) * 180 / PI;
	targetRotationAngle = angleBetweeThisAndTarget + 180;

	//std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markBase(Position p)
{
	float x = p.getX() - 17.5f;
	float y = p.getY() - 17.5f;
	int mx = (x / 35);
	int my = (y / 35);
	for (int i = -1; i < 2; i++)
	{
		if (mx + i < 22 && mx + i >0)
		{
			for (int j = -1; j < 2; j++)
			{
				if (my + j < 16 && my + j>0)
				{
					map->mapArray[my + j][mx + i]->setWall();
				}
			}
		}
	}
	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	distanceBetweenThisAndTarget = sqrt(dx*dx + dy*dy);
	if (distanceBetweenThisAndTarget < safeDistance)
	{
		if (amountReconstrctedPath < 5)
		{
			cout << "Reset path" << endl;
			foundPath = false;
			amountReconstrctedPath++;
		}
	}
	else
	{
		amountReconstrctedPath = 0;
	}
	//std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markShell(Position p)
{
	detectedShell = true;
	float xDistanceBetweenAITankAndTarget = getX() - p.getX();
	float yDistanceBetweenAITankAndTarget = getY() - p.getY();
	angleBetweeThisAndTarget = atan2(yDistanceBetweenAITankAndTarget, xDistanceBetweenAITankAndTarget) * 180 / PI;
	bearingDegrees = angleBetweeThisAndTarget;
	bearingDegrees = (bearingDegrees > 0.0 ? bearingDegrees : (360.0 + bearingDegrees));
	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	distanceBetweenThisAndTarget = sqrt(dx*dx + dy*dy);
	//std::cout << "Shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
	std::cout << bearingDegrees << ")\n";
}

void Henry::markEnemy(Position p)
{
	detectedEnemy = true;
	enemyType = TANK;
	//std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";
	float xDistanceBetweenAITankAndTarget = getX() - p.getX();
	float yDistanceBetweenAITankAndTarget = getY() - p.getY();
	angleBetweeThisAndTarget = atan2(yDistanceBetweenAITankAndTarget, xDistanceBetweenAITankAndTarget) * 180 / PI;
	targetRotationAngle = angleBetweeThisAndTarget + 180;

}
void Henry::setCurrentNode() {
	float x = getX() - 17.5f;
	float y = getY() - 17.5f;
	int mx = (x / 35);
	int my = (y / 35);
	currentNode = map->mapArray[my][mx];
	int ma = 1;
}
void Henry::turretAimMachine()
{
	if (turretAimState == DETECTION&&detectedEnemy)
	{
		turretAimState = AIM;
	}
	if (turretAimState == AIM&&!detectedEnemy)
	{
		turretAimState = DETECTION;
	}
	switch (turretAimState)
	{
	case DETECTION:
	{
		detectingEnemy = true;
		turretGoRight();
	}
		break;
	case AIM: {
		float deltaR = turretTh - targetRotationAngle;
		if (deltaR > 1 && deltaR < 180) {
			turretGoLeft();
			turretHasTargetLocked = false;
		}
		else if (deltaR < -1 && deltaR > -180) {
			turretGoRight();
			turretHasTargetLocked = false;
		}
		else if (deltaR > 180) {
			turretGoRight();
			turretHasTargetLocked = false;
		}
		else if (deltaR < -180) {
			turretGoLeft();
			turretHasTargetLocked = false;
		}
		else {
			turretHasTargetLocked = true;
			detectedEnemy = false;
			detectingEnemy = false;
			stopTurret();
			clearMovement();
		}
		break;
	}
	case NOAMMO:
		break;
	default:
		break;
	}

}
void Henry::turretfiringMachine()
{
	if (turretFiringState == LOOKINGFORTARGET&&turretHasTargetLocked)
	{
		if (enemyType==TANK)
		{
			turretFiringState = SHOOTINGTANK;
		}
		if (enemyType == BASE)
		{
			turretFiringState = SHOOTINGBASE;
		}
	}
	if ((turretFiringState == SHOOTINGTANK|| turretFiringState == SHOOTINGBASE)&&detectingEnemy)
	{
		turretFiringState = LOOKINGFORTARGET;
	}
	switch (turretFiringState)
	{
	case SHOOTINGBASE:
	{
		firing = true;
	}
	break;
	case SHOOTINGTANK: {
		firing = true;
	}
	break;
	case LOOKINGFORTARGET:
	{
		firing = false;
	}
		break;
	default:
		break;
	}
}

void Henry::tankMachine()
{
	tankMovementState = IDLE;
	if (path.size() != 0) { // go through the A* path
		targetNode = &path.front();
		if (targetNode->equals(*currentNode)) {
			cout << "Reached: " << targetNode->getRow() << " " << targetNode->getColumn() << endl;
			path.pop_front();
			if (path.size() != 0) {
			targetNode = &path.front();
			}
		}
	}

	//Local Variables
	float deltaX = getX() - targetNode->getX();
	float deltaY = getY() - targetNode->getY();
	float angleInDegree = atan2(deltaY, deltaX) * 180 / PI;
	float neededAngle = angleInDegree + 180;
	float rotational = pos.getTh();
	float deltaR = rotational - neededAngle;

	int distanceThreshold = 5;
	int directionThreshold = 5;
	bool facingDirection = (deltaR < directionThreshold && deltaR > -directionThreshold);
	bool reachesGoal = (path.size() == 0);

	if (tankMovementState == IDLE)
	{
		if (facingDirection && !reachesGoal) {
			tankMovementState = MOVINGTOWARD;
		}
		else if (!facingDirection && !reachesGoal) {
			tankMovementState = ROTATE;
		}
	}
	else if (tankMovementState == MOVINGTOWARD)
	{
		if (reachesGoal)
		{
			tankMovementState = IDLE;
		}
	}
	else if (tankMovementState == ROTATE)
	{
		if (facingDirection)
		{
			tankMovementState = MOVINGTOWARD;
		}
	}
	switch (tankMovementState)
	{
	case ROTATE:
	{//handles which direction the tank is facing depending on the position of the next node
		if (deltaR > 1 && deltaR < 180) {
			goLeft();
		}
		else if (deltaR < -1 && deltaR > -180) {
			goRight();
		}
		else if (deltaR > 180) {
			goRight();
		}
		else if (deltaR < -180) {
			goLeft();
		}
	} break;
	case MOVINGTOWARD:
	{
		goForward();
	}
		break;
	case IDLE:
	{
		stop();
		ShellDetectedTimer = 0;
	}
		break;
	case RUNAWAY:
		break;
	case DODGESHELL:
	{
		if (ShellDetectedTimer < 50)
		{
			if (distanceBetweenThisAndTarget < 119)
			{
				ShellDetectedTimer++;
				if (bearingDegrees <179)
				{
					backward = false;
					forward = true;
				}
				if (bearingDegrees >180)
				{
					backward = true;
					forward = false;
				}
			}
		}
		else
		{
			backward = false;
			forward = false;
			detectedShell = false;
			tankMovementState = IDLE;
		}
	}
		break;
	default:
		break;
	}

}
void Henry::pathFindingMachine() {
	//if (pathFindingState == MOVETOCENTRE&&foundPath)
	//{
	//	pathFindingState = STAND;
	//}
	switch (pathFindingState)
	{
	case STAND:
	{

	}
		break;
	case MOVETOCENTRE:
	{
		int centreX = 8;
		int centreY = 10;
		while (!foundPath)
		{
			if (map->AStar(*currentNode, *map->mapArray[centreX][centreY]))
			{
				foundPath = true;
				path = map->getPath();
			}
		}
	}
			break;
	default:
		break;
	}
}
bool Henry::isFiring()
{
	return firing;
}

void Henry::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}

