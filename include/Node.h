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
	float f; //total cost of the note F= G+H
	float g;//distance of current to start , geographical score
	float h;//distance of current to goal, heuristic score
	int index;
	int parent_index;	
	int row;
	int column;
	bool neighborToCurrentNode;
	bool diagonal;
	//Using SFML to draw the nodes
	sf::RectangleShape rect;
	float new_g;
	float old_g;
	bool firstTimeCalG;
	bool traversable;
public:
	//Node(int xPosIn, int yPosin, int heightIn, int widthIn, int r, int c);
	Node(float xPosIn, float yPosIn, float heightIn, float widthIn, int r, int c);
	void DrawNode(sf::RenderTarget & target);
	~Node();
	bool current = true;
	int getX();
	int getY();
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
	void setParentIndex(float value);
	void setGScore(float value);
	void setIndex(int index);
	void setDiagonal(bool diagonal);	
	bool isTraversable();
	//getter
	float getFScore() const;
	float getGScore();
	float getHScore();
	int getRow();
	int getColumn();
	int getIndex();
	bool isDiagonal();
	//calculation
	bool equals(Node &otherNode);
	void calculate_F();
	void calcuate_G(float parentGScore);
	void improveG();
	bool canImproveG();
	void calculate_H(Node &goalNode);
};

