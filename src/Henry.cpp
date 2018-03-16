#include "Henry.h"



Henry::Henry()
{
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
	turretAimMachine();
	turretfiringMachine();
	tankMachine();
}

void Henry::reset()
{
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
	//if (tankMovementState==ROTATE&&tankFacingDirection== angleBetweeThisAndTarget)
	//{
	//	tankMovementState = MOVINGTOWARD;
	//}
	//if (tankMovementState == MOVINGTOWARD&&distanceBetweenThisAndTarget<safeDistance)
	//{
	//	tankMovementState = IDLE;
	//}
	//if (tankMovementState == IDLE&&enemyIsDead)
	//{
	//}
	if (tankMovementState == MOVINGTOWARD && getNumberOfShells()<=0)
	{
		tankMovementState = RUNAWAY;
	}
	if (tankMovementState == IDLE && detectedShell) 
	{
		tankMovementState = DODGESHELL;
	}
	switch (tankMovementState)
	{
	case ROTATE:
		break;
	case MOVINGTOWARD:
		break;
	case IDLE:
		ShellDetectedTimer = 0;
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
				if (bearingDegrees >181)
				{
					backward = false;
					forward = true;
				}
				if (bearingDegrees <179)
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

bool Henry::isFiring()
{
	return firing;
}

void Henry::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}

