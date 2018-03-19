#include "Neighbor.h"


Neighbor::Neighbor(int row, int columns, bool diagonal)
{
	this->row = row;
	this->columns = columns;
	this->diagonal = diagonal;
	this->exists = true;
}

Neighbor::~Neighbor()
{
}

void Neighbor::setExists(bool exists)
{
	this->exists = exists;
}

bool Neighbor::isExists()
{
	return exists;
}
int Neighbor::getRow()
{
	return row;
}
int Neighbor::getColumns()
{
	return columns;
}

bool Neighbor::isDiagonal()
{
	return diagonal;
}
