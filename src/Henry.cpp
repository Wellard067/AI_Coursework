#include "Henry.h"



Henry::Henry()
{
}


Henry::~Henry()
{
}

void Henry::move()
{
	turretMachine();
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
	directionOfTarget = atan2(xDistanceBetweenAITankAndTarget, yDistanceBetweenAITankAndTarget) * 180 / PI;
	turrentFacingDirection = directionOfTarget + 180;

	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	distanceBetweenThisAndTarget = sqrt(dx*dx + dy*dy);
	std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markBase(Position p)
{
	detectedEnemy = true;
	float deltaX = getX() - p.getX();
	std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markShell(Position p)
{
	std::cout << "Shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::markEnemy(Position p)
{
	detectedEnemy = true;
	enemyType = TANK;
	std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Henry::turretMachine()
{
	if (turretState == DETECTION&& detectedEnemy)
	{
		if (enemyType == TANK)
		{
			turretState = SHOOTINGTANK;
		}
		else if (enemyType == BASE)
		{
			turretState = SHOOTINGBASE;
		}
	}
	if (getNumberOfShells() == 0&&(turretState==SHOOTINGBASE||turretState==SHOOTINGTANK))
	{
		turretState = NOAMMO;
	}
	if (turretState == AIM)
	{

	}
	if (turretState == SHOOTINGBASE)
	{

	}
	if (turretState == SHOOTINGBASE&&(baseIsDead||tankenDamageRecently))
	{
		turretState = DETECTION;
	}
	if (turretState == SHOOTINGTANK && enemyIsDead)
	{
		turretState = DETECTION;
	}
	switch (turretState)
	{
	case DETECTION:
		turretGoRight();
		break;
	case AIM:

		stopTurret();

		break;
	case SHOOTINGTANK:
		std::cout << "CurrentState (" << turretState<< ")\n";
		stopTurret();
		break;
	case SHOOTINGBASE:
		std::cout << "CurrentState (" << turretState << ")\n";
		stopTurret();
		break;
	case NOAMMO:
		break;
	default:
		break;
	}

}

void Henry::tankMachine()
{
	enum TankState {ROTATE,DETECTION,MOVINGTOWARD,IDLE,RUNAWAY};
	TankState tankState = DETECTION;
	if (tankState==DETECTION&&detectedEnemy||distanceBetweenThisAndTarget > safeDistance)
	{
		tankState = ROTATE;
	}
	if (tankState==ROTATE&&tankFacingDirection== directionOfTarget)
	{
		tankState = MOVINGTOWARD;
	}
	if (tankState == MOVINGTOWARD&&distanceBetweenThisAndTarget<safeDistance)
	{
		tankState = IDLE;
	}
	if (tankState == IDLE&&enemyIsDead)
	{
		tankState = DETECTION;
	}
	if (tankState == MOVINGTOWARD && getNumberOfShells()<=0)
	{
		tankState = RUNAWAY;
	}
	switch (tankState)
	{
	case DETECTION:
		break;
	case ROTATE:
		break;
	case MOVINGTOWARD:
		break;
	case IDLE:
		break;
	case RUNAWAY:
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

