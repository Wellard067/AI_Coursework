#include "Node.h"

Node::Node(float xPosIn, float yPosIn, float heightIn, float widthIn, int r, int c)
{
	xPos = xPosIn;
	yPos = yPosIn;
	row = r;
	column = c;
	height = heightIn;
	width = widthIn;
	current = false;
	diagonal = false;
	traversable = true;
	//set the size, position and color of the nodes
	rect.setSize(sf::Vector2f(width, height));
	rect.setPosition(sf::Vector2f(xPos, yPos));
	rect.setFillColor(sf::Color(255, 255, 255, 50));
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color(255, 255, 255, 100));
}
void Node::DrawNode(sf::RenderTarget &target) {
	target.draw(rect);
}

Node::~Node()
{
}

int Node::getX() const
{
	return xPos + width / 2;
}

int Node::getY() const
{
	return yPos + height / 2;
}

void Node::setWall()
{
	traversable = false;
	rect.setFillColor(sf::Color(100, 255, 255, 50));
}

void Node::setPath()
{
	traversable = true;
	rect.setFillColor(sf::Color(255, 255, 255, 50));
}
bool Node::isTraversable()
{
	return traversable;
}

void Node::setGoal()
{
}

void Node::setStart()
{
}

void Node::setOpen()
{
	rect.setFillColor(sf::Color(200, 100,100, 50));
}

void Node::setClosed()
{
	rect.setFillColor(sf::Color(100, 150,150, 50));
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


float Node::getFScore() const
{
	return f;
}

float Node::getGScore() const
{
	return g;
}

float Node::getHScore() const
{
	return h;
}


int Node::getRow() const
{
	return row;
}

int Node::getColumn() const
{
	return column;
}

bool Node::isDiagonal()
{
	return diagonal;
}


void Node::setGScore(float g)
{
	this->g = g;
}


void Node::setDiagonal(bool diagonal)
{
	this->diagonal = diagonal;
}
bool Node::equals(Node &otherNode)
{
	return (row == otherNode.getRow() && column == otherNode.getColumn());
}

void Node::calcuate_G(float parentGScore) {
	//Add 1.414 to cost if this node is diangal to its parent node
	if (firstTimeCalG)
	{
		diagonal ? g = parentGScore + 1.414f : g = parentGScore + 1.0f;
		diagonal ? new_g = parentGScore + 1.414f : new_g = parentGScore + 1.0f;
		firstTimeCalG = false;
	}
	else
	{
		diagonal ? new_g = parentGScore + 1.414f : new_g = parentGScore + 1.0f;
	}
}

void Node::improveG()
{
	this->g = new_g;
}
bool Node::canImproveG()
{
	return g >= new_g;
}
void Node::calculate_H(Node &goalNode) {
	int goalX = goalNode.getX();
	int goalY = goalNode.getY();
	float dx, dy;
	//Calculate distance between current node and goal node 
	dx = (float)(goalX - xPos);
	dy = (float)(goalY - yPos);
	h = sqrt(dx*dx + dy*dy);
}

void Node::calculate_F()
{
	f = g + h;
}

