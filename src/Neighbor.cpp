#include "Neighbor.h"


Neighbor::Neighbor(int row, int columns, bool diagonal)
{
	this->row = row;
	this->columns = columns;
	this->diagonal = diagonal;
	this->traversable = true;
}

Neighbor::~Neighbor()
{
}

void Neighbor::setTraversable(bool exists)
{
	this->traversable = exists;
}

bool Neighbor::isTraversable()
{
	return traversable;
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
