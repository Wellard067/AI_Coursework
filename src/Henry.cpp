#include "Henry.h"



Henry::Henry()
{
	map = new Map();
	mapMaxRow = map->rows;
	mapMaxColumn = map->columns;
	aStar = new AStar(map);
	clearMovement();
}


Henry::~Henry()
{
}
bool Henry::isFiring()
{
	return firing;
}

void Henry::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}

void Henry::move()
{
	for (int i = 0; i < mapMaxColumn ; i++)
	{
		map->mapArray[0][i]->setWall();
		map->mapArray[mapMaxRow-1][i]->setWall();
	}
	for (int i = 0; i < mapMaxRow; i++)
	{
		map->mapArray[i][0]->setWall();
		map->mapArray[i][mapMaxColumn - 1]->setWall();
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
	collisionDetected = true;
	coliisionTimer++;
}

void Henry::markTarget(Position p)
{
	detectedEnemy = true;
	enemyType = BASE;
	targetDistance = getDistance(p);
	//Using tan(angle) = y/x to work out the angle between the target and our AI tank
	targetRotationAngle = calculateNeededAngle(p);
	//std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markBase(Position p)
{
	markNodeAsNotTraversable(p);
	targetDistance = getDistance(p);
	if (targetDistance < safeDistance)
	{
		if (amountReconstrctedPath < 2)
		{
			cout << "Reset path" << endl;
			foundPath = false;
			amountReconstrctedPath++;
		}
	}
	else if (amountReconstrctedPath > 2)
	{
		amountReconstrctedPath = 0;
	}
	//std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markShell(Position p)
{
	detectedShell = true;
	bearingDegrees = calculateNeededAngle(p);
	foundPath = false;
	shellX = (p.getX()/35);
	shellY = (p.getY()/35);
	cout << "Shell spotted at (" << shellX << ", " << shellY << endl;
}
void Henry::markEnemy(Position p)
{
	detectedEnemy = true;
	enemyType = TANK;
	//std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";
	targetRotationAngle = calculateNeededAngle(p);
}

float Henry::getDistance(Position p)
{
	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	targetDistance = sqrt(dx*dx + dy*dy);
	return targetDistance;
}
float Henry::calculateDeltaR(Node *targetNode)
{
	float neededAngle = calculateNeededAngle(targetNode);
	float rotational = pos.getTh();
	float deltaR = rotational - neededAngle;
	return deltaR;
}
float Henry::calculateNeededAngle(Node*targetNode)
{
	float deltaX = getX() - targetNode->getX();
	float deltaY = getY() - targetNode->getY();
	float angleInDegree = atan2(deltaY, deltaX) * 180 / PI;
	float neededAngle = angleInDegree + 180;
	return neededAngle;
}
float Henry::calculateNeededAngle(Position p)
{
	float deltaX = getX() - p.getX();
	float deltaY = getY() - p.getY();
	float angleInDegree = atan2(deltaY, deltaX) * 180 / PI;
	float neededAngle = angleInDegree + 180;
	return neededAngle;
}
void Henry::markNodeAsNotTraversable(Position target)
{
	float x = target.getX();
	float y = target.getY();
	int mx = (x / 17.5);
	int my = (y / 17.275);
	for (int i = -3; i < 2; i++)
	{
		if (mx + i > mapMaxColumn || mx + i < 0) { continue; }
		for (int j = -3; j < 2; j++)
		{
			if (my + j > mapMaxRow || my + j < 0) { continue; }
				map->mapArray[my + j][mx + i]->setWall();
		}
	}
}

void Henry::setCurrentNode() {
	float x = getX() - 8.75f;
	float y = getY() - 8.75f;
	int mx = (x / 17.5);
	int my = (y / 17.275);
	currentNode = map->mapArray[my][mx];
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
	runTurretAimStateMachine();
}
void Henry::runTurretAimStateMachine()
{
	switch (turretAimState)
	{
	case DETECTION:
	{
		detectingEnemy = true;
		turretGoRight();
	}
	break;
	case AIM: {
		pathFindingState = STAND;
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
	case NO_AMMO:
		break;
	default:
		break;
	}

}
void Henry::turretfiringMachine()
{
	if (turretFiringState == LOOKING_FOR_TARGET&&turretHasTargetLocked)
	{
		if (enemyType==TANK)
		{
			turretFiringState = SHOOTING_TANK;
		}
		if (enemyType == BASE)
		{
			turretFiringState = SHOOTING_BASE;
		}
	}
	if ((turretFiringState == SHOOTING_TANK|| turretFiringState == SHOOTING_BASE)&&detectingEnemy)
	{
		turretFiringState = LOOKING_FOR_TARGET;
	}
	runTurretFiringMachine();
}
void Henry::runTurretFiringMachine()
{
	switch (turretFiringState)
	{
	case SHOOTING_BASE:
	{
		firing = true;
	}
	break;
	case SHOOTING_TANK: {
		firing = true;
	}
						break;
	case LOOKING_FOR_TARGET:
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
		targetNodeInitialize = true;
		map->mapArray[targetNode->getRow()][targetNode->getColumn()]->setPath();
		if (targetNode->equals(*currentNode)) {
			cout << "Reached: " << targetNode->getRow() << " " << targetNode->getColumn() << endl;
			path.pop_front();
			if (path.size() != 0) {
			targetNode = &path.front();
			}
		}
	}
	float deltaR = 0;
	if (targetNodeInitialize)
	{
		 deltaR = calculateDeltaR(targetNode);
	}
	int distanceThreshold = 5;
	int directionThreshold = 5;
	bool facingDirection = (deltaR < directionThreshold && deltaR > -directionThreshold);
	bool reachesGoal = (path.size() == 0);

	if (tankMovementState == IDLE)
	{
		if (collisionDetected)
		{
			tankMovementState = MOVING_BACKWARD;
		}
		else if (facingDirection && !reachesGoal) {
			tankMovementState = MOVING_TOWARD;
		}
		else if (!facingDirection && !reachesGoal) {
			tankMovementState = ROTATE;
		}
	}
	else if (tankMovementState == MOVING_TOWARD)
	{
		if (collisionDetected)
		{
			tankMovementState = MOVING_BACKWARD;
		}
		else if (reachesGoal)
		{
			tankMovementState = IDLE;
		}
	}
	else if (tankMovementState == ROTATE)
	{
		if (collisionDetected)
		{
			tankMovementState = MOVING_BACKWARD;
		}
		else if (facingDirection)
		{
			tankMovementState = MOVING_TOWARD;
		}
	}
	if (tankMovementState == MOVING_BACKWARD&&!collisionDetected)
	{
		tankMovementState = IDLE;
	}
	if (reachesGoal)
	{
		foundPath = false;
	}
	runTankMachine(deltaR);
}

void Henry::runTankMachine(float deltaR)
{
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
	case MOVING_TOWARD:
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
	case MOVING_BACKWARD:
	{
		goBackward();
		if (coliisionTimer< 5)
		{
			collisionDetected = false;
		}
		else
		{
			coliisionTimer = 0;
		}
	}
	break;
	default:
		break;
	}
}
void Henry::pathFindingMachine() {
	int centreX = mapMaxRow / 2;
	int centreY = mapMaxColumn / 2;
	if (pathFindingState == STAND)
	{
		if (detectedEnemy&&!detectingEnemy)
		{
		}
		else if (!detectedEnemy&&detectingEnemy)
		{
			foundPath = false;
			pathFindingState = MOVE_TO_CENTRE;
		}

	}
	if (pathFindingState == MOVE_TO_CENTRE&& currentNode->getColumn() == centreY&& currentNode->getRow() == centreX)
	{
		foundPath = false;
		pathFindingState = MOVEAROUND_MAP;
		lastLocation = MIDDLE;
		cout << "Switch to move around map state " << endl;
	}
	if (pathFindingState == MOVE_TO_CENTRE && collisionDetected)
	{
		foundPath = false;
		pathFindingState = COLLISION;
	}

	if (pathFindingState == DODGE_SHELL&&!detectedShell)
	{
		pathFindingState = MOVEAROUND_MAP;
	}
	if (pathFindingState == COLLISION && !collisionDetected)
	{
		foundPath = false;
		pathFindingState = MOVEAROUND_MAP;
	}
	runPathFindingMachine();
}
void Henry::runPathFindingMachine()
{
	int centreX = mapMaxRow / 2;
	int centreY = mapMaxColumn / 2;
	
	switch (pathFindingState)
	{
	case STAND:
	{
		path.clear();
		clearMovement();
	}
	break;
	case MOVE_TO_CENTRE:
	{
		while (!foundPath)
		{
			path.clear();
			if (aStar->pathExists(*currentNode, *map->mapArray[centreX][centreY]))
			{
				foundPath = true;
				path = aStar->getPath();
			}
			else
			{
				centreX++;
				centreY++;
			}
		}
	}
	break;
	case MOVE_TO_OTHER_SIDE:
	{
		if (currentNode->getRow() < centreX && currentNode->getColumn() > centreY)
		{
			int x = (int)(rand() % 2 + 2);
			int y = (int)(rand() % 2 + 2);
			findPath(y, x);
			lastLocation = TOP_LEFT;
		}
		else if (currentNode->getRow() >= centreX && currentNode->getColumn() > centreY)
		{
			int x = (int)(rand() % 2 + 2);
			int y = (int)(rand() % 2 + (mapMaxRow - 3));
			findPath(y, x);
			lastLocation = BOTTOM_LEFT;
		}
		else if (currentNode->getRow() <= centreX && currentNode->getColumn() < centreY)
		{
			int x = (int)(rand() % 2 + (mapMaxColumn - 3));
			int y = (int)(rand() % 2 + 2);

			findPath(y, x);
			lastLocation = TOP_RIGHT;
		}
		else if (currentNode->getRow() <= centreX && currentNode->getColumn() > centreY)
		{
			int x = (int)(rand() % 2 + (mapMaxColumn - 3));
			int y = (int)(rand() % 2 + (mapMaxRow - 3));
			findPath(y, x);
			lastLocation = BOTTOM_RIGHT;
		}
	}
		break;
	case MOVEAROUND_MAP:
	{
			switch (lastLocation)
			{
			case TOP_LEFT:
			{
				int x = (int)(rand() % 2 + 2);
				int y = (int)(rand() % 2 + (mapMaxRow - 3));
				findPath(y, x);
				lastLocation = BOTTOM_LEFT;
			}
			break;
			case BOTTOM_LEFT:
			{
				int x = (int)(rand() % 2 + 2);
				int y = (int)(rand() % 2 +2);
				 findPath(y, x);
				lastLocation = TOP_LEFT;
			}
			break;
			case TOP_RIGHT:
			{
				int x = (int)(rand() % 2 + (mapMaxColumn - 3));
				int y = (int)(rand() % 2 + (mapMaxRow - 3));
				findPath(y, x);
				lastLocation = BOTTOM_RIGHT;
			}
			break;

			case BOTTOM_RIGHT:
			{
				int x = (int)(rand() % 2 + (mapMaxColumn - 3));
				int y = (int)(rand() % 2 + 2);
				findPath(y, x);
				lastLocation = TOP_RIGHT;
			}
			break;
			case MIDDLE:
				lastLocation = static_cast<LastLocation>(rand() % 3);
				break;
			default:
				break;
			}
		cout << foundPath << endl;
		cout << "Last location: " << lastLocation << endl;
	}
	break;
	//case DODGE_SHELL:
	//{
	//	ShellDetectedTimer++;
	//	int dodgeX = 0;
	//	int dodgeY = 0;
	//	if (currentNode->getRow() > shellY && currentNode->getColumn()> shellX&&pos.getTh() <225&&pos.getTh() >45)
	//	{
	//		dodgeX = currentNode->getColumn() - 1;
	//		dodgeY = currentNode->getRow() + 1;
	//	}
	//	else
	//	{
	//		dodgeX = currentNode->getColumn() + 1;
	//		dodgeY = currentNode->getRow() - 1;
	//	}
	//	if (currentNode->getRow() > shellY && currentNode->getColumn()< shellX&&pos.getTh() <315 && pos.getTh() >135)
	//	{
	//		dodgeX = currentNode->getColumn() - 1;
	//		dodgeY = currentNode->getRow() - 1;
	//	}
	//	else
	//	{
	//		dodgeX = currentNode->getColumn() + 1;
	//		dodgeY = currentNode->getRow() + 1;
	//	}
	//	if (currentNode->getRow() < shellY && currentNode->getColumn()< shellX)
	//	{
	//		dodgeX = currentNode->getColumn() - 2;
	//		dodgeY = currentNode->getRow() + 2;
	//	}
	//	if (currentNode->getRow() < shellY && currentNode->getColumn()> shellX)
	//	{
	//		dodgeX = currentNode->getColumn() - 2;
	//		dodgeY = currentNode->getRow() - 2;
	//	}
	//	if (ShellDetectedTimer <50)
	//	{
	//		if (!foundPath)
	//		{
	//			path.clear();
	//			if (aStar->pathExists(*currentNode, *map->mapArray[dodgeX][dodgeY]))
	//			{
	//				foundPath = true;
	//				path = aStar->getPath();
	//			}
	//		}
	//	}
	//}
	//break;
	case COLLISION:
	{
		lastLocation = TOP_RIGHT;
	}
	break;
	default:
		break;
	}
}
void Henry::findPath(int y,int x)
{
	int row = y;
	int column = x;
	bool xyNeedRadnom = false;
	while (!foundPath)
	{
		if (xyNeedRadnom)
		{
			row = y+(int)(rand() % 1 + 3);
			if (y + row > mapMaxRow - 1)
			{
				row = y - (int)(rand() % 1 + 3);
			}
			column = x+ (int)(rand() % 1 + 3);
			if (x + column > mapMaxColumn - 1)
			{
				column = x  - (int)(rand() % 1 + 3);
			}
		}
		path.clear();
		if (aStar->pathExists(*currentNode, *map->mapArray[row][column]))
		{
			path = aStar->getPath();
			foundPath = true;
			xyNeedRadnom = false;
		}
		else
		{
			xyNeedRadnom = true;
		}
	}
}
