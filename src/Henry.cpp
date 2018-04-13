#include "Henry.h"



Henry::Henry()
{
	//initialize map object
	map = new Map();
	mapMaxRow = map->rows;
	mapMaxColumn = map->columns;
	centreX = mapMaxRow / 2;
	centreY = mapMaxColumn / 2;
	//initialize AStar object
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
	//set the edge of the map as not traversable
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
	//calculate which node the tank is on based on x and y position of the tank
	setCurrentNode();
	//run all four finite state machine
	//responsible for turret aiming
	turretAimMachine();
	//responsible for turret firing
	turretfiringMachine();
	//responsible for path finding
	pathFindingMachine();
	//responsbile for tank movement
	tankMovementMachine();
	//start the collision timer once collision is detected
	if (collisionDetected)
	{
		coliisionTimer++;
	}
	facingFriendlyBase = false;
	detectedEnemy = false;
}

void Henry::reset()
{
	path.clear(); //clear the old path 
}

void Henry::collided()
{
	collisionDetected = true;
}

void Henry::markTarget(Position p)
{
	detectedEnemy = true;
	enemyType = BASE;
	//calculates distance and needed angle once a target has been found
	enemyBaseDistance = calculateDistance(p);
	targetRotationAngle = calculateNeededAngle(p);
	turretHasTargetLocked = false;
}

void Henry::markBase(Position p)
{
	//mark friendly base nodes as not traversale
	markNodeAsNotTraversable(p);
	//calculates distance and needed angle once a target has been found
	friendlyBaseDistance = calculateDistance(p);
	friendlyBaseAngle = calculateNeededAngle(p);
	if (reachedFinalGoal)
	{
		amountReconstrctedPath = 0;
	}
	if (friendlyBaseDistance < safeDistance)
	{
		if (amountReconstrctedPath < 3)
		{
			cout << "Reset path" << endl;
			foundPath = false;
			pathFindingState = FINISH_PRE_PATH;
		}
		amountReconstrctedPath++;
	}
	float friendlyBaseDeltaR = turretTh - friendlyBaseAngle;
	if (friendlyBaseDeltaR > 25 && friendlyBaseDeltaR < 180) {
		facingFriendlyBase = false;
	}
	else if (friendlyBaseDeltaR < -25 && friendlyBaseDeltaR > -180) {
		facingFriendlyBase = false;
	}
	else if (friendlyBaseDeltaR > 180) {
		facingFriendlyBase = false;
	}
	else if (friendlyBaseDeltaR < -180) {
		facingFriendlyBase = false;
	}
	else
	{
		facingFriendlyBase = true;
	}
	cout << facingFriendlyBase << endl;
}
void Henry::markShell(Position p)
{
}
void Henry::markEnemy(Position p)
{
	detectedEnemy = true;
	enemyType = TANK;
	targetRotationAngle = calculateNeededAngle(p);
	turretHasTargetLocked = false;
}
//calculate the distance between AI tank and target
float Henry::calculateDistance(Position p)
{
	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	friendlyBaseDistance = sqrt(dx*dx + dy*dy);
	return friendlyBaseDistance;
}

float Henry::calculateTankDeltaR(Node *targetNode)
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
//mark all the nodes around friendly base as not traversable. 
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
	//switch to different state based on the data the AI gathered
	if (turretAimState == DETECTION&&!hasAmmo())
	{
		turretAimState = NOAMMO;
	}
	if (turretAimState == AIM&&!hasAmmo())
	{
		turretAimState = NOAMMO;
	}
	if (turretAimState == DETECTION&&detectedEnemy&&!facingFriendlyBase)
	{
		turretAimState = AIM;
	}
	if (turretAimState == AIM)
	{
		if (!detectedEnemy|| facingFriendlyBase)
		{
			turretAimState = DETECTION;
		}
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
		//scanning by rotating turret clock-wise
		turretGoRight();
	}
	break;
	case AIM: {
		pathFindingState = STAND;
		//Rotate the turret so that it is facing the target
		float targetAngle = turretTh - targetRotationAngle;
			if (targetAngle > 1 && targetAngle < 180) {
				turretGoLeft();
				turretHasTargetLocked = false;
			}
			else if (targetAngle < -1 && targetAngle > -180) {
				turretGoRight();
				turretHasTargetLocked = false;
			}
			else if (targetAngle > 180) {
				turretGoRight();
				turretHasTargetLocked = false;
			}
			else if (targetAngle < -180) {
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
	{
		turretHasTargetLocked = false;
		turretGoRight();
	}
	default:
		break;
	}

}
void Henry::turretfiringMachine()
{
	//switch to different state based on the data the AI gathered
	if (turretFiringState == LOOKING_FOR_TARGET&&turretHasTargetLocked&&!facingFriendlyBase)
	{
			turretFiringState = SHOOTING;
	}
	if (turretFiringState == SHOOTING)
	{
		if (facingFriendlyBase|| detectingEnemy)
		{
			turretFiringState = LOOKING_FOR_TARGET;
		}
	}
	runTurretFiringMachine();
}
void Henry::runTurretFiringMachine()
{
	switch (turretFiringState)
	{
	case SHOOTING:
	{
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
void Henry::tankMovementMachine()
{
	tankMovementState = IDLE;
	goThorughPath();
	float targetAngle = 0;
	if (targetNodeInitialized)
	{
		 targetAngle = calculateTankDeltaR(targetNode);
	}
	int directionThreshold = 7;
	bool facingDirection = (targetAngle < directionThreshold && targetAngle > -directionThreshold);
	bool reachesGoal = (path.size() == 0);
	//switch to different state based on the data the AI gathered
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
	else if (tankMovementState == MOVING_BACKWARD&&!collisionDetected)
	{
		tankMovementState = IDLE;
	}
	if (reachesGoal)
	{
		foundPath = false;
	}
	runTankMachine(targetAngle);
}
//go through the A* path
void Henry::goThorughPath()
{
	if (path.size() != 0) {
		targetNode = &path.front();
		targetNodeInitialized = true;
		map->mapArray[targetNode->getRow()][targetNode->getColumn()]->setPath();
		if (targetNode->equals(*currentNode)) {
			path.pop_front();
			if (path.size() != 0) {
				targetNode = &path.front();
			}
		}
	}
}
void Henry::runTankMachine(float targetAngle)
{
	switch (tankMovementState)
	{
	case ROTATE:
	{//handles which direction the tank is facing depending on the position of the next node
		if (targetAngle > 1 && targetAngle < 180) {
			goLeft();
		}
		else if (targetAngle < -1 && targetAngle > -180) {
			goRight();
		}
		else if (targetAngle > 180) {
			goRight();
		}
		else if (targetAngle < -180) {
			goLeft();
		}
	} break;
	case MOVING_TOWARD:
	{//move forward if facing the right direction
		goForward();
	}
	break;
	case IDLE:
	{
		stop();
	}
	break;
	case MOVING_BACKWARD:
	{//move backward if collision detected
		goBackward();
		if (coliisionTimer< 20)
		{
			collisionDetected = true;
		}
		if (coliisionTimer > 30)
		{
			collisionDetected = false;
			coliisionTimer = 0;
		}
	}
	break;
	default:
		break;
	}
}
void Henry::pathFindingMachine() {
	reachedFinalGoal = tankReachedGoal(goalNodeX, goalNodeY);
	//switch to different state based on the data the AI gathered
	if (collisionDetected)
	{
		pathFindingState = COLLISION;
	}
	if (pathFindingState == COLLISION&&!collisionDetected)
	{
		foundPath = false;
		reachedFinalGoal = false;
		pathFindingState = FINISH_PRE_PATH;
	}
	if (pathFindingState == STAND)
	{
		if ((!detectedEnemy&&detectingEnemy)||!hasAmmo())
		{
			foundPath = false;
			reachedFinalGoal = false;
			pathFindingState = FINISH_PRE_PATH;
		}
	}
	if (pathFindingState == MOVE_DIAGONAL)
	{
		if (reachedFinalGoal)
		{
			foundPath = false;
			reachedFinalGoal = false;
			pathFindingState = MOVE_TO_OPP_Y;
		}
	}
	if (pathFindingState == MOVE_TO_OPP_Y)
	{
		if (reachedFinalGoal)
		{
			foundPath = false;
			reachedFinalGoal = false;
			pathFindingState = MOVE_DIAGONAL;
		}
	}
	if (pathFindingState == MOVE_TO_OPP_X)
	{
		if (reachedFinalGoal)
		{
			foundPath = false;
			reachedFinalGoal = false;
			pathFindingState = MOVE_DIAGONAL;
		}
	}
	if (pathFindingState == FINISH_PRE_PATH)
	{
 		if (reachedFinalGoal)
		{
			foundPath = false;
			reachedFinalGoal = false;
			pathFindingState = MOVE_TO_OPP_Y;
		}
	}
	runPathFindingMachine();
}
void Henry::runPathFindingMachine()
{
	switch (pathFindingState)
	{
	case STAND:
	{
		path.clear();
		clearMovement();
	}
	break;
	break;
	case MOVE_TO_OPP_X:
	{
		//find path based on where the AI tank is located
		if (tankLocated("TOP_RIGHT"))
		{
			findPathTo(TOP_LEFT, MOVE_TO_OPP_X);
		}
		else if (tankLocated("BOTTOM_RIGHT"))
		{
			findPathTo(BOTTOM_LEFT, MOVE_TO_OPP_X);
		}
		else if (tankLocated("TOP_LEFT"))
		{
			findPathTo(TOP_RIGHT, MOVE_TO_OPP_X);
		}
		else if (tankLocated("BOTTOM_LEFT"))
		{
			findPathTo(BOTTOM_RIGHT, MOVE_TO_OPP_X);
		}
	}
		break;
	case MOVE_TO_OPP_Y:
	{
		//find path based on where the AI tank is located
		if (tankLocated("BOTTOM_LEFT"))
		{
			findPathTo(TOP_LEFT, MOVE_TO_OPP_Y);
		}
		else if (tankLocated("TOP_LEFT"))
		{
			findPathTo(BOTTOM_LEFT, MOVE_TO_OPP_Y);
		}
		else if (tankLocated("BOTTOM_RIGHT"))
		{
			findPathTo(TOP_RIGHT, MOVE_TO_OPP_Y);
		}
		else if (tankLocated("TOP_RIGHT"))
		{
			findPathTo(BOTTOM_RIGHT, MOVE_TO_OPP_Y);
		}
	}
		break;
	case MOVE_DIAGONAL:
	{
		//find path based on where the AI tank is located
		if (tankLocated("BOTTOM_LEFT"))
		{
			findPathTo(TOP_RIGHT, MOVE_DIAGONAL);
		}
		else if (tankLocated("TOP_LEFT"))
		{
			findPathTo(BOTTOM_RIGHT, MOVE_DIAGONAL);
		}
		else if (tankLocated("BOTTOM_RIGHT"))
		{
			findPathTo(TOP_LEFT, MOVE_DIAGONAL);
		}
		else if (tankLocated("TOP_RIGHT"))
		{
			findPathTo(BOTTOM_LEFT, MOVE_DIAGONAL);
		}
		else if (tankLocated("LEFT"))
		{
			findPathTo(BOTTOM_RIGHT, MOVE_DIAGONAL);
		}
		else if (tankLocated("RIGHT"))
		{
			findPathTo(BOTTOM_LEFT, MOVE_DIAGONAL);
		}
	}
	break;
	case FINISH_PRE_PATH:
	{
		bool addPreviousX = false;
		bool addPreviousY = false;
		int x = randomizeXPosition(lastArea, addPreviousX);
		int y = randomizeYPosition(lastArea, addPreviousY);
		runAStarPathFinding(y, x);
	}
	break;
	case COLLISION:
	{
		//randomize enum type
		lastArea = static_cast<LastArea>(rand() % SPAWN);
	}
	break;
	default:
		break;
	}
}

void Henry::findPathTo(LastArea destination, PathFindingState whatState) {
	bool addPreviousX;
	bool addPreviousY;
	switch (whatState)
	{
	case MOVE_DIAGONAL:
	{
		addPreviousX = false;
		addPreviousY = false;
	}
	break;
	case MOVE_TO_OPP_X:
	{
		addPreviousX = false;
		addPreviousY = true;
	}
	break;
	case MOVE_TO_OPP_Y:
	{
		addPreviousX = true;
		addPreviousY = false;
	}
	break;
	case FINISH_PRE_PATH:
	{
		addPreviousX = false;
		addPreviousY = false;
	}
	break;
	default:
	{
		addPreviousX = false;
		addPreviousY = false;
	}
		break;
	}
	int x = randomizeXPosition(destination, addPreviousX);
	int y = randomizeYPosition(destination, addPreviousY);
	runAStarPathFinding(y, x);
	if (whatState != FINISH_PRE_PATH)
	{
		lastArea = destination;
	}
}
//find the shortest path using A star algorithem
void Henry::runAStarPathFinding(int x,int y)
{
	int row = x;
	int column = y;
	while (map->mapArray[row][column]->isTraversable()==false)
	{
		(row <= centreX) ? row =+ 1 : row =- 1;
		(column <= centreY) ? column =+ 1 : column =- 1;
	}
	while (!foundPath)
	{
		path.clear();
		if (aStar->pathExists(*currentNode, *map->mapArray[row][column]))
		{
			path = aStar->getPath();
			foundPath = true;
			goalNodeY = row;
			goalNodeX = column;
		}
	}
}
//check where the tank is located
bool Henry::tankLocated(string position)
{
	int nodeRow = currentNode->getRow();
	int nodeColumn = currentNode->getColumn();
	if (position =="TOP_LEFT")
	{
		return (nodeColumn < centreX && nodeRow < centreY);
	}
	else if (position == "TOP_RIGHT")
	{
		return (nodeColumn > centreX && nodeRow < centreY);
	}
	else if (position == "BOTTOM_LEFT")
	{
		return (nodeColumn < centreX &&nodeRow > centreY);
	}
	else if (position == "BOTTOM_RIGHT")
	{
		return(nodeColumn > centreX && nodeRow > centreY);
	}
	else if (position == "CENTRE")
	{
		return (nodeColumn == centreY&& nodeRow == centreX);
	}
	else if (position == "LEFT")
	{ 
		return (nodeColumn == centreY&& nodeRow < centreX);
	}
	else if (position =="RIGHT")
	{ 
		return (nodeColumn == centreY&& nodeRow > centreX);
	}
	else if (position =="TOP")
	{ 
		return (nodeColumn < centreY&& nodeRow == centreX);
	}
	else if (position =="BOTTOM")
	{ 
		return (nodeColumn > centreY&& nodeRow == centreX);
	}
	return false;
}
//check if the tank reached goal node
bool Henry::tankReachedGoal(int row, int column)
{
	int nodeRow = currentNode->getRow();
	int nodeColumn = currentNode->getColumn();
	if ((nodeColumn == row)&&(nodeRow == column))
	{
		return true;
	}
	return false;
}
//randomize the x position within a given area
int Henry::randomizeXPosition(LastArea destination, bool addPrevious)
{
	int nodeColumn = currentNode->getColumn();
	switch (destination)
	{
	case TOP_LEFT:{
		int x;
		addPrevious ?  x = nodeColumn + (int)(rand() % 2): x = (int)(rand() % 2 + 3);
		return x;
	}
 break;
	case BOTTOM_LEFT: {		
		int x;
		addPrevious ? x = nodeColumn + (int)(rand() % 2) : x = (int)(rand() % 2 + 3);
		return x;
	}
 break;
case TOP_RIGHT:{	
	int x;
	addPrevious ? x = nodeColumn - (int)(rand() % 2) : x = (int)(rand() % 2 + (mapMaxColumn - 3));
	return x;
}
 break;
case BOTTOM_RIGHT:
{				
	int x;
	addPrevious ? x = nodeColumn - (int)(rand() % 2) : x = (int)(rand() % 2 + (mapMaxColumn - 3));
	return x;
}

 break;
default:
 break;
	}
	return 0;
}
int Henry::randomizeYPosition(LastArea destination, bool addPrevious)
{
	int nodeRow = currentNode->getRow();
	switch (destination)
	{
	case TOP_LEFT: 
	{
		int y;
		addPrevious ? y = nodeRow + (int)(rand() % 1 - 1): y = (int)(rand() % 2 + 3);
		return y;
	}
	break;
	case BOTTOM_LEFT: 
	{
		int y;
		addPrevious ? y = nodeRow + (int)(rand() % 1 - 1) : y = (int)(rand() % 2 + (mapMaxRow - 3));
		return y;
	}
	break;
	case TOP_RIGHT: 
	{
		int y;
		addPrevious ? y = nodeRow + (int)(rand() % 1 - 1) : y = (int)(rand() % 2 + 3);
		return y;
	}
	break;
	case BOTTOM_RIGHT: 
	{
		int y;
		addPrevious ? y = nodeRow + (int)(rand() % 1 - 1) : y = (int)(rand() % 2 + (mapMaxRow - 3));
		return y; 
	}
	break;
	default:
		break;
	}
}