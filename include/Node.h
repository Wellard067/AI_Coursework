#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
class Node
{
private:
	int xPos;
	int yPos;
	int height;
	int width;	
	int row;
	int column;
	float f; //total cost of the note F= G+H
	float g;//distance of current to start , geographical score
	float h;//distance of current to goal, heuristic score
	//see if this node is diagonal to the parent node
	bool diagonal;
	//Using SFML to draw the nodes
	sf::RectangleShape rect;
	//new_g and old_g are used to check if the G score can be improved
	//new g score
	float new_g;
	//old g score
	float old_g;
	//see if the node is calculating G score for the first time
	bool firstTimeCalG;
	//see if the node is traversable
	bool traversable;
public:
	//Node(int xPosIn, int yPosin, int heightIn, int widthIn, int r, int c);
	Node(float xPosIn, float yPosIn, float heightIn, float widthIn, int r, int c);
	void DrawNode(sf::RenderTarget & target);
	~Node();
	bool current = true;
	//Drawing
	void setWall();
	void setPath();
	void setGoal();
	void setStart();
	void setOpen();
	void setClosed();
	//setter
	void setCurrent(bool current);
	void setRow(int row);
	void setColumn(int column);	
	void setGScore(float value);
	void setDiagonal(bool diagonal);	
	bool isTraversable();
	//getter
	float getFScore() const;
	float getGScore() const;
	float getHScore() const;
	int getX() const;
	int getY() const;
	int getRow() const;
	int getColumn() const;
	bool isDiagonal();
	bool equals(Node &otherNode);
	//calculate F, H, G scores
	void calculate_F();
	void calculate_H(Node &goalNode);
	void calcuate_G(float parentGScore);
	//check if the G can be improve
	bool canImproveG();
	//improve G score
	void improveG();
};

