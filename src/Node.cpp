#include "Node.h"

Node::Node(int xPosIn,int yPosIn, int heightIn, int widthIn, int r, int c)
{
	xPos = xPosIn;
	yPos = yPosIn;
	row = r;
	column = c;
	height = heightIn;
	width = widthIn;
	current = false;
	neighborToCurrentNode = false;
	diagonal = false;
}

Node::~Node()
{
}

int Node::getX()
{
	return xPos + width / 2;
}

int Node::getY()
{
	return yPos + height / 2;
}

void Node::setWall()
{
	path = false;
}

void Node::setPath()
{
	path = true;
}

void Node::setGoal()
{
}

void Node::setStart()
{
}

void Node::setOpen()
{
}

void Node::setClosed()
{
}

void Node::setCurrent(bool current)
{
	this->current = current;
}

void Node::setRow(int row)
{
	 this->row = row;
}

void Node::setColumn(int column)
{
	this->column = column;
}

int Node::lowerOutcome(const Node other) const
{
	return 0;
}

float Node::getFScore() const
{
	return f;
}

float Node::getGScore()
{
	return g;
}

float Node::getHScore()
{
	return h;
}

int Node::getRow()
{
	return row;
}

int Node::getColumn()
{
	return column;
}

int Node::getIndex()
{
	index = column*width + row;
	return index;
}

bool Node::isDiagonal()
{
	return diagonal;
}

void Node::setParentIndex(float parent_index)
{
	this->parent_index = parent_index;
}

void Node::setGScore(float g)
{
	this->g = g;
}

void Node::setIndex(int index)
{
	this->index = index;
}

void Node::setDiagonal(bool diagonal)
{
	this->diagonal = diagonal;
}
bool Node::equals(Node *otherNode)
{
	return (row == otherNode->getRow() && column == otherNode->getColumn());
}

void Node::calcuate_G(float parentGScore) {

	diagonal ? g = parentGScore + 1.414f : parentGScore + 1.0f;
}

void Node::calculate_H(Node* goalNode) {
	int goalX = goalNode->getX();
	int goalY = goalNode->getY();
	float dx, dy;
	//Calculate distance between current node and goal node 
	dx = (float)(goalX - xPos);
	dy = (float)(goalX - yPos);
	h = sqrt(dx*dx + dy*dy);
}

void Node::calculate_F()
{
	f = g + h;
}
;
