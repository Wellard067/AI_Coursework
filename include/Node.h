#pragma once
#include <cmath>
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
public:
	Node(int xPosIn, int yPosin, int heightIn, int widthIn, int r, int c);
	~Node();
	bool current = true;
	bool notPath = false;
	bool path = true;
	int getX();
	int getY();
	
	void setWall();
	void setPath();
	void setGoal();
	void setStart();
	void setOpen();
	void setClosed();

	int lowerOutcome(const Node other) const;
	//setter
	void setCurrent(bool current);
	void setRow(int row);
	void setColumn(int column);	
	void setParentIndex(float value);
	void setGScore(float value);
	void setIndex(int index);
	void setDiagonal(bool diagonal);
	//getter
	float getFScore() const;
	float getGScore();
	float getHScore();
	int getRow();
	int getColumn();
	int getIndex();
	bool isDiagonal();
	//calculation
	void calculate_H(Node* goalNode);
	void calculate_F();
	void calculate_Score(float parentGScore, Node * goalNode);
	bool equals(Node * otherNode);
	void calcuate_G(float parentGScore);
};

