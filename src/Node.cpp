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
	traversable = false;
}

void Node::setPath()
{
	traversable = true;
	rect.setFillColor(sf::Color(100, 255, 255, 50));
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
bool Node::equals(Node &otherNode)
{
	return (row == otherNode.getRow() && column == otherNode.getColumn());
}

void Node::calcuate_G(float parentGScore) {
	//Add 1.414 to cost if the
	if (firstTimeCalG)
	{
		diagonal ? g = parentGScore + 1.414f : g = parentGScore + 1.0f;
		diagonal ? new_g = parentGScore + 1.414f : new_g = parentGScore + 1.0f;
		firstTimeCalG = false;
	}
	else
	{
		diagonal ? new_g = parentGScore + 1.414f : new_g = parentGScore + 1.0f;
		//std::cout << " Node Row :" << row << " Node Col :" << column << " G Score : " << g << " New G Score" << new_g << std::endl;
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
;
